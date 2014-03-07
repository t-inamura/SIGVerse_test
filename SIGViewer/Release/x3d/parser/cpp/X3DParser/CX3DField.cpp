#include "CX3DField.h"
#include "CX3DParser.h"
#include "CVRMLFieldData.h"

char *CX3DField::getFieldTypeName(CX3DFieldType ftype)
{
	switch (ftype)
	{
		case SFBOOL:
			return "SFBOOL";
			
		case MFBOOL:
			return "MFBOOL";
			
		case SFINT32:
			return "SFINT32";
			
		case MFINT32:
			return "MFINT32";
			
		case SFFLOAT:
			return "SFFLOAT";
			
		case MFFLOAT:
			return "MFFLOAT";
			
		case SFDOUBLE:
			return "SFDOUBLE";
			
		case MFDOUBLE:
			return "MFDOUBLE";
			
		case SFTIME:
			return "SFTIME";
			
		case MFTIME :
			return "MFTIME";
			
		case SFNODE :
			return "SFNODE";
			
		case MFNODE:
			return "MFNODE";
			
		case SFVEC2F :
			return "SFVEC2F";
			
		case MFVEC2F :
			return "MFVEC2F";
			
		case SFVEC3F :
			return "SFVEC3F";
			
		case MFVEC3F :
			return "MFVEC3F";
			
		case SFVEC3D :
			return "SFVEC3D";
			
		case MFVEC3D :
			return "MFVEC3D";
			
		case SFROTATION :
			return "SFROTATION";
			
		case MFROTATION :
			return "MFROTATION";
			
		case SFCOLOR :
			return "SFCOLOR";
			
		case MFCOLOR :
			return "MFCOLOR";
			
		case SFCOLORRGBA :
			return "SFCOLORRGBA";
			
		case MFCOLORRGBA :
			return "MFCOLORRGBA";
			
		case SFIMAGE :
			return "SFIMAGE";
			
		case MFIMAGE :
			return "MFIMAGE";
			
		case SFSTRING :
			return "SFSTRING";
			
		case MFSTRING :
			return "MFSTRING";
			
		case SFVEC2D :
			return "SFVEC2D";
			
		case MFVEC2D :
			return "MFVEC2D";
			
		case SFLONG :
			return "SFLONG";
			
		case MFLONG :
			return "MFLONG";
			
		case SFVEC4F :
			return "SFVEC4F";
			
		case MFVEC4F :
			return "MFVEC4F";
			
		case SFVEC4D :
			return "SFVEC4D";
			
		case MFVEC4D :
			return "MFVEC4D";
			
		case SFMATRIX3F :
			return "SFMATRIX3F";
			
		case MFMATRIX3F :
			return "MFMATRIX3F";
			
		case SFMATRIX4F :
			return "SFMATRIX4F";
			
		case MFMATRIX4F :
			return "MFMATRIX4F";
			
		case SFMATRIX3D :
			return "SFMATRIX3D";
			
		case MFMATRIX3D :
			return "MFMATRIX3D";
			
		case SFMATRIX4D :
			return "SFMATRIX4D";
			
		case MFMATRIX4D :
			return "MFMATRIX4D";
	}

	return "UNKNOWN";
}

void SFBool::setValue(CVRMLFieldData *data)
{
	if (!data) return;
	if (data->getFieldType() != BOOLEAN_DATA) return;

	m_value = ((CVRMLBooleanData *)data)->getValue();
	//(CVRMLBooleanData *)data)->count();
	CX3DParser::printLog("m_value = %d, count = %d\n",m_value, data->count());
}

void SFInt32::setValue(CVRMLFieldData *data)
{
	if (!data) return;
	if (data->getFieldType() != INT_DATA) return;

	m_value = ((CVRMLIntData *)data)->getValue();
}

void SFFloat::setValue(CVRMLFieldData *data)
{
	if (!data) return;
	if (data->getFieldType() != FLOAT_DATA) return;

	m_value = ((CVRMLFloatData *)data)->getValue();
}

void SFDouble::setValue(CVRMLFieldData *data)
{
	if (!data) return;
	if (data->getFieldType() != DOUBLE_DATA) return;

	m_value = ((CVRMLDoubleData *)data)->getValue();
}

