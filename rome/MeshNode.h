#ifndef MESHNODE_H
#define MESHNODE_H

#include "scene_node.h"
#include "mesh.h"

class MeshNode : public scene_node {
public:
	MeshNode();
	mesh * getMesh();
	void setMesh(mesh *);
	void draw();
	
private:
	mesh * objectMesh;
};

#endif