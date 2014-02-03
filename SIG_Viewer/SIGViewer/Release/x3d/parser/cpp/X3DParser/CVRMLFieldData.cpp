#include "CX3DParser.h"
#include "CVRMLFieldData.h"
#include "CJNIUtil.h"

#include <jni.h>

////////////////////////////////////////////////////////
//
// CVRMLFieldData

char *CVRMLFieldData::getFieldTypeName(CVRMLFieldDataType ftype)
{
	switch (ftype)
	{
		case UNKNOWN:
			return "UNKNOWN";

		case BOOLEAN_DATA:
			return "BOOLEAN_DATA";

		case INT_DATA:
			return "INT_DATA";

		case LONG_DATA:
			return "LONG_DATA";

		case FLOAT_DATA:
			return "FLOAT_DATA";

		case DOUBLE_DATA:
			return "DOUBLE_DATA";

		case STRING_DATA:
			return "STRING_DATA";

		case NODE_DATA:
			return "NODE_DATA";

		case BOOLEAN_ARRAY_DATA:
			return "BOOLEAN_ARRAY_DATA";

		case INT_ARRAY_DATA:
			return "INT_ARRAY_DATA";

		case LONG_ARRAY_DATA:
			return "LONG_ARRAY_DATA";

		case FLOAT_ARRAY_DATA:
			return "FLOAT_ARRAY_DATA";

		case DOUBLE_ARRAY_DATA:
			return "DOUBLE_ARRAY_DATA";

		case STRING_ARRAY_DATA:
			return "STRING_ARRAY_DATA";

		case NODE_ARRAY_DATA:
			return "NODE_ARRAY_DATA";
	}

	return "UNKNOWN";
}

