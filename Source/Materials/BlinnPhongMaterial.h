#ifndef BLINNPHONGMATERIAL_H
#define BLINNPHONGMATERIAL_H

#include "Material.h"

class MaterialProperties;

class BlinnPhongMaterial : public Material {
public:
	BlinnPhongMaterial();
	void use();
	void unuse();
	void setUniforms(MaterialProperties* properties);
};

#endif