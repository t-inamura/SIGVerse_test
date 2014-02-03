#include "CX3DIndexedFaceSetNode.h"
#include "CX3DParserUtil.h"
#include "CX3DParser.h"
#include "CJNIUtil.h"
#include <math.h>

static float min6Float(float *f);

static float sqrDist(float px, float py, float pz, float qx, float qy, float qz);

/////////////////////////////////////////////////////////
//
//  CX3DIndexedFaceSetNode

CX3DIndexedFaceSetNode::CX3DIndexedFaceSetNode(jobject vrmlNode)
{

	m_convex.setValue(false);
	isConvex = false;
	CX3DParser::printLog("convex = false\n");

	if (!vrmlNode)
	{
		m_vrmlNode = NULL;
		return;
	}

	CJNIUtil *ju = CJNIUtil::getUtil();

	if (!ju->isInstanceOfVRMLNode(vrmlNode))
	{
		fprintf(stderr, "vrmlNode is not instance of VRMLNode [%s:%d]\n", __FILE__, __LINE__);
		exit(1);
	}

	m_vrmlNode = ju->env()->NewGlobalRef(vrmlNode);
	
	CX3DNode *tmpNode = new CX3DNode(vrmlNode);
	if (tmpNode)
	{
		char *nodeName = tmpNode->getNodeName();

		if (nodeName && strcmp(nodeName, "IndexedFaceSet")==0)
		{
			int n = tmpNode->countFields();
			for (int i=0; i<n; i++)
			{
				char *pFieldName = tmpNode->getFieldName(i);
				if (pFieldName)
				{
					std::string fieldName = pFieldName;

					CVRMLFieldData *data = tmpNode->createFieldValue((char *)(fieldName.c_str()));
					if (data)
					{
						CX3DParser::printLog("field name = %s\n", fieldName.c_str());

						if (strcmp(fieldName.c_str(), "coord") == 0)
						{
							m_coord.setValue(data);
						}
						else if (strcmp(fieldName.c_str(), "color") == 0)
						{
							m_color.setValue(data);
						}
						else if (strcmp(fieldName.c_str(), "normal") == 0)
						{
							m_normal.setValue(data);
						}
						else if (strcmp(fieldName.c_str(), "texCoord") == 0)
						{
							m_texCoord.setValue(data);
						}
						else if (strcmp(fieldName.c_str(), "solid") == 0)
						{
							m_solid.setValue(data);
						}
						else if (strcmp(fieldName.c_str(), "convex") == 0)
						{
							//if(data->)
							isConvex = true;
							CX3DParser::printLog("convex!!!\n");
							m_convex.setValue(data);
						}
						else if (strcmp(fieldName.c_str(), "ccw") == 0)
						{
							m_ccw.setValue(data);
						}
						else if (strcmp(fieldName.c_str(), "creaseAngle") == 0)
						{
							m_creaseAngle.setValue(data);
						}
						else if (strcmp(fieldName.c_str(), "colorPerVertex") == 0)
						{
							m_colorPerVertex.setValue(data);
						}
						else if (strcmp(fieldName.c_str(), "normalPerVertex") == 0)
						{
							m_normalPerVertex.setValue(data);
						}
						else if (strcmp(fieldName.c_str(), "coordIndex") == 0)
						{
							m_coordIndex.setValue(data);
						}
						else if (strcmp(fieldName.c_str(), "colorIndex") == 0)
						{
							m_colorIndex.setValue(data);
						}
						else if (strcmp(fieldName.c_str(), "normalIndex") == 0)
						{
							m_normalIndex.setValue(data);
						}
						else if (strcmp(fieldName.c_str(), "texCoordIndex") == 0)
						{
							m_texCoordIndex.setValue(data);
						}

						delete data;
					}
				}
			}
		}
		delete tmpNode;
	}
}

