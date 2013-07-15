#ifndef SOLIDTRANSPARENTMATERIAL_H
#define SOLIDTRANSPARENTMATERIAL_H

#include "Material.h"

class MaterialProperties;

class SolidTransparentMaterial : public Material {
public:
	SolidTransparentMaterial();
	void use();
	void unuse();
	void setUniforms(MaterialProperties* properties);
};

#endif