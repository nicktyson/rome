#include <vector>
#include "MatrixStack.h"
#include "../Lib/glm/glm.hpp"
#include "../Lib/glm/gtc/matrix_transform.hpp"

MatrixStack::MatrixStack() {
	stack.resize(1);
	stack.front() = glm::dmat4();
}
void MatrixStack::pushMatrix() {
	glm::dmat4 newMatrix(stack.back());
	stack.push_back(newMatrix);
}

void MatrixStack::popMatrix() {
	if(stack.size() > 1) {
		stack.pop_back();
	}
}

void MatrixStack::rotated(double angle, double x, double y, double z) {
	stack.back() = glm::rotate(stack.back(), angle, glm::dvec3(x,y,z));
}

void MatrixStack::translated(double x, double y, double z) {
	stack.back() = glm::translate(stack.back(), glm::dvec3(x,y,z));
}

void MatrixStack::scaled(double x, double y, double z) {
	stack.back() = glm::scale(stack.back(), glm::dvec3(x,y,z));
}

void MatrixStack::loadIdentity() {
	if (stack.size() > 0) {
		stack.back() = glm::dmat4();
	}
}