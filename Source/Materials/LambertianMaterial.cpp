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
	glUniform1i(shader->getUniformLocation("hasDiffuseTexture"), properties->hasDiffuseTexture);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, properties->diffuseTexture);
	glUniform1i(shader->getUniformLocation("diffuseTexture"), 0);
}