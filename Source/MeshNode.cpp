#include <GL/glew.h>
#include <iostream>
#include <string>
#include "../Lib/glm/glm.hpp"
#include "../Lib/glm/gtc/type_ptr.hpp"
#include "MeshNode.h"
#include "Materials\MaterialList.h"
#include "Materials\Material.h"
#include "MatrixStack.h"

MeshNode::MeshNode() {
	objectMesh = new mesh();
	setMaterial(MaterialList::NORMAL);
}

MeshNode::MeshNode(std::string fileLocation) {
	objectMesh = new mesh(fileLocation);
	setMaterial(MaterialList::NORMAL);
}

MeshNode::MeshNode(std::string fileLocation, MaterialList::Materials materialType) {
	objectMesh = new mesh(fileLocation);
	setMaterial(materialType);
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

void MeshNode::draw() {
	extern MatrixStack* sceneGraphMatrixStack;
	extern MatrixStack* projectionMatrixStack;

	applyTransformation();

	material->use();
	
	glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(sceneGraphMatrixStack->last()));
	glUniformMatrix4fv(1, 1, GL_FALSE, glm::value_ptr(projectionMatrixStack->last()));

	objectMesh->bindBuffers();

	glDrawElements(GL_TRIANGLES, 3*objectMesh->getTriCount(), GL_UNSIGNED_INT, NULL);

	objectMesh->unbindBuffers();

	material->unuse();

	//draw children
	for(std::vector<scene_node*>::iterator it = children.begin(); it != children.end(); ++it) {
		(*it)->draw();
	}

	//undo changes to matrix
	sceneGraphMatrixStack->popMatrix();
}

void MeshNode::update(double deltaT) {
	for(std::vector<scene_node*>::iterator it = children.begin(); it != children.end(); ++it) {
		(*it)->update(deltaT);
	}
}