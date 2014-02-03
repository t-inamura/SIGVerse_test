#ifndef _CX3DDIRECTIONALLIGHTNODE_H_
#define _CX3DDIRECTIONALLIGHTNODE_H_

#include "CX3DNode.h"

class CX3DDirectionalLightNode : public CX3DNode
{
public:
	CX3DDirectionalLightNode(jobject vrmlNode);
	CX3DNodeType getNodeType() { return DIRECTIONAL_LIGHT_NODE; }

	SFFloat *getAmbientIntensity() { return &m_ambientIntensity; }
	SFColor *getColor() { return &m_color; }
	SFVec3f *getDirection() { return &m_direction; }
	SFFloat *getIntensity() { return &m_intensity; }
	SFBool *getOn() { return &m_on; }
	SFBool *getGlobal() { return &m_global; }

	void print(int indent=0);

private:
	SFFloat m_ambientIntensity;
	SFColor m_color;
	SFVec3f m_direction;
	SFFloat m_intensity;
	SFBool m_on;
	SFBool m_global;
};

#endif
