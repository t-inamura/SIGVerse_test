#ifndef _CX3DBOXNODE_H_
#define _CX3DBOXNODE_H_

#include "CX3DNode.h"

class CX3DBoxNode : public CX3DNode
{
public:
	CX3DBoxNode(jobject vrmlNode);
	CX3DNodeType getNodeType() { return BOX_NODE; }

	SFBool *getSolid() { return &m_solid; }
	SFVec3f *getSize() { return &m_size; }

	void print(int indent=0);

private:
	SFBool m_solid;
	SFVec3f m_size;
};

#endif
