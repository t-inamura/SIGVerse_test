#ifndef _CX3DFIELD_H_
#define _CX3DFIELD_H_

//#include "CX3DNode.h"
class CX3DNode;

//#include "CVRMLFieldData.h"
class CVRMLFieldData;

#include <vector>
#include <string>

/////////////////////////////////////////////////////////////
//
//	Field ID

enum CX3DFieldType
{
	SFBOOL = 1,
	MFBOOL = 2,
	SFINT32 = 3,
	MFINT32 = 4,
	SFFLOAT = 5,
	MFFLOAT = 6,
	SFDOUBLE = 7,
	MFDOUBLE = 8,
	SFTIME = 9,
	MFTIME = 10,
	SFNODE = 11,
	MFNODE= 12,
	SFVEC2F = 13,
	MFVEC2F = 14,
	SFVEC3F = 15,
	MFVEC3F = 16,
	SFVEC3D = 17,
	MFVEC3D = 18,
	SFROTATION = 19,
	MFROTATION = 20,
	SFCOLOR = 21,
	MFCOLOR = 22,
	SFCOLORRGBA = 23,
	MFCOLORRGBA = 24,
	SFIMAGE = 25,
	MFIMAGE = 26,
	SFSTRING = 27,
	MFSTRING = 28,
	SFVEC2D = 29,
	MFVEC2D = 30,
	SFLONG = 31,
	MFLONG = 32,
	SFVEC4F = 33,
	MFVEC4F = 34,
	SFVEC4D = 35,
	MFVEC4D = 36,
	SFMATRIX3F = 37,
	MFMATRIX3F = 38,
	SFMATRIX4F = 39,
	MFMATRIX4F = 40,
	SFMATRIX3D = 41,
	MFMATRIX3D = 42,
	SFMATRIX4D = 43,
	MFMATRIX4D = 44
};

/////////////////////////////////////////////////////////////
//
//	Base class

class CX3DField
{
public:
	CX3DField() {}
	virtual ~CX3DField() {};
	static char *getFieldTypeName(CX3DFieldType ftype);
};


/////////////////////////////////////////////////////////////
//
//	SFxxx

class SFBool : public CX3DField
{
public:
	SFBool() : m_value(false) {}
	void setValue(CVRMLFieldData *data);
	bool getValue() { return m_value; }

private:
	bool m_value;
};

class SFInt32 : public CX3DField
{
public:
	SFInt32() : m_value(0) {}
	void setValue(CVRMLFieldData *data);
	int getValue() { return m_value; }

private:
	int m_value;
};

class SFFloat : public CX3DField
{
public:
	SFFloat() : m_value(0.0f) {}
	void setValue(CVRMLFieldData *data);
	float getValue() { return m_value; }

private:
	float m_value;
};

class SFDouble : public CX3DField
{
public:
	SFDouble() : m_value(0.0) {}
	void setValue(CVRMLFieldData *data);
	double getValue() { return m_value; }

private:
	double m_value;
};

class SFTime : public CX3DField
{
public:
	SFTime() : m_value(0.0) {}
	void setValue(CVRMLFieldData *data);
	double getValue() { return m_value; }

private:
	double m_value;
};

class SFVec2f : public CX3DField
{
public:
	SFVec2f() {
		m_values[0] = 0.0f;
		m_values[1] = 0.0f;
	}

	void setValue(float x, float y) {
		m_values[0] = x;
		m_values[1] = y;
	}
	void setValue(CVRMLFieldData *data);

	float x() { return m_values[0]; }
	float y() { return m_values[1]; }
	void getValue(float &x, float &y) {
		x = m_values[0];
		y = m_values[1];
	}
	void getValue(float *f) {
		f[0] = m_values[0];
		f[1] = m_values[1];
	}

private:
	float m_values[2];
};

class SFVec3f : public CX3DField
{
public:
	SFVec3f() {
		m_values[0] = 0.0f;
		m_values[1] = 0.0f;
		m_values[2] = 0.0f;
	}

	void setValue(float x, float y, float z) {
		m_values[0] = x;
		m_values[1] = y;
		m_values[2] = z;
	}
	void setValue(CVRMLFieldData *data);

	float x() { return m_values[0]; }
	float y() { return m_values[1]; }
	float z() { return m_values[2]; }
	void getValue(float &x, float &y, float &z) {
		x = m_values[0];
		y = m_values[1];
		z = m_values[2];
	}
	void getValue(float *f) {
		f[0] = m_values[0];
		f[1] = m_values[1];
		f[2] = m_values[2];
	}
	float m_values[3];
private:

};

class SFVec4f : public CX3DField
{
public:
	SFVec4f() {
		m_values[0] = 0.0f;
		m_values[1] = 0.0f;
		m_values[2] = 0.0f;
		m_values[3] = 0.0f;
	}

	void setValue(float x, float y, float z, float w) {
		m_values[0] = x;
		m_values[1] = y;
		m_values[2] = z;
		m_values[3] = w;
	}
	void setValue(CVRMLFieldData *data);

