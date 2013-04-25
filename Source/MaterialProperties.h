#ifndef MATERIALPROPERTIES_H
#define MATERIALPROPERTIES_H

#include <gl/glew.h>
#include <vector>
#include <string>

class MaterialProperties {
public:
	MaterialProperties();
	MaterialProperties(float r, float g, float b);
	MaterialProperties(float r, float g, float b, float bpExp, float ctM);
	MaterialProperties(float r, float g, float b, float bpExp, float ctM, std::string diffuseTextureName);
	void setTexture(std::string diffuseTextureName);
	std::vector<float> diffuse;
	float blinnPhongExponent;
	float cookTorrM;
	GLuint diffuseTexture;
	int hasDiffuseTexture;
};

#endif