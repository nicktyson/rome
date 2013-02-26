#include <vector>
#include "ScreenQuad.h"

ScreenQuad::ScreenQuad() {
	vbo = -1;
	vao = -1;
	indexBuffer = -1;
	init();
}

void ScreenQuad::init() {
	std::vector<float> coords;
	coords.resize(12);
	std::vector<int> indices;
	indices.resize(6);

	//bottom-left vertex
	coords[0] = -0.9;
	coords[1] = -1.0;
	coords[2] = 0.0;
	//bottom-right
	coords[3] = 1.0;
	coords[4] = -1.0;
	coords[5] = 0.0;
	//top-right
	coords[6] = 1.0;
	coords[7] = 1.0;
	coords[8] = 0.0;
	//top-left
	coords[9] = -1.0;
	coords[10] = 1.0;
	coords[11] = 0.0;

	//triangle indices
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 0;
	indices[4] = 2;
	indices[5] = 3;

	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &indexBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, &coords[0], GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * 6, &indices[0], GL_STATIC_DRAW);

	//set up VAO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), NULL);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}

void ScreenQuad::draw() {
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
	glBindVertexArray(0);
}