void CVRMLFieldData::print(int indent)
{
	FILE *fp = CX3DParser::getDebugLogFp();

	CX3DParser::printIndent(indent);
	fprintf(fp, "[%s]", CVRMLFieldData::getFieldTypeName(getFieldType()));

	switch (getFieldType())
	{
	case BOOLEAN_DATA:
		{
			CVRMLBooleanData *boolData = (CVRMLBooleanData *)this;

			fprintf(fp, "\n");
			CX3DParser::printIndent(indent);
			fprintf(fp, "%s\n", boolData->getValue() ? "TRUE" : "FALSE");
		}
		break;

	case INT_DATA:
		{
			CVRMLIntData *intData = (CVRMLIntData *)this;

			fprintf(fp, "\n");
			CX3DParser::printIndent(indent);
			fprintf(fp, "%d\n", intData->getValue());
		}
		break;

	case FLOAT_DATA:
		{
			CVRMLFloatData *floatData = (CVRMLFloatData *)this;

			fprintf(fp, "\n");
			CX3DParser::printIndent(indent);
			fprintf(fp, "%f\n", floatData->getValue());
		}
		break;

	case DOUBLE_DATA:
		{
			CVRMLDoubleData *doubleData = (CVRMLDoubleData *)this;

			fprintf(fp, "\n");
			CX3DParser::printIndent(indent);
			fprintf(fp, "%f\n", doubleData->getValue());
		}
		break;

	case STRING_DATA:
		{
			CVRMLStringData *stringData = (CVRMLStringData *)this;

			fprintf(fp, "\n");
			CX3DParser::printIndent(indent);
			fprintf(fp, "%s\n", stringData->getValue());
		}
		break;

	case NODE_DATA:
		{
			CVRMLNodeData *nodeData = (CVRMLNodeData *)this;

			fprintf(fp, "\n");

			jobject vrmlNode = ((CVRMLNodeData *)nodeData)->getValue();
			CX3DNode *node = CX3DNodeFactory::createNode(vrmlNode);
			if (node)
			{
				node->print(indent+1);
				delete node;
			}
			else
			{
				CX3DParser::printIndent(indent);
				fprintf(fp, "NULL\n");
			}
		}
		break;

	case BOOLEAN_ARRAY_DATA:
		{
			CVRMLBooleanArrayData *boolArrayData = (CVRMLBooleanArrayData *)this;

			int n = boolArrayData->count();
			fprintf(fp, " (n=%d)\n", n);

			int nMax = CX3DParser::getMaxPrintElemsForMFField();
			bool bPartialPrint = false;
			if ((nMax > 0) && (n > nMax)) { n = nMax; bPartialPrint = true; }

			for (int i=0; i<n; i++)
			{
				CX3DParser::printIndent(indent);
				fprintf(fp, "[%d] %s\n", i, boolArrayData->getValue(i) ? "TRUE" : "FALSE");
			}

			if (bPartialPrint)
			{
				CX3DParser::printIndent(indent);
				fprintf(fp, "...\n");
			}
		}
		break;

	case INT_ARRAY_DATA:
		{
			CVRMLIntArrayData *intArrayData = (CVRMLIntArrayData *)this;

			int n = intArrayData->count();
			fprintf(fp, " (n=%d)\n", n);

			int nMax = CX3DParser::getMaxPrintElemsForMFField();
			bool bPartialPrint = false;
			if ((nMax > 0) && (n > nMax)) { n = nMax; bPartialPrint = true; }

			for (int i=0; i<n; i++)
			{
				CX3DParser::printIndent(indent);
				fprintf(fp, "[%d] %d\n", i, intArrayData->getValue(i));
			}

			if (bPartialPrint)
			{
				CX3DParser::printIndent(indent);
				fprintf(fp, "...\n");
			}
		}
		break;

	case FLOAT_ARRAY_DATA:
		{
			CVRMLFloatArrayData *floatArrayData = (CVRMLFloatArrayData *)this;

			int n = floatArrayData->count();
			fprintf(fp, " (n=%d)\n", n);

			int nMax = CX3DParser::getMaxPrintElemsForMFField();
			bool bPartialPrint = false;
			if ((nMax > 0) && (n > nMax)) { n = nMax; bPartialPrint = true; }

			for (int i=0; i<n; i++)
			{
				CX3DParser::printIndent(indent);
				fprintf(fp, "[%d] %f\n", i, floatArrayData->getValue(i));
			}

			if (bPartialPrint)
			{
				CX3DParser::printIndent(indent);
				fprintf(fp, "...\n");
			}
		}
		break;

	case DOUBLE_ARRAY_DATA:
		{
			CVRMLDoubleArrayData *doubleArrayData = (CVRMLDoubleArrayData *)this;

			int n = doubleArrayData->count();
			fprintf(fp, " (n=%d)\n", n);

			int nMax = CX3DParser::getMaxPrintElemsForMFField();
			bool bPartialPrint = false;
			if ((nMax > 0) && (n > nMax)) { n = nMax; bPartialPrint = true; }

			for (int i=0; i<n; i++)
			{
				CX3DParser::printIndent(indent);
				fprintf(fp, "[%d] %f\n", i, doubleArrayData->getValue(i));
			}

			if (bPartialPrint)
			{
				CX3DParser::printIndent(indent);
				fprintf(fp, "...\n");
			}
		}
		break;

	case STRING_ARRAY_DATA:
		{
			CVRMLStringArrayData *stringArrayData = (CVRMLStringArrayData *)this;

			int n = stringArrayData->count();
			fprintf(fp, " (n=%d)\n", n);

			int nMax = CX3DParser::getMaxPrintElemsForMFField();
			bool bPartialPrint = false;
			if ((nMax > 0) && (n > nMax)) { n = nMax; bPartialPrint = true; }

			for (int i=0; i<n; i++)
			{
				CX3DParser::printIndent(indent);
				fprintf(fp, "[%d] %s\n", i, stringArrayData->getValue(i));
			}

			if (bPartialPrint)
			{
				CX3DParser::printIndent(indent);
				fprintf(fp, "...\n");
			}
		}
		break;

	case NODE_ARRAY_DATA:
		{
			CVRMLNodeArrayData *nodeArrayData = (CVRMLNodeArrayData *)this;

			int n = nodeArrayData->count();
			fprintf(fp, " (n=%d)\n", n);
			
			for (int i=0; i<nodeArrayData->count(); i++)
			{
				jobject vrmlNode = nodeArrayData->getValue(i);
				CX3DNode *node = CX3DNodeFactory::createNode(vrmlNode);
				if (node)
				{
					node->print(indent+1);
					delete node;
				}
				else
				{
					CX3DParser::printIndent(indent+1);
					fprintf(fp, "NULL\n");
				}
			}
		}
		break;
	}
}


