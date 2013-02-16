#ifndef GREENTESTMATERIAL_H
#define GREENTESTMATERIAL_H

#include "Material.h"

class GreenTestMaterial : public Material {
public:
	GreenTestMaterial();
	void use();
	void unuse();
};


#endif