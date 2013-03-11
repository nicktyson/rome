#ifndef MATERIALPROPERTIES_H
#define MATERIALPROPERTIES_H

#include <vector>

class MaterialProperties {
public:
	MaterialProperties();
	MaterialProperties(float r, float g, float b);
	MaterialProperties(float r, float g, float b, float bpExp, float ctM);
	std::vector<float> diffuse;
	float blinnPhongExponent;
	float cookTorrM;
};


#endif