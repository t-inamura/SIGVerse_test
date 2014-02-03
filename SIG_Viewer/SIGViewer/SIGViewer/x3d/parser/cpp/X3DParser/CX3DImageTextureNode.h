#ifndef _CX3DIMAGETEXTURENODE_H_
#define _CX3DIMAGETEXTURENODE_H_

#include "CX3DNode.h"

class CX3DImageTextureNode : public CX3DNode
{
public:
	CX3DImageTextureNode(jobject vrmlNode);
	CX3DNodeType getNodeType() { return IMAGE_TEXTURE_NODE; }

	MFString *getUrl() { return &m_url; }
	SFBool *getRepeatS() { return &m_repeatS; }
	SFBool *getRepeatT() { return &m_repeatT; }
	SFNode *getTextureProperties() { return &m_textureProperties; }

	void print(int indent=0);

private:
	MFString m_url;
	SFBool m_repeatS;
	SFBool m_repeatT;
	SFNode m_textureProperties;
};

#endif
