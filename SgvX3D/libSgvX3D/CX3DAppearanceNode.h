#ifndef _CX3DAPPEARANCENODE_H_
#define _CX3DAPPEARANCENODE_H_

#include "CX3DNode.h"

class CX3DAppearanceNode : public CX3DNode
{
public:
	CX3DAppearanceNode(jobject vrmlNode);
	CX3DNodeType getNodeType() { return APPEARANCE_NODE; }

	SFNode *getMaterial() { return &m_material; }
	SFNode *getTexture() { return &m_texture; }
	SFNode *getTextureTransform() { return &m_textureTransform; }
	SFNode *getPointProperties() { return &m_pointProperties; }
	SFNode *getLineProperties() { return &m_lineProperties; }
	SFNode *getFillProperties() { return &m_fillProperties; }
	SFNode *getTextureProperties() { return &m_textureProperties; }

	void print(int indent=0);

private:
	SFNode m_material;
	SFNode m_texture;
	SFNode m_textureTransform;
	SFNode m_pointProperties;
	SFNode m_lineProperties;
	SFNode m_fillProperties;
	SFNode m_textureProperties;
};

#endif