////////////////////////////////////////////////////////
//
// CVRMLFieldDataFactory

CVRMLFieldDataFactory *g_CVRMLFieldDataFactory = NULL;

void CVRMLFieldDataFactory::init()
{
	if (!g_CVRMLFieldDataFactory)
	{
		g_CVRMLFieldDataFactory = new CVRMLFieldDataFactory();
	}
}

void CVRMLFieldDataFactory::destroy()
{
	if (g_CVRMLFieldDataFactory)
	{
		delete g_CVRMLFieldDataFactory;
		g_CVRMLFieldDataFactory = NULL;
	}
}

CVRMLFieldDataFactory *CVRMLFieldDataFactory::getFactory()
{
	return g_CVRMLFieldDataFactory;
}

CVRMLFieldDataFactory::CVRMLFieldDataFactory()
{
	CJNIUtil *ju = CJNIUtil::getUtil();

	if (!ju)
	{
		fprintf(stderr, "CJNIUtil not initialized [%s:%d]\n", __FILE__, __LINE__);
		exit(1);
	}

	JNIEnv *env = ju->env();

	// ************************************************************
	// ************************************************************
	//jclass c = env->FindClass("org.web3d.vrml.nodes.VRMLFieldData");

	jclass c = env->FindClass("org/web3d/vrml/nodes/VRMLFieldData");
	if (!c)
	{
		fprintf(stderr, "cannot find VRMLFieldData class\n");
		exit(1);
	}

	m_fid_dataType = env->GetFieldID(c, "dataType", "S");

	m_fid_booleanValue = env->GetFieldID(c, "booleanValue", "Z");	// "Z" = boolean
	m_fid_intValue = env->GetFieldID(c, "intValue", "I");
	m_fid_longValue = env->GetFieldID(c, "longValue", "J");	// "J" = long
	m_fid_floatValue = env->GetFieldID(c, "floatValue", "F");
	m_fid_doubleValue = env->GetFieldID(c, "doubleValue", "D");
	m_fid_stringValue = env->GetFieldID(c, "stringValue", "Ljava/lang/String;");
	m_fid_nodeValue = env->GetFieldID(c, "nodeValue", "Lorg/web3d/vrml/lang/VRMLNode;");

	m_fid_booleanArrayValue = env->GetFieldID(c, "booleanArrayValue", "[Z");
	m_fid_intArrayValue = env->GetFieldID(c, "intArrayValue", "[I");
	m_fid_longArrayValue = env->GetFieldID(c, "longArrayValue", "[J");
	m_fid_floatArrayValue = env->GetFieldID(c, "floatArrayValue", "[F");
	m_fid_doubleArrayValue = env->GetFieldID(c, "doubleArrayValue", "[D");
	m_fid_stringArrayValue = env->GetFieldID(c, "stringArrayValue", "[Ljava/lang/String;");
	m_fid_nodeArrayValue = env->GetFieldID(c, "nodeArrayValue", "[Lorg/web3d/vrml/lang/VRMLNode;");
}

