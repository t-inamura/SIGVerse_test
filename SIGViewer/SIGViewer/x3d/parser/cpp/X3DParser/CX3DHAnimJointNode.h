#ifndef _CX3DHANIMJOINTNODE_H_
#define _CX3DHANIMJOINTNODE_H_

#include "CX3DNode.h"

class CX3DHAnimJointNode : public CX3DNode
{
public:
	CX3DHAnimJointNode(jobject vrmlNode);
	CX3DNodeType getNodeType() { return HANIM_JOINT_NODE; }

	MFNode *getChildren() { return &m_children; }
	SFVec3f *getCenter() { return &m_center; }
	SFRotation *getRotation() { return &m_rotation; }
	SFVec3f *getScale() { return &m_scale; }
	SFRotation *getScaleOrientation() { return &m_scaleOrientation; }
	SFVec3f *getTranslation() { return &m_translation; }
	SFString *getName() { return &m_name; }
	SFRotation *getLimitOrientation() { return &m_limitOrientation; }
	MFFloat *getLlimit() { return &m_llimit; }
	MFInt32 *getSkinCoordIndex() { return &m_skinCoordIndex; }
	MFFloat *getSkinCoordWeight() { return &m_skinCoordWeight; }
	MFFloat *getStiffness() { return &m_stiffness; }
	MFFloat *getUlimit() { return &m_ulimit; }

	void print(int indent=0);

private:
	MFNode m_children;
	SFVec3f m_center;
	SFRotation m_rotation;
	SFVec3f m_scale;
	SFRotation m_scaleOrientation;
	SFVec3f m_translation;
	SFString m_name;
	SFRotation m_limitOrientation;
	MFFloat m_llimit;
	MFInt32 m_skinCoordIndex;
	MFFloat m_skinCoordWeight;
	MFFloat m_stiffness;
	MFFloat m_ulimit;
};

#endif
