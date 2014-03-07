#ifndef _CX3DSHAPENODE_H_
#define _CX3DSHAPENODE_H_

#include "CX3DNode.h"

class CX3DShapeNode : public CX3DNode
{
public:
	CX3DShapeNode(jobject vrmlNode);
	CX3DNodeType getNodeType() { return SHAPE_NODE; }

	SFNode *getAppearance() { return &m_appearance; }
	SFNode *getGeometry() { return &m_geometry; }

	void print(int indent=0);

private:
	SFNode m_appearance;
	SFNode m_geometry;
};

#endif
