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
	// combine vertices and normals into one array to send to VBO
	float* bufferData = new float[6*vertCount];

	for (int i = 0; i < vertCount; i++) {
		bufferData[3*i] = vertices[3*i];
		bufferData[3*i + 1] = vertices[3*i + 1];
		bufferData[3*i + 2] = vertices[3*i + 2];

		bufferData[3*i + 3] = normals[3*i];
		bufferData[3*i + 4] = normals[3*i + 1];
		bufferData[3*i + 5] = normals[3*i + 2];
	}

	//generate and fill vertex data and index buffers
	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &indexBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * vertCount, bufferData, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * triCount * 3, &triangles[0], GL_STATIC_DRAW);

	//set up VAO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), NULL);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

void mesh::bindBuffers() {
	glBindVertexArray(vao);
}

void mesh::unbindBuffers() {
	glBindVertexArray(0);
}