#include "SolidDiffuseMaterial.h"
#include "../MaterialProperties.h"
#include "../ShaderProgram.h"

SolidDiffuseMaterial::SolidDiffuseMaterial() {
	shader = new ShaderProgram("solid_diffuse");
}

void SolidDiffuseMaterial::use() {
	shader->use();
}

void SolidDiffuseMaterial::unuse() {
	shader->unuse();
}

void SolidDiffuseMaterial::setUniforms(MaterialProperties* properties) {
	glUniform3f(shader->getUniformLocation("diffuseColor"), properties->diffuse[0], properties->diffuse[1], properties->diffuse[2]);

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