#ifndef MESHNODE_H
#define MESHNODE_H

#include <string>
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
	void setMaterialProperties(float r, float g, float b, float bpExp, float ctM);
	void setMaterialProperties(float r, float g, float b, float bpExp, float ctM, std::string diffuseTextureName);
	void setMaterialProperties(std::string normalMapName);
	virtual void draw(Renderer* r);
	virtual void update(double deltaT);
	
protected:
	mesh* objectMesh;
	Material* material;
	MaterialProperties* properties;
};

#endif