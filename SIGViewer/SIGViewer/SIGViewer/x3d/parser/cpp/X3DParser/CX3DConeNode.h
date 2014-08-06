#ifndef _CX3DCONENODE_H_
#define _CX3DCONENODE_H_

#include "CX3DNode.h"

class CX3DConeNode : public CX3DNode
{
public:
	CX3DConeNode(jobject vrmlNode);
	CX3DNodeType getNodeType() { return CONE_NODE; }

	SFBool *getSolid() { return &m_solid; }
	SFFloat *getBottomRadius() { return &m_bottomRadius; }
	SFFloat *getHeight() { return &m_height; }
	SFBool *getBottom() { return &m_bottom; }
	SFBool *getSide() { return &m_side; }

	void print(int indent=0);

private:
	SFBool m_solid;
	SFFloat m_bottomRadius;
	SFFloat m_height;
	SFBool m_bottom;
	SFBool m_side;
};

#endif
