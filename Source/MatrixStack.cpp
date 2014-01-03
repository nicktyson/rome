#include <vector>
#include "MatrixStack.h"
#include "../Lib/glm/glm.hpp"
#include "../Lib/glm/gtc/matrix_transform.hpp"
#include "../Lib/glm/gtx/matrix_query.hpp"

MatrixStack::MatrixStack() {
	stack.resize(1);
	stack.front() = glm::mat4();
}

void MatrixStack::pushMatrix() {
	glm::mat4 newMatrix(stack.back());
	stack.push_back(newMatrix);
}

void MatrixStack::popMatrix() {
	if(stack.size() > 1) {
		stack.pop_back();
	}
}

void MatrixStack::rotated(double angle, double x, double y, double z) {
	stack.back() = glm::rotate(stack.back(), (float)angle, glm::vec3(x,y,z));
}

void MatrixStack::translated(double x, double y, double z) {
	stack.back() = glm::translate(stack.back(), glm::vec3(x,y,z));
}

void MatrixStack::scaled(double x, double y, double z) {
	stack.back() = glm::scale(stack.back(), glm::vec3(x,y,z));
}

void MatrixStack::loadIdentity() {
	if (stack.size() > 0) {
		stack.back() = glm::mat4();
	}
}

void MatrixStack::perspective(float fov, float aspect, float zNear, float zFar) {
	stack.back() = stack.back() * glm::perspective(fov, aspect, zNear, zFar);
}

glm::mat4 MatrixStack::last() {
	return stack.back();
}