void CX3DIndexedFaceSetNode::print(int indent)
{
	FILE *fp = CX3DParser::getDebugLogFp();
	int nMax = CX3DParser::getMaxPrintElemsForMFField();
	bool bPartialPrint = false;

	char *nodeName = getNodeName();
	if (nodeName)
	{
		int i, n;

		CX3DParser::printIndent(indent);
		fprintf(fp, "%s (%s)\n", nodeName, CX3DNode::getNodeTypeString(getNodeType()));

		CX3DParser::printIndent(indent+1);
		fprintf(fp, "coord\n");
		CX3DNode *pCoord = getCoord()->getNode();
		if (pCoord) pCoord->print(indent+2);

		CX3DParser::printIndent(indent+1);
		fprintf(fp, "color\n");
		CX3DNode *pColor = getColor()->getNode();
		if (pColor) pColor->print(indent+2);

		CX3DParser::printIndent(indent+1);
		fprintf(fp, "normal\n");
		CX3DNode *pNormal = getNormal()->getNode();
		if (pNormal) pNormal->print(indent+2);

		CX3DParser::printIndent(indent+1);
		fprintf(fp, "texCoord\n");
		CX3DNode *pTexCoord = getTexCoord()->getNode();
		if (pTexCoord) pTexCoord->print(indent+2);

		CX3DParser::printIndent(indent+1);
		fprintf(fp, "solid : %s\n", getSolid()->getValue() ? "TRUE" : "FALSE");

		CX3DParser::printIndent(indent+1);
		fprintf(fp, "convex : %s\n", getConvex()->getValue() ? "TRUE" : "FALSE");

		CX3DParser::printIndent(indent+1);
		fprintf(fp, "ccw : %s\n", getCCW()->getValue() ? "TRUE" : "FALSE");

		CX3DParser::printIndent(indent+1);
		fprintf(fp, "creaseAngle : %f\n", getCreaseAngle()->getValue());

		CX3DParser::printIndent(indent+1);
		fprintf(fp, "colorPerVertex : %s\n", getColorPerVertex()->getValue() ? "TRUE" : "FALSE");

		CX3DParser::printIndent(indent+1);
		fprintf(fp, "normalPerVertex : %s\n", getNormalPerVertex()->getValue() ? "TRUE" : "FALSE");

		CX3DParser::printIndent(indent+1);
		n = m_coordIndex.count();
		fprintf(fp, "coordIndex [%d]\n", n);
		if ((nMax > 0) && (n > nMax)) { n = nMax; bPartialPrint = true; }
		else { bPartialPrint = false; }
		CX3DParser::printIndent(indent+2);
		fprintf(fp, "(");
		for (i=0; i<n; i++)
		{
			fprintf(fp, "%d ", m_coordIndex.getValue(i));
		}
		if (bPartialPrint) fprintf(fp, "...");
		fprintf(fp, ")\n");

		CX3DParser::printIndent(indent+1);
		n = m_colorIndex.count();
		fprintf(fp, "colorIndex [%d]\n", n);
		if ((nMax > 0) && (n > nMax)) { n = nMax; bPartialPrint = true; }
		else { bPartialPrint = false; }
		CX3DParser::printIndent(indent+2);
		fprintf(fp, "(");
		for (i=0; i<n; i++)
		{
			fprintf(fp, "%d ", m_colorIndex.getValue(i));
		}
		if (bPartialPrint) fprintf(fp, "...");
		fprintf(fp, ")\n");

		CX3DParser::printIndent(indent+1);
		n = m_normalIndex.count();
		fprintf(fp, "normalIndex [%d]\n", n);
		if ((nMax > 0) && (n > nMax)) { n = nMax; bPartialPrint = true; }
		else { bPartialPrint = false; }
		CX3DParser::printIndent(indent+2);
		fprintf(fp, "(");
		for (i=0; i<n; i++)
		{
			fprintf(fp, "%d ", m_normalIndex.getValue(i));
		}
		if (bPartialPrint) fprintf(fp, "...");
		fprintf(fp, ")\n");

		CX3DParser::printIndent(indent+1);
		n = m_texCoordIndex.count();
		fprintf(fp, "texCoordIndex [%d]\n", n);
		if ((nMax > 0) && (n > nMax)) { n = nMax; bPartialPrint = true; }
		else { bPartialPrint = false; }
		CX3DParser::printIndent(indent+2);
		fprintf(fp, "(");
		for (i=0; i<n; i++)
		{
			fprintf(fp, "%d ", m_texCoordIndex.getValue(i));
		}
		if (bPartialPrint) fprintf(fp, "...");
		fprintf(fp, ")\n");
	}
}

//////////////////////////////////////////////////////////////
//

