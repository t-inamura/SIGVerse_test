#ifndef _CX3DVIEWPOINTNODE_H_
#define _CX3DVIEWPOINTNODE_H_

#include "CX3DNode.h"

class CX3DViewpointNode : public CX3DNode
{
public:
	CX3DViewpointNode(jobject vrmlNode);
	CX3DNodeType getNodeType() { return VIEWPOINT_NODE; }

	SFFloat *getFieldOfView() { return &m_fieldOfView; }
	SFBool *getJump() { return &m_jump; }
	SFBool *getRetainUserOffsets() { return &m_retainUserOffsets; }
	SFRotation *getOrientation() { return &m_orientation; }
	SFVec3f *getPosition() { return &m_position; }
	SFVec3f *getCenterOfRotation() { return &m_centerOfRotation; }
	SFString *getDescription() { return &m_description; }

	void print(int indent=0);

private:
	SFFloat m_fieldOfView;
	SFBool m_jump;
	SFBool m_retainUserOffsets;
	SFRotation m_orientation;
	SFVec3f m_position;
	SFVec3f m_centerOfRotation;
	SFString m_description;
};

#endif
