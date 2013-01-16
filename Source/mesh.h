#ifndef MESH_H
#define MESH_H

#include <string>
#include <vector>

class mesh {
public:
	mesh();
	void loadMesh(std::string fileLocation);
	std::vector<int>& getTriangles();
	std::vector<float>& getNormals();
	std::vector<float>& getVertices();

private:
	std::vector<int> triangles;
	std::vector<float> normals;
	std::vector<float> vertices;
};

#endif