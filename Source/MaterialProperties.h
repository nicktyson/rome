#ifndef MATERIALPROPERTIES_H
#define MATERIALPROPERTIES_H

#include <gl/glew.h>
#include <vector>
#include <string>

class MaterialProperties {
public:
	MaterialProperties();
	void setTexture(std::string diffuseTextureName);
	void setNormalMap(std::string normalMapName);

	std::vector<float> diffuse;
	std::vector<float> specular;
	float blinnPhongExponent;
	float cookTorrM;
	float cookTorrN;
	GLuint diffuseTexture;
	int hasDiffuseTexture;
	GLuint normalMap;
	int hasNormalMap;
};

#endif