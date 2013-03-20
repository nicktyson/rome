#include "BlinnPhongMaterial.h"
#include "../MaterialProperties.h"
#include "../ShaderProgram.h"

BlinnPhongMaterial::BlinnPhongMaterial() {
	shader = new ShaderProgram("blinn_phong");
}

void BlinnPhongMaterial::use() {
	shader->use();
}

void BlinnPhongMaterial::unuse() {
	shader->unuse();
}

void BlinnPhongMaterial::setUniforms(MaterialProperties* properties) {
	glUniform3f(3, properties->diffuse[0], properties->diffuse[1], properties->diffuse[2]);
	glUniform1f(4, properties->blinnPhongExponent);
}