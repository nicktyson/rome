#include "SolidTransparentMaterial.h"
#include "../MaterialProperties.h"
#include "../ShaderProgram.h"

SolidTransparentMaterial::SolidTransparentMaterial() {
	shader = new ShaderProgram("solid_transparent");
	hasTransparency = true;
}

void SolidTransparentMaterial::use() {
	shader->use();
}

void SolidTransparentMaterial::unuse() {
	shader->unuse();
}

void SolidTransparentMaterial::setUniforms(MaterialProperties* properties) {
	glUniform3f(shader->getUniformLocation("diffuseColor"), properties->diffuse[0], properties->diffuse[1], properties->diffuse[2]);
	glUniform1f(shader->getUniformLocation("alpha"), properties->alpha);

	glUniform1i(shader->getUniformLocation("hasDiffuseTexture"), properties->hasDiffuseTexture);

	if(properties->hasDiffuseTexture == 1) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, properties->diffuseTexture);
		glUniform1i(shader->getUniformLocation("diffuseTexture"), 0);
	}
}