#ifndef _CVRMLFIELDDATA_H_
#define _CVRMLFIELDDATA_H_

#include "CX3DNode.h"
#include "CJNIUtil.h"
#include <vector>
#include <jni.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef WIN32
typedef __int64		_CVRML_LONG_;
#else
typedef long long	_CVRML_LONG_;
#endif

enum CVRMLFieldDataType
{
	UNKNOWN,
	BOOLEAN_DATA = 1,
	INT_DATA = 2,
	LONG_DATA = 3,
	FLOAT_DATA = 4,
	DOUBLE_DATA = 5,

	STRING_DATA = 6,	
	NODE_DATA = 7,	

	BOOLEAN_ARRAY_DATA = 8,
	INT_ARRAY_DATA = 9,
	LONG_ARRAY_DATA = 10,
	FLOAT_ARRAY_DATA = 11,
	DOUBLE_ARRAY_DATA = 12,

	NODE_ARRAY_DATA = 13,
	STRING_ARRAY_DATA = 14
};

///////////////////////////////////////////////////////////
//
//	base class

class CVRMLFieldData
{
public:
	virtual ~CVRMLFieldData() {}
	virtual CVRMLFieldDataType getFieldType() = 0;
	bool isNodeField() { return (getFieldType() == NODE_DATA) ? true : false; }
	bool isNodeArrayField() { return (getFieldType() == NODE_ARRAY_DATA) ? true : false; }
	static char *getFieldTypeName(CVRMLFieldDataType ftype);
	virtual int count() = 0;

	void print(int indent=0);
};

///////////////////////////////////////////////////////////
//
//	concrete classes

// --------------------------------------------------------
//  Single data types
// --------------------------------------------------------
class CVRMLBooleanData : public CVRMLFieldData
{
public:
	CVRMLBooleanData() : m_value(false) {}
	CVRMLBooleanData(bool value) : m_value(value){}
	CVRMLFieldDataType getFieldType() { return BOOLEAN_DATA; }
	int count() { return 1; }
	bool getValue() { return m_value; }

private:
	bool m_value;
};

class CVRMLIntData : public CVRMLFieldData
{
public:
	CVRMLIntData() : m_value(0) {}
	CVRMLIntData(int value) : m_value(value) {}
	CVRMLFieldDataType getFieldType() { return INT_DATA; }
	int count() { return 1; }
	int getValue() { return m_value; }

private:
	int m_value;
};

class CVRMLLongData : public CVRMLFieldData
{
public:
	CVRMLLongData() : m_value(0) {}
	CVRMLLongData(_CVRML_LONG_ value) : m_value(value) {}
	CVRMLFieldDataType getFieldType() { return LONG_DATA; }
	int count() { return 1; }
	_CVRML_LONG_ getValue() { return m_value; }

private:
	_CVRML_LONG_ m_value;
};

class CVRMLFloatData : public CVRMLFieldData
{
public:
	CVRMLFloatData() : m_value(0.0f) {}
	CVRMLFloatData(float value) : m_value(value) {}
	CVRMLFieldDataType getFieldType() { return FLOAT_DATA; }
	int count() { return 1; }
	float getValue() { return m_value; }

private:
	float m_value;
};

class CVRMLDoubleData : public CVRMLFieldData
{
public:
	CVRMLDoubleData() : m_value(0.0) {}
	CVRMLDoubleData(double value) : m_value(value) {}
	CVRMLFieldDataType getFieldType() { return DOUBLE_DATA; }
	int count() { return 1; }
	double getValue() { return m_value; }

private:
	double m_value;
};

class CVRMLStringData : public CVRMLFieldData
{
public:
	CVRMLStringData() : m_value("") {}
	CVRMLStringData(const char *value) { 
		if (value) m_value = value;
		else m_value = "";
	}
	CVRMLFieldDataType getFieldType() { return STRING_DATA; }
	int count() { return 1; }
	const char *getValue() { return m_value.c_str(); }

private:
	std::string m_value;
};

class CVRMLNodeData : public CVRMLFieldData
{
public:
	CVRMLNodeData() : m_vrmlNode(NULL) {}
	CVRMLNodeData(jobject vrmlNode) {
		CJNIUtil *ju = CJNIUtil::getUtil();
		JNIEnv *env = ju->env();

		if (vrmlNode)
		{
			if (!ju->isInstanceOfVRMLNode(vrmlNode))
			{
				fprintf(stderr, "vrmlNode is not instance of VRMLNode [%s:%d]\n", __FILE__, __LINE__);
				exit(1);
			}
			
			m_vrmlNode = env->NewGlobalRef(vrmlNode);
		}
		else
		{
	//		fprintf(stderr, "vrmlNode is NULL [%s:%d]\n", __FILE__, __LINE__);
			m_vrmlNode = NULL;
		}
	}

	virtual ~CVRMLNodeData() {
		JNIEnv *env = CJNIUtil::getEnv();

		if (m_vrmlNode)
		{
			env->DeleteGlobalRef(m_vrmlNode);
			m_vrmlNode = NULL;
		}
	}

	CVRMLFieldDataType getFieldType() { return NODE_DATA; }
	int count() { return 1; }

	jobject getValue() { return m_vrmlNode; }

private:
	jobject m_vrmlNode;
};


