#ifndef _CX3DCYLINDERNODE_H_
#define _CX3DCYLINDERNODE_H_

#include "CX3DNode.h"

class CX3DCylinderNode : public CX3DNode
{
public:
	CX3DCylinderNode(jobject vrmlNode);
	CX3DNodeType getNodeType() { return CYLINDER_NODE; }

	SFBool *getSolid() { return &m_solid; }
	SFFloat *getRadius() { return &m_radius; }
	SFFloat *getHeight() { return &m_height; }
	SFBool *getBottom() { return &m_bottom; }
	SFBool *getSide() { return &m_side; }
	SFBool *getTop() { return &m_top; }

	void print(int indent=0);

private:
	SFBool m_solid;
	SFFloat m_radius;
	SFFloat m_height;
	SFBool m_bottom;
	SFBool m_side;
	SFBool m_top;
};

#endif

