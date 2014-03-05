#include "CJNIUtil.h"
#include "SgvConfigFile.h"
#include "CX3DParser.h"
#include <string.h>
#include <jni.h>
#include <windows.h>

CJNIUtil *g_jniUtil = NULL;

// -------------------------------------------
//	static methods
// -------------------------------------------
bool CJNIUtil::init(char *confFile)
{
	if (!g_jniUtil)
	{
		g_jniUtil = new CJNIUtil();
		if (g_jniUtil)
		{
			if (g_jniUtil->createJavaVM(confFile))
			{
				return true;
			}
			else
			{
				delete g_jniUtil;
				g_jniUtil = NULL;
				return false;
			}
		}
	}

	return true;
}

void CJNIUtil::destroy()
{
	if (g_jniUtil)
	{
		g_jniUtil->destroyJavaVM();
		delete g_jniUtil;
		g_jniUtil = NULL;
	}
}

CJNIUtil *CJNIUtil::getUtil()
{
	return g_jniUtil;
}

JNIEnv *CJNIUtil::getEnv()
{
	if (!g_jniUtil) return NULL;

	return g_jniUtil->env();
}

// -------------------------------------------
//	class methods
// -------------------------------------------
CJNIUtil::CJNIUtil()
{
	m_jvm = NULL;
	m_env = NULL;

	m_class_VRMLNode = NULL;
}

CJNIUtil::~CJNIUtil()
{
	if (m_jvm)
	{
		m_jvm->DestroyJavaVM();
		m_jvm = NULL;
	}
}

// begin(fix)(2009/11/30)
// modified by sekikawa
bool CJNIUtil::createJavaVM(char *confFile)
{
	if (m_jvm) { return true; }

	JavaVMOption *options = NULL;
	JavaVMInitArgs vm_args;
	const char *pJavaClassPath = NULL;
	const char *pJavaLibraryPath = NULL;
	char javaClassPath[1024];
	char javaLibraryPath[1024];
	int iOption, nOptions;
	int nOtherOptions;
	jint ret;

	memset(javaClassPath, 0, sizeof(javaClassPath));
	memset(javaLibraryPath, 0, sizeof(javaLibraryPath));

	Sgv::ConfigFile conf;

	if (!conf.load(confFile))
	{
		CX3DParser::printLog("Failed to load config file (%s)\n", confFile);
		return false;
	}

	nOptions = 0;

	pJavaClassPath = conf.getStringValue("java.class.path");
	if (pJavaClassPath)
	{
		sprintf(javaClassPath, "-Djava.class.path=%s", pJavaClassPath);
		nOptions++;
	}

	pJavaLibraryPath = conf.getStringValue("java.library.path");
	if (pJavaLibraryPath)
	{
		sprintf(javaLibraryPath, "-Djava.library.path=%s", pJavaLibraryPath);
		nOptions++;
	}

	nOtherOptions = conf.countOtherOptions();

	nOptions += nOtherOptions;

	options = (JavaVMOption *)malloc(sizeof(JavaVMOption) * nOptions);
	if (!options)
	{
		CX3DParser::printLog("Failed to alloc memory for setting JavaVM Options.\n");
		return false;
	}

	iOption = 0;

	if (strlen(javaClassPath) > 0)
	{
		options[iOption++].optionString = javaClassPath;
	}

	if (strlen(javaLibraryPath) > 0)
	{
		options[iOption++].optionString = javaLibraryPath;
	}

	for (int iOtherOption=0; iOtherOption<nOtherOptions; iOtherOption++)
	{
		options[iOption].optionString = (char *)conf.getOtherOption(iOtherOption);

		iOption++;
	}

	CX3DParser::printLog("*** JavaVMOption ***\n");
	for (int i=0; i<nOptions; i++)
	{
		CX3DParser::printLog("options[%d].optionString = (%s)\n", i, options[i].optionString);
	}

	// -----------------------------------------------
	//	fill in startup structure
	// -----------------------------------------------
	vm_args.version = JNI_VERSION_1_2;
	JNI_GetDefaultJavaVMInitArgs(&vm_args);
	vm_args.options = options;
	vm_args.nOptions = nOptions;

	// -----------------------------------------------
	// -----------------------------------------------
	ret = JNI_CreateJavaVM(&m_jvm, (void **)&m_env, &vm_args);
	if (ret < 0)
	{
		CX3DParser::printLog("Failed to create Java VM\n");
		return false;
	}

	CX3DParser::printLog("Java VM start ok\n");

	// -----------------------------------------------
	// -----------------------------------------------
	free(options);
	options = NULL;

	return true;
}
// end(fix)(2009/11/30)

