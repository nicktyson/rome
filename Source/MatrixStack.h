#ifndef MATRIXSTACK_H
#define MATRIXSTACK_H

//Uses GLM to replicate the matrix operations removed from OpenGL 3

#include <vector>
#include "../Lib/glm/glm.hpp"
#include "../Lib/glm/gtc/matrix_transform.hpp"

class MatrixStack {
public:
	MatrixStack();
	void pushMatrix();
	void popMatrix();
	void rotated(double angle, double x, double y, double z);
	void translated(double x, double y, double z);
	void scaled(double x, double y, double z);
	void loadIdentity();
	void perspective(float fov, float aspect, float zNear, float zFar);
	glm::mat4 last();

private:
	std::vector<glm::mat4> stack;
};

#endif