void SFTime::setValue(CVRMLFieldData *data)
{
	if (!data) return;
	if (data->getFieldType() != DOUBLE_DATA) return;

	m_value = ((CVRMLDoubleData *)data)->getValue();
}

void SFVec2f::setValue(CVRMLFieldData *data)
{
	if (!data) return;
	if (data->getFieldType() != FLOAT_ARRAY_DATA) return;
	
	CVRMLFloatArrayData *a = (CVRMLFloatArrayData *)data;
	if (a->count() < 2) return;

	m_values[0] = a->getValue(0);
	m_values[1] = a->getValue(1);
}

void SFVec3f::setValue(CVRMLFieldData *data)
{
	if (!data) return;
	if (data->getFieldType() != FLOAT_ARRAY_DATA) return;
	
	CVRMLFloatArrayData *a = (CVRMLFloatArrayData *)data;
	if (a->count() < 3) return;

	m_values[0] = a->getValue(0);
	m_values[1] = a->getValue(1);
	m_values[2] = a->getValue(2);
}

void SFVec4f::setValue(CVRMLFieldData *data)
{
	if (!data) return;
	if (data->getFieldType() != FLOAT_ARRAY_DATA) return;
	
	CVRMLFloatArrayData *a = (CVRMLFloatArrayData *)data;
	if (a->count() < 4) return;

	m_values[0] = a->getValue(0);
	m_values[1] = a->getValue(1);
	m_values[2] = a->getValue(2);
	m_values[3] = a->getValue(3);
}

void SFVec2d::setValue(CVRMLFieldData *data)
{
	if (!data) return;
	if (data->getFieldType() != DOUBLE_ARRAY_DATA) return;
	
	CVRMLDoubleArrayData *a = (CVRMLDoubleArrayData *)data;
	if (a->count() < 2) return;

	m_values[0] = a->getValue(0);
	m_values[1] = a->getValue(1);
}

void SFVec3d::setValue(CVRMLFieldData *data)
{
	if (!data) return;
	if (data->getFieldType() != DOUBLE_ARRAY_DATA) return;
	
	CVRMLDoubleArrayData *a = (CVRMLDoubleArrayData *)data;
	if (a->count() < 3) return;

	m_values[0] = a->getValue(0);
	m_values[1] = a->getValue(1);
	m_values[2] = a->getValue(2);
}

void SFVec4d::setValue(CVRMLFieldData *data)
{
	if (!data) return;
	if (data->getFieldType() != DOUBLE_ARRAY_DATA) return;
	
	CVRMLDoubleArrayData *a = (CVRMLDoubleArrayData *)data;
	if (a->count() < 4) return;

	m_values[0] = a->getValue(0);
	m_values[1] = a->getValue(1);
	m_values[2] = a->getValue(2);
	m_values[3] = a->getValue(3);
}

void SFRotation::setValue(CVRMLFieldData *data)
{
	if (!data) return;
	if (data->getFieldType() != FLOAT_ARRAY_DATA) return;
	
	CVRMLFloatArrayData *a = (CVRMLFloatArrayData *)data;
	if (a->count() < 4) return;

	m_values[0] = a->getValue(0);
	m_values[1] = a->getValue(1);
	m_values[2] = a->getValue(2);
	m_values[3] = a->getValue(3);
}

void SFColor::setValue(CVRMLFieldData *data)
{
	if (!data) return;
	if (data->getFieldType() != FLOAT_ARRAY_DATA) return;
	
	CVRMLFloatArrayData *a = (CVRMLFloatArrayData *)data;
	if (a->count() < 3) return;

	m_values[0] = a->getValue(0);
	m_values[1] = a->getValue(1);
	m_values[2] = a->getValue(2);
}

void SFString::setValue(CVRMLFieldData *data)
{
	if (!data) return;
	if (data->getFieldType() != STRING_DATA) return;

	CVRMLStringData *str = (CVRMLStringData *)data;

	const char * p = str->getValue();
	m_value = p ? p : "";
}

void SFNode::setValue(CVRMLFieldData *data)
{
	if (!data) return;
	if (data->getFieldType() != NODE_DATA) return;

	CVRMLNodeData *p = (CVRMLNodeData *)data;

	if (m_pNode)
	{
		delete m_pNode;
		m_pNode = NULL;
	}

	m_pNode = CX3DNodeFactory::createNode(p->getValue());
}

