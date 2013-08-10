#include "Skybox.h"
#include <vector>
#include "Scene.h"
#include "Camera.h"
#include "SimState.h"
#include "Materials/MaterialList.h"
#include "Materials/Material.h"
#include "MatrixStack.h"
#include "mesh.h"
#include "MeshNode.h"
#include "../Lib/glm/glm.hpp"
#include "../Lib/glm/gtc/matrix_inverse.hpp"
#include "../Lib/glm/gtc/type_ptr.hpp"

Skybox::Skybox(Scene* scn, std::string fileLocation) : MeshNode(fileLocation, MaterialList::SOLIDDIFFUSE) {
	scene = scn;
}

void Skybox::draw(Renderer* r, bool isTransparentPass) {
	if (isTransparentPass) {
		return;
	}

	extern MatrixStack* sceneGraphMatrixStack;
	extern MatrixStack* projectionMatrixStack;

	mn_State* currentState = &mn_states[SimState::currentRenderState];
	sn_State* currentSNState = &sn_states[SimState::currentRenderState];


	//don't translate
	sceneGraphMatrixStack->pushMatrix();

	sceneGraphMatrixStack->rotated(currentSNState->rotation[0], 1, 0, 0);
	sceneGraphMatrixStack->rotated(currentSNState->rotation[1], 0, 1, 0);
	sceneGraphMatrixStack->rotated(currentSNState->rotation[2], 0, 0, 1);
	sceneGraphMatrixStack->scaled(currentSNState->scaling[0], currentSNState->scaling[1], currentSNState->scaling[2]);

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
	
	//undo changes to matrix
	sceneGraphMatrixStack->popMatrix();
}

void Skybox::update(double deltaT) {
	sn_State* newestState = &sn_states[SimState::newestState];

	MeshNode::stateUpdate(deltaT);

	std::vector<float> cameraRotation = scene->getCurrentCamera()->getRotation();

	newestState->rotation[0] = cameraRotation[0];
	newestState->rotation[1] = cameraRotation[1];
	newestState->rotation[2] = cameraRotation[2];
}