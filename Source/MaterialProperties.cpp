#include "MaterialProperties.h"
#include "TextureManager.h"

MaterialProperties::MaterialProperties() {
	diffuse.resize(3);
	diffuse[0] = 0.6;
	diffuse[1] = 0.6;
	diffuse[2] = 0.6;
	blinnPhongExponent = 5.0;
	hasDiffuseTexture = 0;
}

MaterialProperties::MaterialProperties(float r, float g, float b) {
	diffuse.resize(3);
	diffuse[0] = r;
	diffuse[1] = g;
	diffuse[2] = b;
	blinnPhongExponent = 5.0;
	hasDiffuseTexture = 0;
}

MaterialProperties::MaterialProperties(float r, float g, float b, float bpExp, float ctM) {
	diffuse.resize(3);
	diffuse[0] = r;
	diffuse[1] = g;
	diffuse[2] = b;
	blinnPhongExponent = bpExp;
	cookTorrM = ctM;
	hasDiffuseTexture = 0;
}

MaterialProperties::MaterialProperties(float r, float g, float b, float bpExp, float ctM, std::string diffuseTextureName) {
	diffuse.resize(3);
	diffuse[0] = r;
	diffuse[1] = g;
	diffuse[2] = b;
	blinnPhongExponent = bpExp;
	cookTorrM = ctM;

	diffuseTexture = TextureManager::getTexture(diffuseTextureName);
	if(diffuseTexture == -1) {
		hasDiffuseTexture = 0;
	} else {
		hasDiffuseTexture = 1;
	}
}

void MaterialProperties::setTexture(std::string diffuseTextureName) {
	diffuseTexture = TextureManager::getTexture(diffuseTextureName);
	if(diffuseTexture == -1) {
		hasDiffuseTexture = 0;
	} else {
		hasDiffuseTexture = 1;
	}
}
