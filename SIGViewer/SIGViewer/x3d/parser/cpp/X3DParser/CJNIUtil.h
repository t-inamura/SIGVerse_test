#ifndef _CJNIUTIL_H_
#define _CJNIUTIL_H_

#include <jni.h>

class CJNIUtil
{
public:
	// ========================================================
	// ========================================================

	// -----------------------------------------------
	// -----------------------------------------------
	static bool init(char *confFile);

	// -----------------------------------------------
	// -----------------------------------------------
	static void destroy();

	// -----------------------------------------------
	// -----------------------------------------------
	static CJNIUtil *getUtil();

	// -----------------------------------------------
	// -----------------------------------------------
	static JNIEnv *getEnv();

	// -----------------------------------------------
	// -----------------------------------------------
	bool createJavaVM(char *confFile);

	// -----------------------------------------------
	// -----------------------------------------------
	void destroyJavaVM();

	JNIEnv *env() { return m_env; }
	JavaVM *jvm() { return m_jvm; }

	// -----------------------------------------------
	// -----------------------------------------------
	jclass getClass(char *className);

	// -----------------------------------------------
	// -----------------------------------------------
	jobject newInstance(char *className);
	jobject newInstance(jclass c);

	// -----------------------------------------------
	// -----------------------------------------------
	jmethodID getMethodID(char *className, char *methodName, char *methodSig);
	jmethodID getMethodID(jclass c, char *methodName, char *methodSig);


	// ========================================================
	// ========================================================

	// -----------------------------------------------
	// -----------------------------------------------
	jclass getClassOfVRMLNode();

	// -----------------------------------------------
	// -----------------------------------------------
	bool isInstanceOfVRMLNode(jobject obj);

	// -----------------------------------------------
	// -----------------------------------------------
	bool X3DParser_parse(jobject x3dParser, char *fname);
	jobjectArray X3DParser_getChildrenOfRootNode(jobject x3dParser);
	jobjectArray X3DParser_getDefNames(jobject x3dParser);
	jobject X3DParser_getDefNode(jobject x3dParser, char *defName);

	// -----------------------------------------------
	// -----------------------------------------------
	char *VRMLNode_getNodeName(jobject vrmlNode);
	int VRMLNode_getNumFields(jobject vrmlNode);
	jobject VRMLNode_getFieldDeclaration(jobject vrmlNode, int i);
	int VRMLNode_getFieldIndex(jobject vrmlNode, char *fieldName);

	// -----------------------------------------------
	// -----------------------------------------------
	jobject VRMLNodeType_getFieldValue(jobject vrmlNode, int i);

	// -----------------------------------------------
	// -----------------------------------------------
	char *VRMLFieldDeclaration_getName(jobject vrmlDecl);
	int VRMLFieldDeclaration_getFieldType(jobject vrmlDecl);
	char *VRMLFieldDeclaration_getFieldTypeString(jobject vrmlDecl);

private:
	JNIEnv *m_env;
	JavaVM *m_jvm;

	jclass m_class_VRMLNode;

	// ------------------------------------------------
	// ------------------------------------------------
	CJNIUtil();
	virtual ~CJNIUtil();
};

#endif

