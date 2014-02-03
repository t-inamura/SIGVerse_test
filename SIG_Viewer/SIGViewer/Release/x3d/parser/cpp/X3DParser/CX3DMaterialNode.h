#ifndef _CX3DMATERIALNODE_H_
#define _CX3DMATERIALNODE_H_

#include "CX3DNode.h"

class CX3DMaterialNode : public CX3DNode
{
public:
	CX3DMaterialNode(jobject vrmlNode);
	CX3DNodeType getNodeType() { return MATERIAL_NODE; }

	SFColor *getDiffuseColor() { return &m_diffuseColor; }
	SFColor *getSpecularColor() { return &m_specularColor; }
	SFColor *getEmissiveColor() { return &m_emissiveColor; }
	SFFloat *getShininess() { return &m_shininess; }
	SFFloat *getTransparency() { return &m_transparency; }
	SFFloat *getAmbientIntensity() { return &m_ambientIntensity; }

	bool isTransparency(){return m_isTrans;}
	void print(int indent=0);

private:
	SFColor m_diffuseColor;
	SFColor m_specularColor;
	SFColor m_emissiveColor;
	SFFloat m_shininess;
	SFFloat m_transparency;
	SFFloat m_ambientIntensity;
	bool m_isTrans;
};

#endif

