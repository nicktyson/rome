#include "NormalMaterial.h"
#include "../ShaderProgram.h"

NormalMaterial::NormalMaterial() {
	shader = new ShaderProgram("normal");
}

void NormalMaterial::use() {
	shader->use();
}

void NormalMaterial::unuse() {
	shader->unuse();
}