#ifndef _CX3DGROUPNODE_H_
#define _CX3DGROUPNODE_H_

#include "CX3DNode.h"

class CX3DGroupNode : public CX3DNode
{
public:
	CX3DGroupNode(jobject vrmlNode);
	CX3DNodeType getNodeType() { return GROUP_NODE; }

	MFNode *getChildren() { return &m_children; }

	void print(int indent=0);

private:
	MFNode m_children;
};

#endif
