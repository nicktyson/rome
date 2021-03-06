#include <vector>
#include <string>
#include "MaterialList.h"
#include "Material.h"
#include "GreenTestMaterial.h"
#include "NormalMaterial.h"
#include "LambertianMaterial.h"
#include "BlinnPhongMaterial.h"
#include "CookTorrMaterial.h"
#include "SolidTransparentMaterial.h"
#include "SolidDiffuseMaterial.h"

MaterialList::MaterialList() {
	numMaterials = 0;
	initMaterials();
}

void MaterialList::initMaterials() {
	list.insert(std::make_pair(NORMAL, new NormalMaterial()));
	list.insert(std::make_pair(GREENTEST, new GreenTestMaterial()));
	list.insert(std::make_pair(LAMBERTIAN, new LambertianMaterial()));
	list.insert(std::make_pair(BLINNPHONG, new BlinnPhongMaterial()));
	list.insert(std::make_pair(COOKTORR, new CookTorrMaterial()));
	list.insert(std::make_pair(SOLIDTRANSPARENT, new SolidTransparentMaterial()));
	list.insert(std::make_pair(SOLIDDIFFUSE, new SolidDiffuseMaterial()));
	numMaterials = 7;
}

Material* MaterialList::getMaterial(Materials materialName) {
	if (materialName >= numMaterials) {
		return list[0];
	} else {
		return list[materialName];
	}
}