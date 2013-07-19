#include "Material.h"
#include "../ShaderProgram.h"

Material::Material() {
	hasTransparency = false;
}

bool Material::getHasTransparency() {
	return hasTransparency;
}

void Material::setTransparencyUniforms(GLuint opaqueDepthBuffer, GLuint peelDepthBuffer, int passNumber) {
	shader->use();

	glUniform1i(shader->getUniformLocation("passNumber"), passNumber);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_RECTANGLE, opaqueDepthBuffer);
	glUniform1i(shader->getUniformLocation("opaqueDepthBuffer"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_RECTANGLE, peelDepthBuffer);
	glUniform1i(shader->getUniformLocation("peelDepthBuffer"), 1);

	shader->unuse();
}