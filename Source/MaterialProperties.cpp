#include "MaterialProperties.h"

MaterialProperties::MaterialProperties() {
	diffuse.resize(3);
	diffuse[0] = 0.2;
	diffuse[1] = 0.2;
	diffuse[2] = 0.2;
}

MaterialProperties::MaterialProperties(float r, float g, float b) {
	diffuse.resize(3);
	diffuse[0] = r;
	diffuse[1] = g;
	diffuse[2] = b;
}

MaterialProperties::MaterialProperties(float r, float g, float b, float bpExp, float ctM) {
	diffuse.resize(3);
	diffuse[0] = r;
	diffuse[1] = g;
	diffuse[2] = b;
	blinnPhongExponent = bpExp;
	cookTorrM = ctM;
}