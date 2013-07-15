#include <GL/glew.h>
#include <iostream>
#include <string>
#include "../Lib/glm/glm.hpp"
#include "../Lib/glm/gtc/matrix_inverse.hpp"
#include "../Lib/glm/gtc/type_ptr.hpp"
#include "MeshNode.h"
#include "Materials\MaterialList.h"
#include "Materials\Material.h"
#include "MatrixStack.h"
#include "mesh.h"
#include "Renderer.h"
#include "MaterialProperties.h"

MeshNode::MeshNode() {
	objectMesh = new mesh();
	setMaterial(MaterialList::NORMAL);
	properties = new MaterialProperties();
}

MeshNode::MeshNode(std::string fileLocation) {
	objectMesh = new mesh(fileLocation);
	setMaterial(MaterialList::NORMAL);
	properties = new MaterialProperties();
}

MeshNode::MeshNode(std::string fileLocation, MaterialList::Materials materialType) {
	objectMesh = new mesh(fileLocation);
	setMaterial(materialType);
	properties = new MaterialProperties();
}

mesh * MeshNode::getMesh() {
	return objectMesh;
}

void MeshNode::setMesh(mesh* newMesh) {
	objectMesh = newMesh;
}

void MeshNode::setMaterial(MaterialList::Materials materialType) {
	extern MaterialList* materialList;
	material = materialList->getMaterial(materialType);
}

void MeshNode::setMaterialColor(float r, float g, float b) {
	properties->diffuse[0] = r;
	properties->diffuse[1] = g;
	properties->diffuse[2] = b;
}

void MeshNode::setMaterialColor(float rD, float gD, float bD, float rS, float gS, float bS) {
	properties->diffuse[0] = rD;
	properties->diffuse[1] = gD;
	properties->diffuse[2] = bD;

	properties->specular[0] = rS;
	properties->specular[1] = gS;
	properties->specular[2] = bS;
}

void MeshNode::setMaterialProperties(float bpExp, float ctM, float ctN) {
	properties->blinnPhongExponent = bpExp;
	properties->cookTorrM = ctM;
	properties->cookTorrN = ctN;
}

void MeshNode::setMaterialAlpha(float a) {
	properties->alpha = a;
}

void MeshNode::setMaterialTexture(std::string diffuseTextureName) {
	properties->setTexture(diffuseTextureName);
}

void MeshNode::setMaterialNormalMap(std::string normalMapName) {
	properties->setNormalMap(normalMapName);
}

void MeshNode::draw(Renderer* r, bool isTransparentPass) {
	extern MatrixStack* sceneGraphMatrixStack;
	extern MatrixStack* projectionMatrixStack;
	bool isMaterialTransparent = material->getHasTransparency();

	applyTransformation();

	if (isMaterialTransparent == isTransparentPass) {
		material->use();

		//construct normal matrix
		glm::mat4 normalMatrix = glm::mat4(sceneGraphMatrixStack->last());
		normalMatrix = glm::inverse(normalMatrix);
		normalMatrix = glm::transpose(normalMatrix);

		//apply geometry/mesh uniforms
		glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(sceneGraphMatrixStack->last()));
		glUniformMatrix4fv(1, 1, GL_FALSE, glm::value_ptr(projectionMatrixStack->last()));
		glUniformMatrix4fv(2, 1, GL_FALSE, glm::value_ptr(normalMatrix));

		//apply material uniforms
		material->setUniforms(properties);

		//bind, draw, and unbind vertices
		objectMesh->bindBuffers();
		glDrawElements(GL_TRIANGLES, 3*objectMesh->getTriCount(), GL_UNSIGNED_INT, NULL);
		objectMesh->unbindBuffers();

		material->unuse();
	}

	//draw children
	for(std::vector<scene_node*>::iterator it = children.begin(); it != children.end(); ++it) {
		(*it)->draw(r, isTransparentPass);
	}

	//undo changes to matrix
	sceneGraphMatrixStack->popMatrix();
}

void MeshNode::update(double deltaT) {
	for(std::vector<scene_node*>::iterator it = children.begin(); it != children.end(); ++it) {
		(*it)->update(deltaT);
	}
}