void CJNIUtil::destroyJavaVM()
{
	// -----------------------------------------------
	//	destroy Java VM
	// -----------------------------------------------
	if (m_jvm)
	{
		m_jvm->DestroyJavaVM();
		m_jvm = NULL;
	}
}

jclass CJNIUtil::getClass(char *className)
{
	if (!m_env)
	{
		CX3DParser::printLog("java vm haven't started\n");
		return NULL;
	}

	jclass c = m_env->FindClass(className);
	if (!c)
	{
		CX3DParser::printLog("cannot find %s.class\n", className);
		return NULL;
	}

	return c;
}

jobject CJNIUtil::newInstance(char *className)
{
	jclass c = getClass(className);
	if (!c) return NULL;

	return newInstance(c);
}

jobject CJNIUtil::newInstance(jclass c)
{
	if (!m_env)
	{
		CX3DParser::printLog("java vm haven't started\n");
		return NULL;
	}

	// get methodID of constructor
	jmethodID midConstructor = m_env->GetMethodID(c, "<init>", "()V");

	// call constructor to create new instance
	jobject obj = m_env->NewObject(c, midConstructor);
	if (!obj)
	{
		CX3DParser::printLog("cannot create object\n");
		return NULL;
	}

	return obj;
}

jmethodID CJNIUtil::getMethodID(char *className, char *methodName, char *methodSig)
{
	jclass c = getClass(className);
	if (!c) return NULL;

	return getMethodID(c, methodName, methodSig);
}

jmethodID CJNIUtil::getMethodID(jclass c, char *methodName, char *methodSig)
{
	if (!m_env)
	{
		CX3DParser::printLog("java vm haven't started\n");
		return NULL;
	}

	jmethodID mid = m_env->GetMethodID(c, methodName, methodSig);
	if (!mid)
	{
		CX3DParser::printLog("cannot get methodID of %s\n", methodName);
		return NULL;
	}

	return mid;
}

jclass CJNIUtil::getClassOfVRMLNode()
{
	if (m_class_VRMLNode) return m_class_VRMLNode;

	m_class_VRMLNode = getClass("org/web3d/vrml/lang/VRMLNode");

	return m_class_VRMLNode;
}

bool CJNIUtil::isInstanceOfVRMLNode(jobject obj)
{
	jclass vrmlNodeClass = getClassOfVRMLNode();

	return (m_env->IsInstanceOf(obj, vrmlNodeClass)) ? true : false;
}

// -----------------------------------------------
// -----------------------------------------------
bool CJNIUtil::X3DParser_parse(jobject x3dParser, char *fname)
{
	if (!m_env)
	{
		CX3DParser::printLog("java vm haven't started\n");
		return false;
	}

	if (!x3dParser) return false;

	// -----------------------------------------------
	//	get method id of X3DParse.parse()
	// -----------------------------------------------
	char *className = "X3DParser";
	char *methodName = "parse";
	char *methodSig = "(Ljava/lang/String;)Z";

	jmethodID mid = getMethodID(className, methodName, methodSig);
	if (!mid)
	{
		CX3DParser::printLog("cannot get methodID of %s", methodName);
		return false;
	}

	// -----------------------------------------------
	//	make argument
	// -----------------------------------------------
	jstring utf8_fname = m_env->NewStringUTF(fname);

	// -----------------------------------------------
	//	call java's parser
	// -----------------------------------------------
	return (m_env->CallBooleanMethod(x3dParser, mid, utf8_fname) == JNI_TRUE) ? true : false;
}

jobjectArray CJNIUtil::X3DParser_getChildrenOfRootNode(jobject x3dParser)
{
	if (!m_env)
	{
		CX3DParser::printLog("java vm haven't started\n");
		return NULL;
	}

	if (!x3dParser) return NULL;

	// -----------------------------------------------
	//	get method id
	// -----------------------------------------------
	char *className = "X3DParser";
	char *methodName = "getChildrenOfRootNode";
	char *methodSig = "()[Lorg/web3d/vrml/lang/VRMLNode;";

	jmethodID mid = getMethodID(className, methodName, methodSig);
	if (!mid)
	{
		CX3DParser::printLog("cannot get methodID of %s", methodName);
		return NULL;
	}

	// -----------------------------------------------
	//	call java's method
	// -----------------------------------------------
	jobjectArray vrmlNodeArray = (jobjectArray)(m_env->CallObjectMethod(x3dParser, mid));

	return vrmlNodeArray;
}

