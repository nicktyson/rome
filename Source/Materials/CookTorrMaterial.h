#ifndef COOKTORRMATERIAL_H
#define COOKTORRMATEIAL_H

#include "Material.h"

class MaterialProperties;

class CookTorrMaterial : public Material {
public:
	CookTorrMaterial();
	void use();
	void unuse();
	void setUniforms(MaterialProperties* properties);
};

#endif