void MFBool::setValue(CVRMLFieldData *data)
{
	if (!data) return;
	if (data->getFieldType() != BOOLEAN_ARRAY_DATA) return;

	CVRMLBooleanArrayData *a = (CVRMLBooleanArrayData *)data;

	m_values.clear();
	for (int i=0; i<a->count(); i++)
	{
		m_values.push_back(a->getValue(i));
	}
}

void MFInt32::setValue(CVRMLFieldData *data)
{
	if (!data) return;
	if (data->getFieldType() != INT_ARRAY_DATA) return;

	CVRMLIntArrayData *a = (CVRMLIntArrayData *)data;

	m_values.clear();
	for (int i=0; i<a->count(); i++)
	{
		m_values.push_back(a->getValue(i));
	}
}

void MFFloat::setValue(CVRMLFieldData *data)
{
	if (!data) return;
	if (data->getFieldType() != FLOAT_ARRAY_DATA) return;

	CVRMLFloatArrayData *a = (CVRMLFloatArrayData *)data;

	m_values.clear();
	for (int i=0; i<a->count(); i++)
	{
		m_values.push_back(a->getValue(i));
	}
}

void MFDouble::setValue(CVRMLFieldData *data)
{
	if (!data) return;
	if (data->getFieldType() != DOUBLE_ARRAY_DATA) return;

	CVRMLDoubleArrayData *a = (CVRMLDoubleArrayData *)data;

	m_values.clear();
	for (int i=0; i<a->count(); i++)
	{
		m_values.push_back(a->getValue(i));
	}
}

void MFVec2f::setValue(CVRMLFieldData *data)
{
	if (!data) return;
	if (data->getFieldType() != FLOAT_ARRAY_DATA) return;

	CVRMLFloatArrayData *a = (CVRMLFloatArrayData *)data;

	float x, y;
	x = y = 0.0f;

	m_values.clear();
	for (int i=0; i<a->count(); i++)
	{
		int j = (i % 2);

		switch (j)
		{
		case 0:
			x = a->getValue(i);
			break;
		case 1:
			y = a->getValue(i);

			SFVec2f vec;
			vec.setValue(x, y);
			m_values.push_back(vec);

			x = y = 0.0f;
			break;
		}
	}
}

void MFVec3f::setValue(CVRMLFieldData *data)
{
	if (!data) return;
	if (data->getFieldType() != FLOAT_ARRAY_DATA) return;

	CVRMLFloatArrayData *a = (CVRMLFloatArrayData *)data;

	float x, y, z;
	x = y = z = 0.0f;

	m_values.clear();
	for (int i=0; i<a->count(); i++)
	{
		int j = (i % 3);

		switch (j)
		{
		case 0:
			x = a->getValue(i);
			break;
		case 1:
			y = a->getValue(i);
			break;
		case 2:
			z = a->getValue(i);

			SFVec3f vec;
			vec.setValue(x, y, z);
			m_values.push_back(vec);

			x = y = z = 0.0f;
			break;
		}
	}
}

void MFVec4f::setValue(CVRMLFieldData *data)
{
	if (!data) return;
	if (data->getFieldType() != FLOAT_ARRAY_DATA) return;

	CVRMLFloatArrayData *a = (CVRMLFloatArrayData *)data;

	float x, y, z, w;
	x = y = z = w = 0.0f;

	m_values.clear();
	for (int i=0; i<a->count(); i++)
	{
		int j = (i % 4);

		switch (j)
		{
		case 0:
			x = a->getValue(i);
			break;
		case 1:
			y = a->getValue(i);
			break;
		case 2:
			z = a->getValue(i);
			break;
		case 3:
			w = a->getValue(i);

			SFVec4f vec;
			vec.setValue(x, y, z, w);
			m_values.push_back(vec);

			x = y = z = w = 0.0f;
			break;
		}
	}
}

