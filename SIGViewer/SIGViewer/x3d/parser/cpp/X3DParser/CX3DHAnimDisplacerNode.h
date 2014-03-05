#ifndef _CX3DHANIMDISPLACERNODE_H_
#define _CX3DHANIMDISPLACERNODE_H_

#include "CX3DNode.h"

class CX3DHAnimDisplacerNode : public CX3DNode
{
public:
	CX3DHAnimDisplacerNode(jobject vrmlNode);
	CX3DNodeType getNodeType() { return HANIM_DISPLACER_NODE; }

	MFInt32 *getCoordIndex() { return &m_coordIndex; }
	MFFloat *getDisplacements() { return &m_displacements; }
	SFString *getName() { return &m_name; }

	void print(int indent=0);

private:
	MFInt32 m_coordIndex;
	MFFloat m_displacements;
	SFString m_name;
};

#endif
