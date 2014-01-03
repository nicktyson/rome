#include <GL/glew.h>
#include <iostream>
#include <string>
#include "../Lib/glm/glm.hpp"
#include "../Lib/glm/gtc/matrix_inverse.hpp"
#include "../Lib/glm/gtc/type_ptr.hpp"
#include "../Lib/glm/gtx/matrix_query.hpp"
#include "MeshNode.h"
#include "SimState.h"
#include "Materials\MaterialList.h"
#include "Materials\Material.h"
#include "MatrixStack.h"
#include "mesh.h"
#include "Renderer.h"
#include "MaterialProperties.h"

MeshNode::MeshNode() : mn_states(3) {
	extern MaterialList* materialList;

	mesh* tempMesh = new mesh();
	MaterialProperties* tempProps = new MaterialProperties();

	for (int i = 0; i < 3; i++) {
		mn_states[i].objectMesh = tempMesh;
		mn_states[i].material = materialList->getMaterial(MaterialList::NORMAL);
		mn_states[i].properties = tempProps;
	}
}

MeshNode::MeshNode(std::string fileLocation) : mn_states(3) {
	extern MaterialList* materialList;

	mesh* tempMesh = new mesh(fileLocation);
	MaterialProperties* tempProps = new MaterialProperties();

	for (int i = 0; i < 3; i++) {
		mn_states[i].objectMesh = tempMesh;
		mn_states[i].material = materialList->getMaterial(MaterialList::NORMAL);
		mn_states[i].properties = tempProps;
	}
}

MeshNode::MeshNode(std::string fileLocation, MaterialList::Materials materialType) : mn_states(3) {
	extern MaterialList* materialList;

	mesh* tempMesh = new mesh(fileLocation);
	MaterialProperties* tempProps = new MaterialProperties();

	for (int i = 0; i < 3; i++) {
		mn_states[i].objectMesh = tempMesh;
		mn_states[i].material = materialList->getMaterial(materialType);
		mn_states[i].properties = tempProps;
	}
}

mesh* MeshNode::getMesh() {
	mn_State* currentState = &mn_states[SimState::currentUpdateState];

	return currentState->objectMesh;
}

void MeshNode::setMesh(mesh* newMesh) {
	mn_State* currentState = &mn_states[SimState::currentUpdateState];

	currentState->objectMesh = newMesh;
}

void MeshNode::setMaterial(MaterialList::Materials materialType) {
	extern MaterialList* materialList;
	mn_State* currentState = &mn_states[SimState::currentUpdateState];

	currentState->material = materialList->getMaterial(materialType);
}

void MeshNode::setMaterialColor(float r, float g, float b) {
	mn_State* currentState = &mn_states[SimState::currentUpdateState];

	currentState->properties->diffuse[0] = r;
	currentState->properties->diffuse[1] = g;
	currentState->properties->diffuse[2] = b;
}

void MeshNode::setMaterialColor(float rD, float gD, float bD, float rS, float gS, float bS) {
	mn_State* currentState = &mn_states[SimState::currentUpdateState];

	currentState->properties->diffuse[0] = rD;
	currentState->properties->diffuse[1] = gD;
	currentState->properties->diffuse[2] = bD;

	currentState->properties->specular[0] = rS;
	currentState->properties->specular[1] = gS;
	currentState->properties->specular[2] = bS;
}

void MeshNode::setMaterialProperties(float bpExp, float ctM, float ctN) {
	mn_State* currentState = &mn_states[SimState::currentUpdateState];

	currentState->properties->blinnPhongExponent = bpExp;
	currentState->properties->cookTorrM = ctM;
	currentState->properties->cookTorrN = ctN;
}

void MeshNode::setMaterialAlpha(float a) {
	mn_State* currentState = &mn_states[SimState::currentUpdateState];

	currentState->properties->alpha = a;
}

void MeshNode::setMaterialTexture(std::string diffuseTextureName) {
	mn_State* currentState = &mn_states[SimState::currentUpdateState];

	currentState->properties->setTexture(diffuseTextureName);
}

void MeshNode::setMaterialNormalMap(std::string normalMapName) {
	mn_State* currentState = &mn_states[SimState::currentUpdateState];

	currentState->properties->setNormalMap(normalMapName);
}

void MeshNode::draw(Renderer* r, bool isTransparentPass) {
	extern MatrixStack* sceneGraphMatrixStack;
	extern MatrixStack* projectionMatrixStack;


	//if (glm::isIdentity(projectionMatrixStack->last(), 0.01f)) {
	//	std::cout << "projection matrix shouldn't be identity :/" << std::endl;
	//}

	mn_State* currentState = &mn_states[SimState::currentRenderState];
	sn_State* currentSNState = &sn_states[SimState::currentRenderState];

	bool isMaterialTransparent = currentState->material->getHasTransparency();

	applyTransformation();

	if (isMaterialTransparent == isTransparentPass) {
		currentState->material->use();

		//construct normal matrix
		glm::mat4 normalMatrix = glm::mat4(sceneGraphMatrixStack->last());
		normalMatrix = glm::inverse(normalMatrix);
		normalMatrix = glm::transpose(normalMatrix);

		//apply geometry/mesh uniforms
		glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(sceneGraphMatrixStack->last()));
		glUniformMatrix4fv(1, 1, GL_FALSE, glm::value_ptr(projectionMatrixStack->last()));
		glUniformMatrix4fv(2, 1, GL_FALSE, glm::value_ptr(normalMatrix));

		//apply material uniforms
		currentState->material->setUniforms(currentState->properties);

		//bind, draw, and unbind vertices
		currentState->objectMesh->bindBuffers();
		glDrawElements(GL_TRIANGLES, 3*currentState->objectMesh->getTriCount(), GL_UNSIGNED_INT, NULL);
		currentState->objectMesh->unbindBuffers();

		currentState->material->unuse();
	}

	//draw children
	for(std::vector<scene_node*>::iterator it = currentSNState->children.begin(); it != currentSNState->children.end(); ++it) {
		(*it)->draw(r, isTransparentPass);
	}

	//undo changes to matrix
	sceneGraphMatrixStack->popMatrix();
}

void MeshNode::update(double deltaT) {
	sn_State* currentSNState = &sn_states[SimState::currentUpdateState];

	stateUpdate(deltaT);

	for(std::vector<scene_node*>::iterator it = currentSNState->children.begin(); it != currentSNState->children.end(); ++it) {
		(*it)->update(deltaT);
	}
}

void MeshNode::stateUpdate(double deltaT) {
	scene_node::stateUpdate(deltaT);

	mn_State* currentState = &mn_states[SimState::currentUpdateState];
	mn_State* newestState = &mn_states[SimState::newestState];

	currentState->material = newestState->material;
	currentState->objectMesh = newestState->objectMesh;
	currentState->properties = newestState->properties;
}