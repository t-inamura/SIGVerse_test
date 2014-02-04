#ifndef _CX3DINDEXEDFACESETNODE_H_
#define _CX3DINDEXEDFACESETNODE_H_

#include "CX3DNode.h"

struct OV_Sphere
{
	// radius
	float r;

	// center position
	float x, y, z;

	float yugami;
};

struct OV_Cylinder
{
	// radius
	float r;

	// height
	float h;

	// center of bottom circle
	float x, y, z;

	float yugami;
};

struct OV_Box
{
	// size
	float x1, y1, z1;
	float x2, y2, z2;

	float yugami;
};

class CX3DIndexedFaceSetNode : public CX3DNode
{
public:
	CX3DIndexedFaceSetNode(jobject vrmlNode);
	CX3DNodeType getNodeType() { return INDEXED_FACE_SET_NODE; }

	SFNode *getCoord() { return &m_coord; }
	SFNode *getColor() { return &m_color; }
	SFNode *getNormal() { return &m_normal; }
	SFNode *getTexCoord() { return &m_texCoord; }
	//SFNode *getFogCoord() { return &m_fogCoord; }
	//SFNode *getAttrib() { return &m_attrib; }
	SFBool *getSolid() { return &m_solid; }
	SFBool *getConvex() { 
		if(isConvex)
			return &m_convex; 
		else
			return NULL;
	}
	SFBool *getCCW() { return &m_ccw; }
	SFFloat *getCreaseAngle() { return &m_creaseAngle; }
	SFBool *getColorPerVertex() { return &m_colorPerVertex; }
	SFBool *getNormalPerVertex() { return &m_normalPerVertex; }
	MFInt32 *getCoordIndex() { return &m_coordIndex; }
	MFInt32 *getColorIndex() { return &m_colorIndex; }
	MFInt32 *getNormalIndex() { return &m_normalIndex; }
	MFInt32 *getTexCoordIndex() { return &m_texCoordIndex; }

	bool isConvex;

	// ---------------------------------
	// ---------------------------------
	bool calcSimplifiedSphere(OV_Sphere& retValue);

	bool calcSimplifiedCylinder(OV_Cylinder& retValue);

	bool calcSimplifiedBox(OV_Box& retValue);

	void print(int indent=0);

private:
	SFNode m_coord;
	SFNode m_color;
	SFNode m_normal;
	SFNode m_texCoord;
	//SFNode m_fogCoord;
	//SFNode m_attrib;
	SFBool m_solid;
	SFBool m_convex;
	SFBool m_ccw;
	SFFloat m_creaseAngle;
	SFBool m_colorPerVertex;
	SFBool m_normalPerVertex;
	MFInt32 m_coordIndex;
	MFInt32 m_colorIndex;
	MFInt32 m_normalIndex;
	MFInt32 m_texCoordIndex;

	float calcYugamiWithSphere(float r, float gx, float gy, float gz, MFVec3f *coords, MFInt32 *coordIndex);
	float calcYugamiWithCylinder(float r, float cx, float cz, MFVec3f *coords, MFInt32 *coordIndex);
	float calcYugamiWithBox(float x1, float y1, float z1, float x2, float y2, float z2, MFVec3f *coords, MFInt32 *coordIndex);
};

#endif