void MFVec2d::setValue(CVRMLFieldData *data)
{
	if (!data) return;
	if (data->getFieldType() != DOUBLE_ARRAY_DATA) return;

	CVRMLDoubleArrayData *a = (CVRMLDoubleArrayData *)data;

	double x, y;
	x = y = 0.0;

	m_values.clear();
	for (int i=0; i<a->count(); i++)
	{
		int j = (i % 2);

		switch (j)
		{
		case 0:
			x = a->getValue(i);
			break;
		case 1:
			y = a->getValue(i);

			SFVec2d vec;
			vec.setValue(x, y);
			m_values.push_back(vec);

			x = y = 0.0;
			break;
		}
	}
}

void MFVec3d::setValue(CVRMLFieldData *data)
{
	if (!data) return;
	if (data->getFieldType() != DOUBLE_ARRAY_DATA) return;

	CVRMLDoubleArrayData *a = (CVRMLDoubleArrayData *)data;

	double x, y, z;
	x = y = z = 0.0;

	m_values.clear();
	for (int i=0; i<a->count(); i++)
	{
		int j = (i % 3);

		switch (j)
		{
		case 0:
			x = a->getValue(i);
			break;
		case 1:
			y = a->getValue(i);
			break;
		case 2:
			z = a->getValue(i);

			SFVec3d vec;
			vec.setValue(x, y, z);
			m_values.push_back(vec);

			x = y = z = 0.0;
			break;
		}
	}
}

void MFVec4d::setValue(CVRMLFieldData *data)
{
	if (!data) return;
	if (data->getFieldType() != DOUBLE_ARRAY_DATA) return;

	CVRMLDoubleArrayData *a = (CVRMLDoubleArrayData *)data;

	double x, y, z, w;
	x = y = z = w = 0.0;

	m_values.clear();
	for (int i=0; i<a->count(); i++)
	{
		int j = (i % 4);

		switch (j)
		{
		case 0:
			x = a->getValue(i);
			break;
		case 1:
			y = a->getValue(i);
			break;
		case 2:
			z = a->getValue(i);
			break;
		case 3:
			w = a->getValue(i);

			SFVec4d vec;
			vec.setValue(x, y, z, w);
			m_values.push_back(vec);

			x = y = z = w = 0.0;
			break;
		}
	}
}

void MFColor::setValue(CVRMLFieldData *data)
{
	if (!data) return;
	if (data->getFieldType() != FLOAT_ARRAY_DATA) return;

	float r, g, b;
	r = g = b = 0.0f;

	m_values.clear();

	CVRMLFloatArrayData *a = (CVRMLFloatArrayData *)data;
	for (int i=0; i<a->count(); i++)
	{
		int j=(i % 3);

		switch (j)
		{
		case 0:
			r = a->getValue(i);
			break;
		case 1:
			g = a->getValue(i);
			break;
		case 2:
			b = a->getValue(i);

			SFColor color;
			color.setValue(r, g, b);

			m_values.push_back(color);

			r = g = b = 0.0f;
			break;
		}
	}
}

void MFString::setValue(CVRMLFieldData *data)
{
	if (!data) return;
	if (data->getFieldType() != STRING_ARRAY_DATA) return;

	CVRMLStringArrayData *pArray = (CVRMLStringArrayData *)data;
	if (pArray)
	{
		m_values.clear();

		int n = pArray->count();
		for (int i=0; i<n; i++)
		{
			const char *p = pArray->getValue(i);
			if (p)
			{
				m_values.push_back(p);
			}
			else
			{
				m_values.push_back("");
			}
		}
	}
}

void MFNode::setValue(CVRMLFieldData *data)
{
	if (!data) return;
	if (data->getFieldType() != NODE_ARRAY_DATA) return;

	CVRMLNodeArrayData *pArray = (CVRMLNodeArrayData *)data;
	if (pArray)
	{
		free();

		int n = pArray->count();
		for (int i=0; i<n; i++)
		{
#if 1
			jobject vrmlNode = pArray->getValue(i);
			if (vrmlNode)
			{
				CX3DNode *node = CX3DNodeFactory::createNode(vrmlNode);
				if (node)
				{
					m_values.push_back(node);
				}
			}
#else
			// old
			jobject vrmlNode = pArray->getValue(i);
			CX3DNode *node = CX3DNodeFactory::createNode(vrmlNode);
			m_values.push_back(node);
#endif
		}
	}
}

