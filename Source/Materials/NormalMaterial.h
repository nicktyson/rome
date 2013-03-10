#ifndef NORMALMATERIAL_H
#define NORMALMATERIAL_H

#include "Material.h"

class MaterialProperties;

class NormalMaterial : public Material {
public:
	NormalMaterial();
	void use();
	void unuse();
	void setUniforms(MaterialProperties* properties);
};


#endif