#ifndef _CX3DOPENHRPHUMANOIDNODE_H_
#define _CX3DOPENHRPHUMANOIDNODE_H_

#include "CX3DNode.h"

class CX3DOpenHRPHumanoidNode : public CX3DNode
{
public:
	CX3DOpenHRPHumanoidNode(jobject vrmlNode);
	CX3DNodeType getNodeType() { return OPENHRP_HUMANOID_NODE; }

	SFVec3f *getCenter() { return &m_center; }
	SFRotation *getRotation() { return &m_rotation; }
	SFVec3f *getScale() { return &m_scale; }
	SFRotation *getScaleOrientation() { return &m_scaleOrientation; }
	SFVec3f *getTranslation() { return &m_translation; }
	SFString *getName() { return &m_name; }
	MFString *getInfo() { return &m_info; }
	MFNode *getJoints() { return &m_joints; }
	MFNode *getSegments() { return &m_segments; }
	MFNode *getSites() { return &m_sites; }
	SFString *getVersion() { return &m_version; }
	MFNode *getViewpoints() { return &m_viewpoints; }

	MFNode *getHumanoidBody() { return &m_humanoidBody; }

//	MFNode *getSkeleton() { return &m_skeleton; }
//	MFNode *getSkin() { return &m_skin; }
//	SFNode *getSkinCoord() { return &m_skinCoord; }
//	SFNode *getSkinNormal() { return &m_skinNormal; }

	void print(int indent=0);

private:
	SFVec3f m_center;
	SFRotation m_rotation;
	SFVec3f m_scale;
	SFRotation m_scaleOrientation;
	SFVec3f m_translation;
	SFString m_name;
	MFString m_info;
	MFNode m_joints;
	MFNode m_segments;
	MFNode m_sites;
	SFString m_version;
	MFNode m_viewpoints;

	MFNode m_humanoidBody;

//	MFNode m_skeleton;
//	MFNode m_skin;
//	SFNode m_skinCoord;
//	SFNode m_skinNormal;
};

#endif

