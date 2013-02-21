#include <vector>
#include <string>
#include "MaterialList.h"
#include "Material.h"
#include "GreenTestMaterial.h"
#include "NormalMaterial.h"
#include "LambertianMaterial.h"

MaterialList::MaterialList() {
	numMaterials = 0;
	initMaterials();
}

void MaterialList::initMaterials() {
	list.insert(std::make_pair(NORMAL, new NormalMaterial()));
	list.insert(std::make_pair(GREENTEST, new GreenTestMaterial()));
	list.insert(std::make_pair(LAMBERTIAN, new LambertianMaterial()));
	numMaterials = 3;
}

Material* MaterialList::getMaterial(Materials materialName) {
	if (materialName >= numMaterials) {
		return list[0];
	} else {
		return list[materialName];
	}
}