jobjectArray CJNIUtil::X3DParser_getDefNames(jobject x3dParser)
{
	if (!m_env)
	{
		CX3DParser::printLog("java vm haven't started\n");
		return NULL;
	}

	if (!x3dParser) return NULL;

	// -----------------------------------------------
	//	get method id
	// -----------------------------------------------
	char *className = "X3DParser";
	char *methodName = "getDefNames";
	char *methodSig = "()[Ljava/lang/String;";

	jmethodID mid = getMethodID(className, methodName, methodSig);
	if (!mid)
	{
		CX3DParser::printLog("cannot get methodID of %s", methodName);
		return NULL;
	}

	// -----------------------------------------------
	//	call java's method
	// -----------------------------------------------
	jobjectArray defNames = (jobjectArray)(m_env->CallObjectMethod(x3dParser, mid));

	return defNames;
}

jobject CJNIUtil::X3DParser_getDefNode(jobject x3dParser, char *defName)
{
	if (!m_env)
	{
		CX3DParser::printLog("java vm haven't started\n");
		return NULL;
	}

	if (!x3dParser) return NULL;

	// -----------------------------------------------
	//	get method id
	// -----------------------------------------------
	char *className = "X3DParser";
	char *methodName = "getDefNode";
	char *methodSig = "(Ljava/lang/String;)Lorg/web3d/vrml/lang/VRMLNode;";

	jmethodID mid = getMethodID(className, methodName, methodSig);
	if (!mid)
	{
		CX3DParser::printLog("cannot get methodID of %s", methodName);
		return NULL;
	}

	// -----------------------------------------------
	//	make java's String from C/C++ char*
	// -----------------------------------------------
	jstring utf8_defName = m_env->NewStringUTF(defName);

	// -----------------------------------------------
	//	call java's method
	// -----------------------------------------------
	jobject defNode = m_env->CallObjectMethod(x3dParser, mid, utf8_defName);

	return defNode;
}

// -----------------------------------------------
// -----------------------------------------------
char *CJNIUtil::VRMLNode_getNodeName(jobject vrmlNode)
{
	static char buf[256];

	if (!m_env)
	{
		CX3DParser::printLog("java vm haven't started\n");
		return NULL;
	}

	if (!vrmlNode) return NULL;

	jmethodID mid = getMethodID(
		"org/web3d/vrml/lang/VRMLNode",
		"getVRMLNodeName",
		"()Ljava/lang/String;");

	if (!mid)
	{
		CX3DParser::printLog("method id not found [%s:%d]\n", __FILE__, __LINE__);
		return NULL;
	}

	jstring jNodeName = (jstring)(m_env->CallObjectMethod(vrmlNode, mid));

	if (!jNodeName) return NULL;

	jboolean isCopy;
	const char *pNodeName = m_env->GetStringUTFChars(jNodeName, &isCopy);
	if (pNodeName)
	{
#ifdef WIN32
		strcpy_s(buf, pNodeName);
#else
		strcpy(buf, pNodeName);
#endif
	}
	else
	{
		memset(buf, 0, sizeof(buf));
	}

	if (isCopy == JNI_TRUE)
	{
		m_env->ReleaseStringUTFChars(jNodeName, pNodeName);
	}

	return buf;
}

int CJNIUtil::VRMLNode_getNumFields(jobject vrmlNode)
{
	if (!m_env)
	{
		CX3DParser::printLog("java vm haven't started\n");
		return -1;
	}

	if (!vrmlNode) return 0;

	jmethodID mid = getMethodID(
		"org/web3d/vrml/lang/VRMLNode",
		"getNumFields",
		"()I");

	jint n = m_env->CallIntMethod(vrmlNode, mid);

	return n;
}

jobject CJNIUtil::VRMLNode_getFieldDeclaration(jobject vrmlNode, int i)
{
	if (!m_env)
	{
		CX3DParser::printLog("java vm haven't started\n");
		return NULL;
	}

	if (!vrmlNode) return NULL;

	jmethodID mid = getMethodID(
		"org/web3d/vrml/lang/VRMLNode",
		"getFieldDeclaration",
		"(I)Lorg/web3d/vrml/lang/VRMLFieldDeclaration;");

	jobject decl = m_env->CallObjectMethod(vrmlNode, mid, i);

	return decl;
}

