#include "mesh.h"
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

mesh::mesh() {

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

	int numVerts;
	int numTris;
	fileStream >> numVerts;
	fileStream >> numTris;

	triangles.resize(3 * numTris);
	normals.resize(3 * numVerts);
	vertices.resize(3 * numVerts);

	std::string trash;
	fileStream >> trash;

	for(int i = 0; i < 3*numVerts; i++) {
		fileStream >> vertices[i];
	}

	fileStream >> trash;

	for(int j = 0; j < 3*numTris; j++) {
		fileStream >> triangles[j];
	}

	fileStream >> trash;

	for(int k = 0; k < 3*numVerts; k++) {
		fileStream >> normals[k];
	}

	fileStream.close();
}