CVRMLFieldData *CVRMLFieldDataFactory::createField(jobject vrmlFieldData)
{
	CVRMLFieldData *data = NULL;

	CJNIUtil *ju = CJNIUtil::getUtil();
	JNIEnv *env = ju->env();

	short dataType = env->GetShortField(vrmlFieldData, m_fid_dataType);

	switch (dataType)
	{
	case BOOLEAN_DATA:
		{
			jboolean value = env->GetBooleanField(vrmlFieldData, m_fid_booleanValue);
			data = new CVRMLBooleanData((value == JNI_TRUE) ? true : false);
		}
		break;

	case INT_DATA:
		{
			jint value = env->GetIntField(vrmlFieldData, m_fid_intValue);
			data = new CVRMLIntData((int)value);
		}
		break;

	case LONG_DATA:
		{
			jlong value = env->GetLongField(vrmlFieldData, m_fid_longValue);
			data = new CVRMLLongData(value);
		}
		break;

	case FLOAT_DATA:
		{
			jfloat value = env->GetFloatField(vrmlFieldData, m_fid_floatValue);
			data = new CVRMLFloatData(value);
		}
		break;

	case DOUBLE_DATA:
		{
			jdouble value = env->GetDoubleField(vrmlFieldData, m_fid_doubleValue);
			data = new CVRMLDoubleData(value);
		}
		break;

	case STRING_DATA:
		{
			jobject value = env->GetObjectField(vrmlFieldData, m_fid_stringValue);

			jclass stringClass = env->FindClass("java/lang/String");
			if (!stringClass)
			{
				fprintf(stderr, "cannot find String Class\n");
				exit(1);
			}

#if 1
			// fix(2009/2/23)
			if (value)
			{
				if (env->IsInstanceOf(value, stringClass))
				{
					jstring str = (jstring)value;

					jboolean isCopy;
					const char *utf8Chars = env->GetStringUTFChars(str, &isCopy);

					data = new CVRMLStringData(utf8Chars);

					if (isCopy == JNI_TRUE)
					{
						env->ReleaseStringUTFChars(str, utf8Chars);
					}
				}
			}
			else
			{
				data = new CVRMLStringData("");
			}
#else
			if (env->IsInstanceOf(value, stringClass))
			{
				jstring str = (jstring)value;

				jboolean isCopy;
				const char *utf8Chars = env->GetStringUTFChars(str, &isCopy);

				data = new CVRMLStringData(utf8Chars);

				if (isCopy == JNI_TRUE)
				{
					env->ReleaseStringUTFChars(str, utf8Chars);
				}
			}
#endif
		}
		break;

	case NODE_DATA:
		{
			jobject value = env->GetObjectField(vrmlFieldData, m_fid_nodeValue);

			data = new CVRMLNodeData(value);
		}
		break;

	case BOOLEAN_ARRAY_DATA:
		{
			jbooleanArray values = (jbooleanArray)(env->GetObjectField(vrmlFieldData, m_fid_booleanArrayValue));
			if (values)
			{
				jint len = env->GetArrayLength(values);
				jboolean isCopy;
				jboolean *pValues = env->GetBooleanArrayElements(values, &isCopy);

				if (pValues)
				{
					CVRMLBooleanArrayData *tmpData = new CVRMLBooleanArrayData();

					for (int i=0; i<len; i++)
					{
						tmpData->addValue((pValues[i] == JNI_TRUE) ? true : false);
					}

					data = (CVRMLFieldData *)tmpData;

					if (isCopy == JNI_TRUE)
					{
						env->ReleaseBooleanArrayElements(values, pValues, 0);
					}
				}
			}
		}
		break;

	case INT_ARRAY_DATA:
		{
			jintArray values = (jintArray)(env->GetObjectField(vrmlFieldData, m_fid_intArrayValue));
			if (values)
			{
				jint len = env->GetArrayLength(values);
				jboolean isCopy;
				jint *pValues = env->GetIntArrayElements(values, &isCopy);

				if (pValues)
				{
					CVRMLIntArrayData *tmpData = new CVRMLIntArrayData();

					for (int i=0; i<len; i++)
					{
						tmpData->addValue(pValues[i]);
					}

					data = (CVRMLFieldData *)tmpData;

					if (isCopy == JNI_TRUE)
					{
						env->ReleaseIntArrayElements(values, pValues, 0);
					}
				}
			}
		}
		break;

	case LONG_ARRAY_DATA:
		{
			jlongArray values = (jlongArray)(env->GetObjectField(vrmlFieldData, m_fid_longArrayValue));
			if (values)
			{
				jint len = env->GetArrayLength(values);
				jboolean isCopy;
				jlong *pValues = env->GetLongArrayElements(values, &isCopy);

				if (pValues)
				{
					CVRMLLongArrayData *tmpData = new CVRMLLongArrayData();

					for (int i=0; i<len; i++)
					{
						tmpData->addValue(pValues[i]);
					}

					data = (CVRMLFieldData *)tmpData;

					if (isCopy == JNI_TRUE)
					{
						env->ReleaseLongArrayElements(values, pValues, 0);
					}
				}
			}
		}
		break;

	case FLOAT_ARRAY_DATA:
		{
			jfloatArray values = (jfloatArray)(env->GetObjectField(vrmlFieldData, m_fid_floatArrayValue));
			if (values)
			{
				jint len = env->GetArrayLength(values);
				jboolean isCopy;
				jfloat *pValues = env->GetFloatArrayElements(values, &isCopy);

				if (pValues)
				{
					CVRMLFloatArrayData *tmpData = new CVRMLFloatArrayData();

					for (int i=0; i<len; i++)
					{
						tmpData->addValue(pValues[i]);
					}

					data = (CVRMLFieldData *)tmpData;

					if (isCopy == JNI_TRUE)
					{
						env->ReleaseFloatArrayElements(values, pValues, 0);
					}
				}
			}
		}
		break;

	case DOUBLE_ARRAY_DATA:
		{
			jdoubleArray values = (jdoubleArray)(env->GetObjectField(vrmlFieldData, m_fid_doubleArrayValue));
			if (values)
			{
				jint len = env->GetArrayLength(values);
				jboolean isCopy;
				jdouble *pValues = env->GetDoubleArrayElements(values, &isCopy);

				if (pValues)
				{
					CVRMLDoubleArrayData *tmpData = new CVRMLDoubleArrayData();

					for (int i=0; i<len; i++)
					{
						tmpData->addValue(pValues[i]);
					}

					data = (CVRMLFieldData *)tmpData;

					if (isCopy == JNI_TRUE)
					{
						env->ReleaseDoubleArrayElements(values, pValues, 0);
					}
				}
			}
		}
		break;

	case STRING_ARRAY_DATA:
		{
			jobjectArray values = (jobjectArray)(env->GetObjectField(vrmlFieldData, m_fid_stringArrayValue));
			if (values)
			{
				CVRMLStringArrayData *tmpData = new CVRMLStringArrayData();

				jint len = env->GetArrayLength(values);
				for (int i=0; i<len; i++)
				{
					jstring jstr = (jstring)(env->GetObjectArrayElement(values, i));
					jboolean isCopy;
					const char *utf8Chars = env->GetStringUTFChars(jstr, &isCopy);
					tmpData->addValue(utf8Chars);

					if (isCopy == JNI_TRUE)
					{
						env->ReleaseStringUTFChars(jstr, utf8Chars);
					}
					env->DeleteLocalRef(jstr);
				}

				data = (CVRMLFieldData *)tmpData;
			}
		}
		break;

	case NODE_ARRAY_DATA:
		{
			jobjectArray values = (jobjectArray)(env->GetObjectField(vrmlFieldData, m_fid_nodeArrayValue));
			if (values)
			{
				CVRMLNodeArrayData *tmpData = new CVRMLNodeArrayData();

				jint len = env->GetArrayLength(values);
				for (int i=0; i<len; i++)
				{
					jobject vrmlNode = env->GetObjectArrayElement(values, i);
					if (vrmlNode)
					{
						tmpData->addValue(vrmlNode);
					}
					env->DeleteLocalRef(vrmlNode);
				}

				data = (CVRMLFieldData *)tmpData;
			}
		}
		break;

	default:
		break;
	}

	return data;
}

