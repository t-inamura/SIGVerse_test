#ifndef _CX3DCOLORNODE_H_
#define _CX3DCOLORNODE_H_

#include "CX3DNode.h"

class CX3DColorNode : public CX3DNode
{
public:
	CX3DColorNode(jobject vrmlNode);
	CX3DNodeType getNodeType() { return COLOR_NODE; }

	MFColor *getColor() { return &m_color; }

	void print(int indent=0);

private:
	MFColor m_color;
};

#endif
