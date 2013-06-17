#include "CookTorrMaterial.h"
#include "../MaterialProperties.h"
#include "../ShaderProgram.h"

CookTorrMaterial::CookTorrMaterial() {
	shader = new ShaderProgram("cook_torr");
}

void CookTorrMaterial::use() {
	shader->use();
}

void CookTorrMaterial::unuse() {
	shader->unuse();
}

void CookTorrMaterial::setUniforms(MaterialProperties* properties) {
	glUniform3f(shader->getUniformLocation("diffuseColor"), properties->diffuse[0], properties->diffuse[1], properties->diffuse[2]);
	glUniform3f(shader->getUniformLocation("specularColor"), properties->specular[0], properties->specular[1], properties->specular[2]);
	glUniform1f(shader->getUniformLocation("ctM"), properties->cookTorrM);
	glUniform1f(shader->getUniformLocation("ctN"), properties->cookTorrN);

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