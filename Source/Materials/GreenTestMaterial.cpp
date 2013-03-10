#include "GreenTestMaterial.h"
#include "../ShaderProgram.h"
#include "../MatrixStack.h"
#include "../MaterialProperties.h"

GreenTestMaterial::GreenTestMaterial() {
	shader = new ShaderProgram("green_test");
}

void GreenTestMaterial::use() {
	shader->use();
}

void GreenTestMaterial::unuse() {
	shader->unuse();
}

void GreenTestMaterial::setUniforms(MaterialProperties* properties) {

}