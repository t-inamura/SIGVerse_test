#ifndef _CX3DHANIMSITENODE_H_
#define _CX3DHANIMSITENODE_H_

#include "CX3DNode.h"

class CX3DHAnimSiteNode : public CX3DNode
{
public:
	CX3DHAnimSiteNode(jobject vrmlNode);
	CX3DNodeType getNodeType() { return HANIM_SITE_NODE; }

	MFNode *getChildren() { return &m_children; }
	SFVec3f *getCenter() { return &m_center; }
	SFRotation *getRotation() { return &m_rotation; }
	SFVec3f *getScale() { return &m_scale; }
	SFRotation *getScaleOrientation() { return &m_scaleOrientation; }
	SFVec3f *getTranslation() { return &m_translation; }
	SFString *getName() { return &m_name; }

	void print(int indent=0);

private:
	MFNode m_children;
	SFVec3f m_center;
	SFRotation m_rotation;
	SFVec3f m_scale;
	SFRotation m_scaleOrientation;
	SFVec3f m_translation;
	SFString m_name;
};

#endif
