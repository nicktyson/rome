#include "GreenTestMaterial.h"
#include "../ShaderProgram.h"

GreenTestMaterial::GreenTestMaterial() {
	shader = new ShaderProgram("green_test");
}

void GreenTestMaterial::use() {
	shader->use();
}

void GreenTestMaterial::unuse() {
	shader->unuse();
}