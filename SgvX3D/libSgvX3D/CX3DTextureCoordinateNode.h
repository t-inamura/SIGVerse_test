#ifndef _CX3DTEXTURECOORDINATENODE_H_
#define _CX3DTEXTURECOORDINATENODE_H_

#include "CX3DNode.h"

class CX3DTextureCoordinateNode : public CX3DNode
{
public:
	CX3DTextureCoordinateNode(jobject vrmlNode);
	CX3DNodeType getNodeType() { return TEXTURE_COORDINATE_NODE; }

	MFVec2f *getPoint() { return &m_point; }

	void print(int indent=0);

private:
	MFVec2f m_point;
};

#endif
