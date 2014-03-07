#ifndef _CX3DNORMALNODE_H_
#define _CX3DNORMALNODE_H_

#include "CX3DNode.h"

class CX3DNormalNode : public CX3DNode
{
public:
	CX3DNormalNode(jobject vrmlNode);
	CX3DNodeType getNodeType() { return NORMAL_NODE; }

	MFVec3f *getVector() { return &m_vector; }

	void print(int indent=0);

private:
	MFVec3f m_vector;
};

#endif