	float x() { return m_values[0]; }
	float y() { return m_values[1]; }
	float z() { return m_values[2]; }
	float w() { return m_values[3]; }
	void getValue(float &x, float &y, float &z, float &w) {
		x = m_values[0];
		y = m_values[1];
		z = m_values[2];
		w = m_values[3];
	}
	void getValue(float *f) {
		f[0] = m_values[0];
		f[1] = m_values[1];
		f[2] = m_values[2];
		f[3] = m_values[3];
	}

private:
	float m_values[4];
};

class SFVec2d : public CX3DField
{
public:
	SFVec2d() {
		m_values[0] = 0.0;
		m_values[1] = 0.0;
	}

	void setValue(double x, double y) {
		m_values[0] = x;
		m_values[1] = y;
	}
	void setValue(CVRMLFieldData *data);

	double x() { return m_values[0]; }
	double y() { return m_values[1]; }
	void getValue(double &x, double &y) {
		x = m_values[0];
		y = m_values[1];
	}
	void getValue(double *f) {
		f[0] = m_values[0];
		f[1] = m_values[1];
	}

private:
	double m_values[2];
};

class SFVec3d : public CX3DField
{
public:
	SFVec3d() {
		m_values[0] = 0.0;
		m_values[1] = 0.0;
		m_values[2] = 0.0;
	}

	void setValue(double x, double y, double z) {
		m_values[0] = x;
		m_values[1] = y;
		m_values[2] = z;
	}
	void setValue(CVRMLFieldData *data);

	double x() { return m_values[0]; }
	double y() { return m_values[1]; }
	double z() { return m_values[2]; }
	void getValue(double &x, double &y, double &z) {
		x = m_values[0];
		y = m_values[1];
		z = m_values[2];
	}
	void getValue(double *f) {
		f[0] = m_values[0];
		f[1] = m_values[1];
		f[2] = m_values[2];
	}

private:
	double m_values[3];
};

class SFVec4d : public CX3DField
{
public:
	SFVec4d() {
		m_values[0] = 0.0;
		m_values[1] = 0.0;
		m_values[2] = 0.0;
		m_values[3] = 0.0;
	}

	void setValue(double x, double y, double z, double w) {
		m_values[0] = x;
		m_values[1] = y;
		m_values[2] = z;
		m_values[3] = w;
	}
	void setValue(CVRMLFieldData *data);

	double x() { return m_values[0]; }
	double y() { return m_values[1]; }
	double z() { return m_values[2]; }
	double w() { return m_values[3]; }
	void getValue(double &x, double &y, double &z, double &w) {
		x = m_values[0];
		y = m_values[1];
		z = m_values[2];
		w = m_values[3];
	}
	void getValue(double *f) {
		f[0] = m_values[0];
		f[1] = m_values[1];
		f[2] = m_values[2];
		f[3] = m_values[3];
	}

private:
	double m_values[4];
};

class SFRotation : public CX3DField
{
public:
	SFRotation() {
		m_values[0] = 0.0f;
		m_values[1] = 0.0f;
		m_values[2] = 0.0f;
		m_values[3] = 0.0f;
	}

	void setValue(CVRMLFieldData *data);

	float x() { return m_values[0]; }
	float y() { return m_values[1]; }
	float z() { return m_values[2]; }
	float rot() { return m_values[3]; }
	void getValue(float &x, float &y, float &z, float &rot) {
		x = m_values[0];
		y = m_values[1];
		z = m_values[2];
		rot = m_values[3];
	}
	void getValue(float *f) {
		f[0] = m_values[0];
		f[1] = m_values[1];
		f[2] = m_values[2];
		f[3] = m_values[3];
	}

private:
	float m_values[4];
};

class SFColor : public CX3DField
{
public:
	SFColor() {
		m_values[0] = 0.0f;
		m_values[1] = 0.0f;
		m_values[2] = 0.0f;
	}

	void setValue(float r, float g, float b) {
		m_values[0] = r;
		m_values[1] = g;
		m_values[2] = b;
	}
	void setValue(CVRMLFieldData *data);

	float r() { return m_values[0]; }
	float g() { return m_values[1]; }
	float b() { return m_values[2]; }
	void getValue(float &r, float &g, float &b) {
		r = m_values[0];
		g = m_values[1];
		b = m_values[2];
	}
	void getValue(float *f) {
		f[0] = m_values[0];
		f[1] = m_values[1];
		f[2] = m_values[2];
	}

private:
	float m_values[3];
};

class SFString : public CX3DField
{
public:
	SFString() : m_value("") {}

	void setValue(const char *s) {
		m_value = s ? s : "";
	}
	void setValue(CVRMLFieldData *data);

	const char *getValue() { return m_value.c_str(); }

private:
	std::string m_value;
};

