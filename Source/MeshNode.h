#ifndef MESHNODE_H
#define MESHNODE_H

#include "scene_node.h"
#include "Materials\MaterialList.h"

class Material;
class Renderer;
class mesh;
class MaterialProperties;

class MeshNode : public scene_node {
public:
	MeshNode();
	MeshNode(std::string fileLocation);
	MeshNode(std::string fileLocation, MaterialList::Materials materialType);
	mesh* getMesh();
	void setMesh(mesh*);
	void setMaterial(MaterialList::Materials materialType);
	virtual void draw(Renderer* r);
	virtual void update(double deltaT);
	
protected:
	mesh* objectMesh;
	Material* material;
	MaterialProperties* properties;
};

#endif