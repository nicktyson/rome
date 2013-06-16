#include "MaterialProperties.h"
#include "TextureManager.h"

MaterialProperties::MaterialProperties() {
	diffuse.resize(3);
	diffuse[0] = 1.0;
	diffuse[1] = 1.0;
	diffuse[2] = 1.0;
	specular.resize(3);
	specular[0] = 1.0;
	specular[1] = 1.0;
	specular[2] = 1.0;
	blinnPhongExponent = 40.0;
	cookTorrM = 1.0;
	cookTorrN = 1.0;
	hasDiffuseTexture = 0;
	hasNormalMap = 0;
}

///set the diffuse texture
void MaterialProperties::setTexture(std::string diffuseTextureName) {
	diffuseTexture = TextureManager::getTexture(diffuseTextureName);
	if(diffuseTexture == -1) {
		hasDiffuseTexture = 0;
	} else {
		hasDiffuseTexture = 1;
	}
}

///set the normal map
void MaterialProperties::setNormalMap(std::string normalMapName) {
	normalMap = TextureManager::getTexture(normalMapName);
	if (normalMap == -1) {
		hasNormalMap = 0;
	} else {
		hasNormalMap = 1;
	}
}