// --------------------------------------------------------
//  Array data types
// --------------------------------------------------------
class CVRMLBooleanArrayData : public CVRMLFieldData
{
public:
	CVRMLBooleanArrayData() { m_values.clear(); }
	CVRMLFieldDataType getFieldType() { return BOOLEAN_ARRAY_DATA; }
	void addValue(bool value) { m_values.push_back(value); }
	int count() { return (int)m_values.size(); }
	bool getValue(int i) { return m_values[i]; }

private:
	std::vector<bool> m_values;
};

class CVRMLIntArrayData : public CVRMLFieldData
{
public:
	CVRMLIntArrayData() { m_values.clear(); }
	CVRMLFieldDataType getFieldType() { return INT_ARRAY_DATA; }
	void addValue(int value) { m_values.push_back(value); }
	int count() { return (int)m_values.size(); }
	int getValue(int i) { return m_values[i]; }

private:
	std::vector<int> m_values;
};

class CVRMLLongArrayData : public CVRMLFieldData
{
public:
	CVRMLLongArrayData() { m_values.clear(); }
	CVRMLFieldDataType getFieldType() { return LONG_ARRAY_DATA; }
	void addValue(_CVRML_LONG_ value) { m_values.push_back(value); }
	int count() { return (int)m_values.size(); }
	_CVRML_LONG_ getValue(int i) { return m_values[i]; }

private:
	std::vector<_CVRML_LONG_> m_values;
};

class CVRMLFloatArrayData : public CVRMLFieldData
{
public:
	CVRMLFloatArrayData() { m_values.clear(); }
	CVRMLFieldDataType getFieldType() { return FLOAT_ARRAY_DATA; }
	void addValue(float value) { m_values.push_back(value); }
	int count() { return (int)m_values.size(); }
	float getValue(int i) { return m_values[i]; }

private:
	std::vector<float> m_values;
};

class CVRMLDoubleArrayData : public CVRMLFieldData
{
public:
	CVRMLDoubleArrayData() { m_values.clear(); }
	CVRMLFieldDataType getFieldType() { return DOUBLE_ARRAY_DATA; }
	void addValue(double value) { m_values.push_back(value); }
	int count() { return (int)m_values.size(); }
	double getValue(int i) { return m_values[i]; }

private:
	std::vector<double> m_values;
};

class CVRMLStringArrayData : public CVRMLFieldData
{
public:
	CVRMLStringArrayData() { m_values.clear(); }
	CVRMLFieldDataType getFieldType() { return STRING_ARRAY_DATA; }
	void addValue(const char *value) { m_values.push_back(value); }
	int count() { return (int)m_values.size(); }
	const char *getValue(int i) { return m_values[i].c_str(); }

private:
	std::vector<std::string> m_values;
};

class CVRMLNodeArrayData : public CVRMLFieldData
{
public:
	CVRMLNodeArrayData() { m_values.clear(); }
	~CVRMLNodeArrayData() {
		JNIEnv *env = CJNIUtil::getEnv();

		std::vector<jobject>::iterator i;
		for (i=m_values.begin(); i!=m_values.end(); i++)
		{
			jobject vrmlNode = *i;
			if (vrmlNode)
			{
				env->DeleteGlobalRef(vrmlNode);
			}
		}
		m_values.clear();
	}

	CVRMLFieldDataType getFieldType() { return NODE_ARRAY_DATA; }

	void addValue(jobject vrmlNode) {
		CJNIUtil *ju = CJNIUtil::getUtil();
		JNIEnv *env = ju->env();

		if (vrmlNode)
		{
			if (!ju->isInstanceOfVRMLNode(vrmlNode))
			{
				fprintf(stderr, "vrmlNode is not instance of VRMLNode [%s:%d]\n", __FILE__, __LINE__);
				exit(1);
			}
			
			m_values.push_back(env->NewGlobalRef(vrmlNode));
		}
		else
		{
	//		fprintf(stderr, "vrmlNode is NULL [%s:%d]\n", __FILE__, __LINE__);
			m_values.push_back(NULL);
		}
	}

	int count() { return (int)m_values.size(); }
	jobject getValue(int i) { return m_values[i]; }

private:
	std::vector<jobject> m_values;
};


///////////////////////////////////////////////////////////
//
//	instance factory for CVRMLFieldData

class CVRMLFieldDataFactory
{
public:
	CVRMLFieldDataFactory();
	static void init();
	static void destroy();
	static CVRMLFieldDataFactory *getFactory();
	CVRMLFieldData *createField(jobject vrmlFieldData);

private:
	jfieldID m_fid_dataType;

	jfieldID m_fid_booleanValue;
	jfieldID m_fid_intValue;
	jfieldID m_fid_longValue;
	jfieldID m_fid_floatValue;
	jfieldID m_fid_doubleValue;
	jfieldID m_fid_stringValue;
	jfieldID m_fid_nodeValue;

	jfieldID m_fid_booleanArrayValue;
	jfieldID m_fid_intArrayValue;
	jfieldID m_fid_longArrayValue;
	jfieldID m_fid_floatArrayValue;
	jfieldID m_fid_doubleArrayValue;
	jfieldID m_fid_stringArrayValue;
	jfieldID m_fid_nodeArrayValue;
};

#endif

