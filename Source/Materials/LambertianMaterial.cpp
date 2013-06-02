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
	//diffuse color
	glUniform3f(3, properties->diffuse[0], properties->diffuse[1], properties->diffuse[2]);

	glUniform1i(shader->getUniformLocation("hasDiffuseTexture"), properties->hasDiffuseTexture);
	glUniform1i(shader->getUniformLocation("hasNormalMap"), properties->hasNormalMap);

	if(properties->hasDiffuseTexture == 1) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, properties->diffuseTexture);
		glUniform1i(shader->getUniformLocation("diffuseTexture"), 0);
	}

	if(properties->hasNormalMap == 1) {
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, properties->normalMap);
		glUniform1i(shader->getUniformLocation("normalMap"), 1);
	}
}