#ifndef MESHNODE_H
#define MESHNODE_H

#include "scene_node.h"
#include "mesh.h"
#include "Materials\MaterialList.h"

class Material;

class MeshNode : public scene_node {
public:
	MeshNode();
	MeshNode(std::string fileLocation);
	MeshNode(std::string fileLocation, MaterialList::Materials materialType);
	mesh* getMesh();
	void setMesh(mesh*);
	void setMaterial(Material* newMaterial);
	virtual void draw();
	virtual void update(double deltaT);
	
protected:
	mesh* objectMesh;
	Material* material;
};

#endif