#ifndef NORMALMATERIAL_H
#define NORMALMATERIAL_H

#include "Material.h"

class NormalMaterial : public Material {
public:
	NormalMaterial();
	void use();
	void unuse();
};


#endif