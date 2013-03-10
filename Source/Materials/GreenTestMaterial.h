#ifndef GREENTESTMATERIAL_H
#define GREENTESTMATERIAL_H

#include "Material.h"

class MaterialProperties;

class GreenTestMaterial : public Material {
public:
	GreenTestMaterial();
	void use();
	void unuse();
	void setUniforms(MaterialProperties* properties);
};


#endif