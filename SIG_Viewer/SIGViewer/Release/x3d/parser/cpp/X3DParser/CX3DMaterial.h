namespace IX3D
{

class Material : public Node
{
public:
	SFFloat getAmbientIntensity();
	SFColor getDiffuseColor();
	SFColor getEmissiveColor();
	SFFloat getShininess();
	SFColor getSpecularColor();
	SFFloat getTransparency();
};

};
