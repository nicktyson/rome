#ifndef LAMBERTIANMATERIAL_H
#define LAMBERTIANMATERIAL_H

#include "Material.h"

class MaterialProperties;

class LambertianMaterial : public Material {
public:
	LambertianMaterial();
	void use();
	void unuse();
	void setUniforms(MaterialProperties* properties);
};

#endif