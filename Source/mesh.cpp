#include <gl\glew.h>
#include "mesh.h"
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include "../Lib/glm/glm.hpp"

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

	calculateTangents();
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
	//12 = floats per vert
	bufferData.resize(12*vertCount);

	for (int i = 0; i < vertCount; i++) {
		bufferData[12*i] = vertices[3*i];
		bufferData[12*i + 1] = vertices[3*i + 1];
		bufferData[12*i + 2] = vertices[3*i + 2];

		bufferData[12*i + 3] = normals[3*i];
		bufferData[12*i + 4] = normals[3*i + 1];
		bufferData[12*i + 5] = normals[3*i + 2];

		bufferData[12*i + 6] = texCoords[2*i];
		bufferData[12*i + 7] = texCoords[2*i + 1];

		bufferData[12*i + 8] = tangents[4*i];
		bufferData[12*i + 9] = tangents[4*i + 1];
		bufferData[12*i + 10] = tangents[4*i + 2];
		bufferData[12*i + 11] = tangents[4*i + 3];
	}

	//generate and fill vertex data and index buffers
	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &indexBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12 * vertCount, &bufferData[0], GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * triCount * 3, &triangles[0], GL_STATIC_DRAW);

	//set up VAO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12*sizeof(GLfloat), NULL);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 12*sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 12*sizeof(GLfloat), (GLvoid*)(6*sizeof(GLfloat)));
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 12*sizeof(GLfloat), (GLvoid*)(8*sizeof(GLfloat)));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	glBindVertexArray(0);
}

void mesh::bindBuffers() {
	glBindVertexArray(vao);
}

void mesh::unbindBuffers() {
	glBindVertexArray(0);
}

void mesh::calculateTangents() {
	std::vector<float> tans(3*vertCount, 0);
	std::vector<float> bitans(3*vertCount, 0);
	tangents.resize(4 * vertCount);

	//calculate tentative tangents and bitangents for each triangle
	for (int i = 0; i < triCount; i++) {
		//find the vertices of the current triangle, and their UV coords
		int vi1 = triangles[3*i];
		int vi2 = triangles[3*i + 1];
		int vi3 = triangles[3*i + 2];

		glm::vec3 vert0(vertices[3*vi1], vertices[3*vi1 + 1], vertices[3*vi1 + 2]);
		glm::vec3 vert1(vertices[3*vi2], vertices[3*vi2 + 1], vertices[3*vi2 + 2]);
		glm::vec3 vert2(vertices[3*vi3], vertices[3*vi3 + 1], vertices[3*vi3 + 2]);

		glm::vec2 uv0(texCoords[2*vi1], texCoords[2*vi1 + 1]);
		glm::vec2 uv1(texCoords[2*vi2], texCoords[2*vi2 + 1]);
		glm::vec2 uv2(texCoords[2*vi3], texCoords[2*vi3 + 1]);

		//differences in position and UV coords
		glm::vec3 dPos1 = vert1 - vert0;
		glm::vec3 dPos2 = vert2 - vert0;

		glm::vec2 dUV1 = uv1 - uv0;
		glm::vec2 dUV2 = uv2 - uv0;

		//calculate and store the tangent and bitangent
		float coeff = 1.0f / (dUV1.x * dUV2.y - dUV1.y * dUV2.x);

		glm::vec3 tan = dPos1 * dUV2.y - dPos2 * dUV1.y;
		glm::vec3 bitan = dPos2 * dUV1.x  - dPos1 * dUV2.x;
		tan *= coeff;
		bitan *= coeff;

		tans[3*vi1] += tan.x;
		tans[3*vi1 + 1] += tan.y;
		tans[3*vi1 + 2] += tan.z;

		tans[3*vi2] += tan.x;
		tans[3*vi2 + 1] += tan.y;
		tans[3*vi2 + 2] += tan.z;

		tans[3*vi3] += tan.x;
		tans[3*vi3 + 1] += tan.y;
		tans[3*vi3 + 2] += tan.z;

		bitans[3*vi1] += bitan.x;
		bitans[3*vi1 + 1] += bitan.y;
		bitans[3*vi1 + 2] += bitan.z;

		bitans[3*vi2] += bitan.x;
		bitans[3*vi2 + 1] += bitan.y;
		bitans[3*vi2 + 2] += bitan.z;

		bitans[3*vi3] += bitan.x;
		bitans[3*vi3 + 1] += bitan.y;
		bitans[3*vi3 + 2] += bitan.z;
	}

	//find the final tangent (and bitangent) for each vertex
	for (int j = 0; j < vertCount; j++) {
		glm::vec3 normal (normals[3*j], normals[3*j + 1], normals[3*j + 2]);
		glm::vec3 tangent (tans[3*j], tans[3*j + 1], tans[3*j + 2]);
		glm::vec3 bitangent (bitans[3*j], bitans[3*j + 1], bitans[3*j + 2]);

		glm::normalize(tangent);
		glm::normalize(bitangent);

		//orthagonalize
		glm::vec3 tangent_orth(normal);
		tangent_orth *= glm::dot(normal, tangent);
		tangent_orth = tangent - tangent_orth;
		glm::normalize(tangent_orth);

		//compute handedness
		float handedness = 1.0f;
		glm::vec3 nCrossT = glm::cross(normal, tangent_orth);
		
		if(glm::dot(nCrossT, bitangent) > 0) {
			handedness = 1.0f;
		} else {
			handedness = -1.0f;
		}

		//store the orthagonalized tangent and handedness
		tangents[4*j] = tangent_orth.x;
		tangents[4*j + 1] = tangent_orth.y;
		tangents[4*j + 2] = tangent_orth.z;
		tangents[4*j + 3] = handedness;
	}

}