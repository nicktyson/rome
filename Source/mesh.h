#ifndef MESH_H
#define MESH_H

#include <gl\glew.h>
#include <string>
#include <vector>

class mesh {
public:
	mesh();
	mesh(std::string meshLocation);
	void loadMesh(std::string fileLocation);
	std::vector<int>& getTriangles();
	std::vector<float>& getNormals();
	std::vector<float>& getVertices();
	std::vector<float>& getTexCoords();
	int getVertCount();
	int getTriCount();
	void setupBuffers();
	void bindBuffers();
	void unbindBuffers();

private:
	void calculateTangents();
	std::vector<int> triangles;
	std::vector<float> normals;
	std::vector<float> vertices;
	std::vector<float> texCoords;
	std::vector<float> tangents;
	int vertCount;
	int triCount;
	GLuint vbo;
	GLuint vao;
	GLuint indexBuffer;
};

#endif