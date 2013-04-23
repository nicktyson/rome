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

std::vector<float>& mesh::getTexCoords() {
	return texCoords;
}

void mesh::loadMesh(std::string fileLocation) {
	//assumes pattern:
	//vert count, tri count
	//vertices, triangles, normals, texcoords
	std::ifstream fileStream(fileLocation);

	fileStream >> vertCount;
	fileStream >> triCount;

	triangles.resize(3 * triCount);
	normals.resize(3 * vertCount);
	vertices.resize(3 * vertCount);
	texCoords.resize(2 * vertCount);

	std::string trash;
	fileStream >> trash;

	//read and store vertex positions
	for(int i = 0; i < 3*vertCount; i++) {
		fileStream >> vertices[i];
	}

	fileStream >> trash;

	//read and store triangles (vertex indices)
	for(int j = 0; j < 3*triCount; j++) {
		fileStream >> triangles[j];
	}

	fileStream >> trash;

	//read and store normals
	for(int k = 0; k < 3*vertCount; k++) {
		fileStream >> normals[k];
	}

	fileStream >> trash;

	//read and store texture coordinates
	for(int l = 0; l < 2*vertCount; l++) {
		fileStream >> texCoords[l];
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
	// combine vertices, normals, and texture coordinates into one array to send to VBO
	std::vector<float> bufferData;
	bufferData.resize(8*vertCount);

	for (int i = 0; i < vertCount; i++) {
		bufferData[8*i] = vertices[3*i];
		bufferData[8*i + 1] = vertices[3*i + 1];
		bufferData[8*i + 2] = vertices[3*i + 2];

		bufferData[8*i + 3] = normals[3*i];
		bufferData[8*i + 4] = normals[3*i + 1];
		bufferData[8*i + 5] = normals[3*i + 2];

		bufferData[8*i + 6] = texCoords[2*i];
		bufferData[8*i + 7] = texCoords[2*i + 1];
	}

	//generate and fill vertex data and index buffers
	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &indexBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8 * vertCount, &bufferData[0], GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * triCount * 3, &triangles[0], GL_STATIC_DRAW);

	//set up VAO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), NULL);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid*)(6*sizeof(GLfloat)));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}

void mesh::bindBuffers() {
	glBindVertexArray(vao);
}

void mesh::unbindBuffers() {
	glBindVertexArray(0);
}