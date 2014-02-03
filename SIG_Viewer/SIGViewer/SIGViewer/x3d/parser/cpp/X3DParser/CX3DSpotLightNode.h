#ifndef _CX3DSPOTLIGHTNODE_H_
#define _CX3DSPOTLIGHTNODE_H_

#include "CX3DNode.h"

class CX3DSpotLightNode : public CX3DNode
{
public:
	CX3DSpotLightNode(jobject vrmlNode);
	CX3DNodeType getNodeType() { return SPOT_LIGHT_NODE; }

	SFFloat *getAmbientIntensity() { return &m_ambientIntensity; }
	SFColor *getColor() { return &m_color; }
	SFFloat *getIntensity() { return &m_intensity; }
	SFVec3f *getDirection() { return &m_direction; }
	SFVec3f *getLocation() { return &m_location; }
	SFFloat *getRadius() { return &m_radius; }
	SFVec3f *getAttenuation() { return &m_attenuation; }
	SFFloat *getBeamWidth() { return &m_beamWidth; }
	SFFloat *getCutOffAngle() { return &m_cutOffAngle; }
	SFBool *getOn() { return &m_on; }
	SFBool *getGlobal() { return &m_global; }

	void print(int indent=0);

private:
	SFFloat m_ambientIntensity;
	SFColor m_color;
	SFFloat m_intensity;
	SFVec3f m_direction;
	SFVec3f m_location;
	SFFloat m_radius;
	SFVec3f m_attenuation;
	SFFloat m_beamWidth;
	SFFloat m_cutOffAngle;
	SFBool m_on;
	SFBool m_global;
};

#endif
