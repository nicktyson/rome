#ifndef SOLIDDIFFUSEMATERIAL_H
#define SOLIDDIFFUSEMATERIAL_H

#include "Material.h"

class MaterialProperties;

class SolidDiffuseMaterial : public Material {
public:
	SolidDiffuseMaterial();
	void use();
	void unuse();
	void setUniforms(MaterialProperties* properties);
};

#endif