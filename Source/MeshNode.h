#ifndef MESHNODE_H
#define MESHNODE_H

#include "scene_node.h"
#include "mesh.h"

class MeshNode : public scene_node {
public:
	MeshNode();
	MeshNode(std::string fileLocation);
	mesh * getMesh();
	void setMesh(mesh *);
	virtual void draw();
	
protected:
	mesh * objectMesh;
};

#endif