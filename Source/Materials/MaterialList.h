#ifndef MATERIALLIST_H
#define MATERIALLIST_H

#include <map>
#include <string>

class Material;

class MaterialList {
public:
	MaterialList();
	enum Materials {
		NORMAL,
		GREENTEST,
		LAMBERTIAN,
		BLINNPHONG,
		COOKTORR,
		SOLIDTRANSPARENT,
		SOLIDDIFFUSE
	};
	Material* getMaterial(Materials materialName);

protected:
	std::map<int, Material*> list;
	void initMaterials();
	int numMaterials;
};


#endif