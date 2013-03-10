#ifndef MATERIALPROPERTIES_H
#define MATERIALPROPERTIES_H

#include <vector>

class MaterialProperties {
public:
	MaterialProperties();
	std::vector<float> diffuse;
	float blinnPhongExponent;
	float cookTorrM;
};


#endif