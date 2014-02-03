#ifndef _CX3DOPENHRPSEGMENTNODE_H_
#define _CX3DOPENHRPSEGMENTNODE_H_

#include "CX3DNode.h"

class CX3DOpenHRPSegmentNode : public CX3DNode
{
public:
	CX3DOpenHRPSegmentNode(jobject vrmlNode);
	CX3DNodeType getNodeType() { return OPENHRP_SEGMENT_NODE; }

	MFNode *getChildren() { return &m_children; }
	SFString *getName() { return &m_name; }
	SFNode *getCoord() { return &m_coord; }
	SFVec3f *getCenterOfMass() { return &m_centerOfMass; }
	MFNode *getDisplacers() { return &m_displacers; }
	SFFloat *getMass() { return &m_mass; }
	MFFloat *getMomentsOfInertia() { return &m_momentsOfInertia; }

	void print(int indent=0);

private:
	MFNode m_children;
	SFString m_name;
	SFNode m_coord;
	SFVec3f m_centerOfMass;
	MFNode m_displacers;
	SFFloat m_mass;
	MFFloat m_momentsOfInertia;
};

#endif

