#ifndef _CX3DSPHERENODE_H_
#define _CX3DSPHERENODE_H_

#include "CX3DNode.h"

class CX3DSphereNode : public CX3DNode
{
public:
	CX3DSphereNode(jobject vrmlNode);
	CX3DNodeType getNodeType() { return SPHERE_NODE; }

	SFBool *getSolid() { return &m_solid; }
	SFFloat *getRadius() { return &m_radius; }

	void print(int indent=0);

private:
	SFBool m_solid;
	SFFloat m_radius;
};

#endif
