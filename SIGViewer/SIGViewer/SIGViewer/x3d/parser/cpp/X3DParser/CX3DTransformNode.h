#ifndef _CX3DTRANSFORMNODE_H_
#define _CX3DTRANSFORMNODE_H_

#include "CX3DNode.h"

class CX3DTransformNode : public CX3DNode
{
public:
	CX3DTransformNode(jobject vrmlNode);
	CX3DNodeType getNodeType() { return TRANSFORM_NODE; }

	SFVec3f *getTranslation() { return &m_translation; }
	SFRotation *getRotation() { return &m_rotation; }
	SFVec3f *getCenter() { return &m_center; }
	SFVec3f *getScale() { return &m_scale; }
	SFRotation *getScaleOrientation() { return &m_scaleOrientation; }
	MFNode *getChildren() { return &m_children; }

	void print(int indent=0);

private:
	SFVec3f m_translation;
	SFRotation m_rotation;
	SFVec3f m_center;
	SFVec3f m_scale;
	SFRotation m_scaleOrientation;
	MFNode m_children;
};

#endif
