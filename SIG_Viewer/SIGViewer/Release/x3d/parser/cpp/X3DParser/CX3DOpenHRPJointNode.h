#ifndef _CX3DOPENHRPJOINTNODE_H_
#define _CX3DOPENHRPJOINTNODE_H_

#include "CX3DNode.h"

class CX3DOpenHRPJointNode : public CX3DNode
{
public:
	CX3DOpenHRPJointNode(jobject vrmlNode);
	CX3DNodeType getNodeType() { return OPENHRP_JOINT_NODE; }

	MFNode *getChildren() { return &m_children; }
	SFVec3f *getCenter() { return &m_center; }
	SFRotation *getRotation() { return &m_rotation; }
	SFVec3f *getScale() { return &m_scale; }
	SFRotation *getScaleOrientation() { return &m_scaleOrientation; }
	SFVec3f *getTranslation() { return &m_translation; }
	SFString *getName() { return &m_name; }
	SFRotation *getLimitOrientation() { return &m_limitOrientation; }
	MFFloat *getLlimit() { return &m_llimit; }
	MFFloat *getStiffness() { return &m_stiffness; }
	MFFloat *getUlimit() { return &m_ulimit; }

	SFString *getJointType() { return &m_jointType; }
	SFInt32 *getJointId() { return &m_jointId; }
	SFVec3f *getJointAxis() { return &m_jointAxis; }
	SFFloat *getEquivalentInertia() { return &m_equivalentInertia; }
	SFFloat *getGearRatio() { return &m_gearRatio; }
	SFFloat *getRotorInertia() { return &m_rotorInertia; }
	SFFloat *getRotorResistor() { return &m_rotorResistor; }
	SFFloat *getTorqueConst() { return &m_torqueConst; }
	SFFloat *getEncoderPulse() { return &m_encoderPulse; }
	SFFloat *getGearEfficiency() { return &m_gearEfficiency; }

//	MFInt32 *getSkinCoordIndex() { return &m_skinCoordIndex; }
//	MFFloat *getSkinCoordWeight() { return &m_skinCoordWeight; }

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
	MFFloat m_stiffness;
	MFFloat m_ulimit;

	SFString m_jointType;
	SFInt32 m_jointId;
	SFVec3f m_jointAxis;
	SFFloat m_equivalentInertia;
	SFFloat m_gearRatio;
	SFFloat m_rotorInertia;
	SFFloat m_rotorResistor;
	SFFloat m_torqueConst;
	SFFloat m_encoderPulse;
	SFFloat m_gearEfficiency;

//	MFInt32 m_skinCoordIndex;
//	MFFloat m_skinCoordWeight;
};

#endif