// ------------------------------------------
// ------------------------------------------
bool CX3DIndexedFaceSetNode::calcSimplifiedSphere(OV_Sphere& retValue)
{
	int i;

	CX3DCoordinateNode *pCoord = (CX3DCoordinateNode *)(getCoord()->getNode());
	if (!pCoord) return false;

	MFVec3f *coords = pCoord->getPoint();

	MFInt32 *coordIndex = getCoordIndex();
	bool bHasCoordIndex = (coordIndex->count() > 0) ? true : false;

	float gx, gy, gz;
	gx = gy = gz = 0;

	int n = bHasCoordIndex ? coordIndex->count() : coords->count();

	int nv = 0;

	for (i=0; i<n; i++)
	{
		SFVec3f pos;
		
		if (bHasCoordIndex)
		{
			int ind = coordIndex->getValue(i);
			if (ind < 0) continue;

			pos = coords->getValue(ind);
		}
		else
		{
			pos = coords->getValue(i);
		}

		gx += pos.x();
		gy += pos.y();
		gz += pos.z();

		nv++;
	}

	gx /= nv;
	gy /= nv;
	gz /= nv;

	float fr, nr, ar;
	bool bFirst = true;

	for (i=0; i<n; i++)
	{
		SFVec3f pos;
		
		if (bHasCoordIndex)
		{
			int ind = coordIndex->getValue(i);
			if (ind < 0) continue;

			pos = coords->getValue(ind);
		}
		else
		{
			pos = coords->getValue(i);
		}

		if (bFirst)
		{
			fr = nr = sqrDist(gx, gy, gz, pos.x(), pos.y(), pos.z());
			bFirst = false;
		}
		else
		{
			float d = sqrDist(gx, gy, gz, pos.x(), pos.y(), pos.z());

			if (d > fr) fr = d;
			if (d < nr) nr = d;
		}
	}
	fr = sqrt(fr);
	nr = sqrt(nr);
	ar = (nr + fr)/2;
	
	float yugami = calcYugamiWithSphere(ar, gx, gy, gz, coords, coordIndex);

	retValue.x = gx;
	retValue.y = gy;
	retValue.z = gz;
	retValue.r = ar;
	retValue.yugami = yugami;

	return true;
}

float CX3DIndexedFaceSetNode::calcYugamiWithSphere(float r, float gx, float gy, float gz, MFVec3f *coords, MFInt32 *coordIndex)
{
	float yugami = 0;

	bool bHasCoordIndex = (coordIndex->count() > 0) ? true : false;

	int n = bHasCoordIndex ? coordIndex->count() : coords->count();
	int nv = 0;
	for (int i=0; i<n; i++)
	{
		SFVec3f pos;
		
		if (bHasCoordIndex)
		{
			int ind = coordIndex->getValue(i);
			if (ind < 0) continue;

			pos = coords->getValue(ind);
		}
		else
		{
			pos = coords->getValue(i);
		}

		float d = sqrt(sqrDist(gx, gy, gz, pos.x(), pos.y(), pos.z()));

		yugami = yugami + (d-r)*(d-r);

		nv++;
	}

	return (nv > 0) ? yugami/nv : 0.0f;
}

// ------------------------------------------
//
//
//
//
// ------------------------------------------
bool CX3DIndexedFaceSetNode::calcSimplifiedCylinder(OV_Cylinder& retValue)
{
	int i;

	CX3DCoordinateNode *pCoord = (CX3DCoordinateNode *)(getCoord()->getNode());
	if (!pCoord) return false;

	MFVec3f *coords = pCoord->getPoint();

	MFInt32 *coordIndex = getCoordIndex();
	bool bHasCoordIndex = (coordIndex->count() > 0) ? true : false;

	float gx, gy, gz;
	gx = gy = gz = 0;

	int n = bHasCoordIndex ? coordIndex->count() : coords->count();
	int nv = 0;
	for (i=0; i<n; i++)
	{
		SFVec3f pos;

		if (bHasCoordIndex)
		{
			int ind = coordIndex->getValue(i);
			if (ind < 0) continue;

			pos = coords->getValue(ind);
		}
		else
		{
			pos = coords->getValue(i);
		}

		gx += pos.x();
		gy += pos.y();
		gz += pos.z();

		nv++;
	}

	gx /= nv;
	gy /= nv;
	gz /= nv;

	retValue.x = gx;
	retValue.y = gy;
	retValue.z = gz;

	float fr, nr, ar;
	bool bFirst = true;

	for (i=0; i<n; i++)
	{
		SFVec3f pos;
		
		if (bHasCoordIndex)
		{
			int ind = coordIndex->getValue(i);
			if (ind < 0) continue;

			pos = coords->getValue(ind);
		}
		else
		{
			pos = coords->getValue(i);
		}

		if (bFirst)
		{
			fr = nr = sqrDist(gx, 0, gz, pos.x(), 0, pos.z());
			bFirst = false;
		}
		else
		{
			float d = sqrDist(gx, 0, gz, pos.x(), 0, pos.z());

			if (d > fr) fr = d;
			if (d < nr) nr = d;
		}
	}
	fr = sqrt(fr);
	nr = sqrt(nr);
	ar = (nr + fr)/2;

	retValue.r = ar;

	float maxy, miny;
	bFirst = true;
	for (i=0; i<n; i++)
	{
		SFVec3f pos;
		
		if (bHasCoordIndex)
		{
			int ind = coordIndex->getValue(i);
			if (ind < 0) continue;

			pos = coords->getValue(ind);
		}
		else
		{
			pos = coords->getValue(i);
		}

		if (bFirst)
		{
			maxy = miny = pos.y();
			bFirst = false;
		}
		else
		{
			float d = pos.y();

			if (d > maxy) maxy = d;
			if (d < miny) miny = d;
		}
	}

	retValue.h = maxy - miny;

	float yugami = calcYugamiWithCylinder(ar, gx, gz, coords, coordIndex);

	retValue.yugami = yugami;

	return true;
}

