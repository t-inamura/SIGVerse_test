#ifndef _CX3DCOORDINATENODE_H_
#define _CX3DCOORDINATENODE_H_

#include "CX3DNode.h"

class CX3DCoordinateNode : public CX3DNode
{
public:
	CX3DCoordinateNode(jobject vrmlNode);
	CX3DNodeType getNodeType() { return COORDINATE_NODE; }

	MFVec3f *getPoint() { return &m_point; }

	void print(int indent=0);

private:
	MFVec3f m_point;
};

#endif
