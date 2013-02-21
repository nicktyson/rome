#include "LambertianMaterial.h"
#include "../ShaderProgram.h"

LambertianMaterial::LambertianMaterial() {
	shader = new ShaderProgram("lambertian");
}

void LambertianMaterial::use() {
	shader->use();
}

void LambertianMaterial::unuse() {
	shader->unuse();
}