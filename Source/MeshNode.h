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
	void setMaterialColor(float r, float g, float b);
	void setMaterialColor(float rD, float gD, float bD, float rS, float gS, float bS);
	void setMaterialProperties(float bpExp, float ctM, float ctN);
	void setMaterialAlpha(float a);
	void setMaterialTexture(std::string diffuseTextureName);
	void setMaterialNormalMap(std::string normalMapName);

	virtual void draw(Renderer* r, bool isTransparentPass);
	virtual void update(double deltaT);
	virtual void stateUpdate(double deltaT);
	
protected:
	struct mn_State {
		mesh* objectMesh;
		Material* material;
		MaterialProperties* properties;
	};

	std::vector<mn_State> mn_states;
};

#endif