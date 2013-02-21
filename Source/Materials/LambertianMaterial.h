#ifndef LAMBERTIANMATERIAL_H
#define LAMBERTIANMATERIAL_H

#include "Material.h"

class LambertianMaterial : public Material {
public:
	LambertianMaterial();
	void use();
	void unuse();
};

#endif