#include "NormalMaterial.h"
#include "../ShaderProgram.h"
#include "../MaterialProperties.h"

NormalMaterial::NormalMaterial() {
	shader = new ShaderProgram("normal");
}

void NormalMaterial::use() {
	shader->use();
}

void NormalMaterial::unuse() {
	shader->unuse();
}

void NormalMaterial::setUniforms(MaterialProperties* properties) {
	
}