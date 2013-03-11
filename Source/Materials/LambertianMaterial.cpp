#include "LambertianMaterial.h"
#include "../MaterialProperties.h"
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

void LambertianMaterial::setUniforms(MaterialProperties* properties) {
	glUniform3f(3, properties->diffuse[0], properties->diffuse[1], properties->diffuse[2]);
}