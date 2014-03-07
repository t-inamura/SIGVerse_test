#ifndef _CX3DTEXTURETRANSFORMNODE_H_
#define _CX3DTEXTURETRANSFORMNODE_H_

#include "CX3DNode.h"

class CX3DTextureTransformNode : public CX3DNode
{
public:
	CX3DTextureTransformNode(jobject vrmlNode);
	CX3DNodeType getNodeType() { return TEXTURE_TRANSFORM_NODE; }

	SFVec2f *getCenter() { return &m_center; }
	SFFloat *getRotation() { return &m_rotation; }
	SFVec2f *getScale() { return &m_scale; }
	SFVec2f *getTranslation() { return &m_translation; }

	void print(int indent=0);

private:
	SFVec2f m_center;
	SFFloat m_rotation;
	SFVec2f m_scale;
	SFVec2f m_translation;
};

#endif