float CX3DIndexedFaceSetNode::calcYugamiWithCylinder(float r, float cx, float cz, MFVec3f *coords, MFInt32 *coordIndex)
{
	float yugami = 0;

	bool bHasCoordIndex = (coordIndex->count() > 0) ? true : false;

	int n = bHasCoordIndex ? coordIndex->count() : coords->count();
	int nv = 0;
	for (int i=0; i<n; i++)
	{
		SFVec3f pos;
		
		if (bHasCoordIndex)
		{
			int ind = coordIndex->getValue(i);
			if (ind < 0) continue;

			pos = coords->getValue(ind);
		}
		else
		{
			pos = coords->getValue(i);
		}

		float d = sqrt(sqrDist(cx, 0, cz, pos.x(), 0, pos.z()));

		yugami = yugami + (d-r)*(d-r);

		nv++;
	}

	return (nv > 0) ? yugami/nv : 0.0f;
}

// ------------------------------------------
// ------------------------------------------
bool CX3DIndexedFaceSetNode::calcSimplifiedBox(OV_Box& retValue)
{
	int i, n;

	CX3DCoordinateNode *pCoord = (CX3DCoordinateNode *)(getCoord()->getNode());
	if (!pCoord) return false;

	MFVec3f *coords = pCoord->getPoint();

	MFInt32 *coordIndex = getCoordIndex();
	bool bHasCoordIndex = (coordIndex->count() > 0) ? true : false;

	float x1, y1, z1, x2, y2, z2;

	n = bHasCoordIndex ? coordIndex->count() : coords->count();
	bool bFirst = true;
	for (i=0; i<n; i++)
	{
		SFVec3f pos;

		if (bHasCoordIndex)
		{
			int ind = coordIndex->getValue(i);
			if (ind < 0) continue;

			pos = coords->getValue(ind);
		}
		else
		{
			pos = coords->getValue(i);
		}

		if (bFirst)
		{
			x1 = x2 = pos.x();
			y1 = y2 = pos.y();
			z1 = z2 = pos.z();

			bFirst = false;
		}
		else
		{
			float x = pos.x();
			float y = pos.y();
			float z = pos.z();

			if (x < x1) x1 = x;
			if (x > x2) x2 = x;

			if (y < y1) y1 = y;
			if (y > y2) y2 = y;

			if (z < z1) z1 = z;
			if (z > z2) z2 = z;
		}
	}

	retValue.x1 = x1;
	retValue.y1 = y1;
	retValue.z1 = z1;
	retValue.x2 = x2;
	retValue.y2 = y2;
	retValue.z2 = z2;

	retValue.yugami = calcYugamiWithBox(x1, y1, z1, x2, y2, z2, coords, coordIndex);

	return true;
}

float CX3DIndexedFaceSetNode::calcYugamiWithBox(float x1, float y1, float z1, float x2, float y2, float z2, MFVec3f *coords, MFInt32 *coordIndex)
{
	float yugami = 0;
	float dist[6];
	float d;

	bool bHasCoordIndex = (coordIndex->count() > 0) ? true : false;

	int n = bHasCoordIndex ? coordIndex->count() : coords->count();
	int nv = 0;
	for (int i=0; i<n; i++)
	{
		SFVec3f pos;
		
		if (bHasCoordIndex)
		{
			int ind = coordIndex->getValue(i);
			if (ind < 0) continue;

			pos = coords->getValue(ind);
		}
		else
		{
			pos = coords->getValue(i);
		}

		dist[0] = pos.x() - x1;
		dist[1] = x2 - pos.x();
		dist[2] = pos.y() - y1;
		dist[3] = y2 - pos.y();
		dist[4] = pos.z() - z1;
		dist[5] = z2 - pos.z();

		d = min6Float(dist);

		yugami = yugami + d*d;

		nv++;
	}

	return (nv > 0) ? yugami/nv : 0.0f;
}

float min6Float(float *f)
{
	float minValue;

	for (int i=0; i<6; i++)
	{
		float x = f[i];

		if (i==0)
		{
			minValue = x;
		}
		else
		{
			if (x < minValue) minValue = x;
		}
	}

	return minValue;
}

float sqrDist(float px, float py, float pz, float qx, float qy, float qz)
{
	return (px-qx)*(px-qx) + (py-qy)*(py-qy) + (pz-qz)*(pz-qz);
}