class SFNode : public CX3DField
{
public:
	SFNode() : m_pNode(NULL) {}
	~SFNode() {
		if (m_pNode)
		{
			delete m_pNode;
			m_pNode = NULL;
		}
	}
	SFNode(CX3DNode *node) : m_pNode(node) {}
	void setValue(CVRMLFieldData *data);
	CX3DNode *getNode() { return m_pNode; }
	CX3DNode *releaseNode()
	{
		CX3DNode *p = m_pNode;
		m_pNode = NULL;
		return p;
	}

private:
	CX3DNode *m_pNode;
};


/////////////////////////////////////////////////////////////
//
//	MFxxx

class MFBool : public CX3DField
{
public:
	MFBool() { m_values.clear(); }

	void setValue(CVRMLFieldData *data);

	int count() { return (int)m_values.size(); }
	bool getValue(int i) { return m_values[i]; }

private:
	std::vector<bool> m_values;
};

class MFInt32 : public CX3DField
{
public:
	MFInt32() { m_values.clear(); }

	void setValue(CVRMLFieldData *data);

	int count() { return (int)m_values.size(); }
	int getValue(int i) { return m_values[i]; }

private:
	std::vector<int> m_values;
};

class MFFloat : public CX3DField
{
public:
	MFFloat() { m_values.clear(); }

	void setValue(CVRMLFieldData *data);

	int count() { return (int)m_values.size(); }
	float getValue(int i) { return m_values[i]; }

private:
	std::vector<float> m_values;
};

class MFDouble : public CX3DField
{
public:
	MFDouble() { m_values.clear(); }

	void setValue(CVRMLFieldData *data);

	int count() { return (int)m_values.size(); }
	double getValue(int i) { return m_values[i]; }

private:
	std::vector<double> m_values;
};

class MFVec2f : public CX3DField
{
public:
	MFVec2f() { m_values.clear(); }

	void setValue(CVRMLFieldData *data);

	int count() { return (int)m_values.size(); }
	SFVec2f getValue(int i) { return m_values[i]; }

private:
	std::vector<SFVec2f> m_values;
};

class MFVec3f : public CX3DField
{
public:
	MFVec3f() { m_values.clear(); }

	void setValue(CVRMLFieldData *data);

	int count() { return (int)m_values.size(); }
	SFVec3f getValue(int i) { return m_values[i]; }

private:
	std::vector<SFVec3f> m_values;
};

class MFVec4f : public CX3DField
{
public:
	MFVec4f() { m_values.clear(); }

	void setValue(CVRMLFieldData *data);

	int count() { return (int)m_values.size(); }
	SFVec4f getValue(int i) { return m_values[i]; }

private:
	std::vector<SFVec4f> m_values;
};

class MFVec2d : public CX3DField
{
public:
	MFVec2d() { m_values.clear(); }

	void setValue(CVRMLFieldData *data);

	int count() { return (int)m_values.size(); }
	SFVec2d getValue(int i) { return m_values[i]; }

private:
	std::vector<SFVec2d> m_values;
};

class MFVec3d : public CX3DField
{
public:
	MFVec3d() { m_values.clear(); }

	void setValue(CVRMLFieldData *data);

	int count() { return (int)m_values.size(); }
	SFVec3d getValue(int i) { return m_values[i]; }

private:
	std::vector<SFVec3d> m_values;
};

class MFVec4d : public CX3DField
{
public:
	MFVec4d() { m_values.clear(); }

	void setValue(CVRMLFieldData *data);

	int count() { return (int)m_values.size(); }
	SFVec4d getValue(int i) { return m_values[i]; }

private:
	std::vector<SFVec4d> m_values;
};

class MFColor : public CX3DField
{
public:
	MFColor() { m_values.clear(); }

	void setValue(CVRMLFieldData *data);

	int count() { return (int)m_values.size(); }
	SFColor getValue(int i) { return m_values[i]; }

private:
	std::vector<SFColor> m_values;
};

class MFString : public CX3DField
{
public:
	MFString() { m_values.clear(); }

	void setValue(CVRMLFieldData *data);

	int count() { return (int)m_values.size(); }
	const char *getValue(int i) { return m_values[i].c_str(); }

private:
	std::vector<std::string> m_values;
};

class MFNode : public CX3DField
{
public:
	MFNode() { m_values.clear(); }
	~MFNode() { free(); }
	void setValue(CVRMLFieldData *data);
	void addValue(CX3DNode *p) { m_values.push_back(p); }

	int count() { return (int)m_values.size(); }

	CX3DNode *getNode(int i)
	{
		// FIX20110324
		return (i>=0 && i<count()) ? m_values[i] : NULL;
	}

	CX3DNode *releaseNode(int i)
	{
		// FIX20110324
		if (i>=0 && i<count())
		{
			CX3DNode *p = m_values[i];
			m_values[i] = NULL;
			return p;
		}
		else
		{
			return NULL;
		}
	}

private:
	void free()
	{
		std::vector<CX3DNode *>::iterator i;
		for (i=m_values.begin(); i!=m_values.end(); i++)
		{
			CX3DNode *p = *i;
			if (p) delete p;
		}

		m_values.clear();
	}

	std::vector<CX3DNode *> m_values;
};

#endif