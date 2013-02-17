#include <gl\glew.h>
#include "mesh.h"
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

mesh::mesh() {
	vbo = -1;
	vao = -1;
	indexBuffer = -1;
}

mesh::mesh(std::string fileLocation) {
	vbo = -1;
	vao = -1;
	indexBuffer = -1;
	loadMesh(fileLocation);
}

std::vector<float>& mesh::getNormals() {
	return normals;
}

std::vector<float>& mesh::getVertices() {
	return vertices;
}

std::vector<int>& mesh::getTriangles() {
	return triangles;
}

void mesh::loadMesh(std::string fileLocation) {
	//assumes pattern:
	//vert count, tri count
	//vertices, triangles, normals
	std::ifstream fileStream(fileLocation);

	fileStream >> vertCount;
	fileStream >> triCount;

	triangles.resize(3 * triCount);
	normals.resize(3 * vertCount);
	vertices.resize(3 * vertCount);

	std::string trash;
	fileStream >> trash;

	for(int i = 0; i < 3*vertCount; i++) {
		fileStream >> vertices[i];
	}

	fileStream >> trash;

	for(int j = 0; j < 3*triCount; j++) {
		fileStream >> triangles[j];
	}

	fileStream >> trash;

	for(int k = 0; k < 3*vertCount; k++) {
		fileStream >> normals[k];
	}

	fileStream.close();

	setupBuffers();
}

int mesh::getVertCount() {
	return vertCount;
}

int mesh::getTriCount() {
	return triCount;
}

void mesh::setupBuffers() {
	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &indexBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertCount, &vertices[0], GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * triCount * 3, &triangles[0], GL_STATIC_DRAW);


	glBindVertexArray(0);
}