int CJNIUtil::VRMLNode_getFieldIndex(jobject vrmlNode, char *fieldName)
{
	if (!m_env)
	{
		CX3DParser::printLog("java vm haven't started\n");
		return -1;
	}

	if (!vrmlNode) return 0;

	jmethodID mid = getMethodID(
		"org/web3d/vrml/lang/VRMLNode",
		"getFieldIndex",
		"(Ljava/lang/String;)I");

	jstring jFieldName = m_env->NewStringUTF(fieldName);

	jint index = m_env->CallIntMethod(vrmlNode, mid, jFieldName);


	return index;
}

// -----------------------------------------------
// -----------------------------------------------
jobject CJNIUtil::VRMLNodeType_getFieldValue(jobject vrmlNode, int i)
{
	if (!m_env)
	{
		CX3DParser::printLog("java vm haven't started\n");
		return NULL;
	}

	if (!vrmlNode) return NULL;

	jmethodID mid = getMethodID(
		"org/web3d/vrml/nodes/VRMLNodeType",
		"getFieldValue",
		"(I)Lorg/web3d/vrml/nodes/VRMLFieldData;");

	jobject vrmlFieldData = m_env->CallObjectMethod(vrmlNode, mid, i);

	return vrmlFieldData;
}

// -----------------------------------------------
// -----------------------------------------------
char *CJNIUtil::VRMLFieldDeclaration_getName(jobject vrmlDecl)
{
	static char buf[256];

	if (!m_env)
	{
		CX3DParser::printLog("java vm haven't started\n");
		return NULL;
	}

	if (!vrmlDecl) return NULL;

	jmethodID mid = getMethodID(
		"org/web3d/vrml/lang/VRMLFieldDeclaration",
		"getName",
		"()Ljava/lang/String;");

	jstring jFieldName = (jstring)m_env->CallObjectMethod(vrmlDecl, mid);

	if (!jFieldName) return NULL;

	jboolean isCopy;
	const char *pFieldName = m_env->GetStringUTFChars(jFieldName, &isCopy);
	if (pFieldName)
	{
#ifdef WIN32
		strcpy_s(buf, pFieldName);
#else
		strcpy(buf, pFieldName);
#endif
	}
	else
	{
		memset(buf, 0, sizeof(buf));
	}

	if (isCopy == JNI_TRUE)
	{
		m_env->ReleaseStringUTFChars(jFieldName, pFieldName);
	}

	return buf;
}

int CJNIUtil::VRMLFieldDeclaration_getFieldType(jobject vrmlDecl)
{
	if (!m_env)
	{
		CX3DParser::printLog("java vm haven't started\n");
		return -1;
	}

	if (!vrmlDecl) return -1;

	jmethodID mid = getMethodID(
		"org/web3d/vrml/lang/VRMLFieldDeclaration",
		"getFieldType",
		"()I");

	jint ftype = m_env->CallIntMethod(vrmlDecl, mid);

	return ftype;
}

char *CJNIUtil::VRMLFieldDeclaration_getFieldTypeString(jobject vrmlDecl)
{
	static char buf[256];

	if (!m_env)
	{
		CX3DParser::printLog("java vm haven't started\n");
		return NULL;
	}

	if (!vrmlDecl) return NULL;

	jmethodID mid = getMethodID(
		"org/web3d/vrml/lang/VRMLFieldDeclaration",
		"getFieldTypeString",
		"()Ljava/lang/String;");

	jstring jTypeName = (jstring)m_env->CallObjectMethod(vrmlDecl, mid);

	if (!jTypeName) return NULL;

	jboolean isCopy;
	const char *pTypeName = m_env->GetStringUTFChars(jTypeName, &isCopy);
	if (pTypeName)
	{
#ifdef WIN32
		strcpy_s(buf, pTypeName);
#else
		strcpy(buf, pTypeName);
#endif
	}
	else
	{
		memset(buf, 0, sizeof(buf));
	}

	if (isCopy == JNI_TRUE)
	{
		m_env->ReleaseStringUTFChars(jTypeName, pTypeName);
	}

	return buf;
}

