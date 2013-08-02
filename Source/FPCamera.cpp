#include <gl/glew.h>
#include "FPCamera.h"
#include <vector>
#include <iostream>
#include <cmath>
#include "MatrixStack.h"
#include "Renderer.h"
#include "SimState.h"
#include "../Lib/glm/glm.hpp"
#include "../Lib/glm/gtc/matrix_inverse.hpp"

const float FPCamera::MAX_VELOCITY = 8.0; // meters/second
const float FPCamera::MOUSE_SENSITIVITY = 6.0; // degrees/second per mouse coordinate
const float FPCamera::VELOCITY_DRAG = 10.0;
const float FPCamera::VIEW_DRAG = 14.0;

FPCamera::FPCamera() {
	lastMouse.resize(2);
	currentMouse.resize(2);

	lastMouse[0] = -1;
	lastMouse[1] = -1;
	currentMouse[0] = -1;
	currentMouse[1] = -1;

	velocity.resize(3);
	angVelocity.resize(3);

	//face forward
	//back away from the default objects
	for (int i = 0; i < 3; i++) {
		sn_states[i].rotation[0] = -90;
		sn_states[i].translation[1] = 6;
	}
}

void FPCamera::draw(Renderer* r, bool isTransparentPass) {
	extern MatrixStack* sceneGraphMatrixStack;

	sn_State* currentState = &sn_states[SimState::currentRenderState];

	sceneGraphMatrixStack->pushMatrix();

	sceneGraphMatrixStack->rotated(currentState->rotation[0], 1, 0, 0);
	sceneGraphMatrixStack->rotated(currentState->rotation[1], 0, 1, 0);
	sceneGraphMatrixStack->rotated(currentState->rotation[2], 0, 0, 1);
	sceneGraphMatrixStack->translated(currentState->translation[0], currentState->translation[1], currentState->translation[2]);

	for(std::vector<scene_node*>::iterator it = currentState->children.begin(); it != currentState->children.end(); ++it) {
		(*it)->draw(r, isTransparentPass);
	}

	sceneGraphMatrixStack->popMatrix();
}

void FPCamera::update(double deltaT) {
	sn_State* currentState = &sn_states[SimState::currentUpdateState];
	sn_State* previousState = &sn_states[SimState::newestState];

	scene_node::stateUpdate(deltaT);

	//moving two directions at once shouldn't be faster
	float inputVelocity = std::abs(velocity[0]) + std::abs(velocity[1]) + std::abs(velocity[2]);
	if (inputVelocity > MAX_VELOCITY) {
		velocity[0] = velocity[0] / inputVelocity * MAX_VELOCITY;
		velocity[1] = velocity[1] / inputVelocity * MAX_VELOCITY;
		velocity[2] = velocity[2] / inputVelocity * MAX_VELOCITY;
	}

	//update position and camera angle
	currentState->translation[0] = previousState->translation[0] + deltaT * velocity[0];
	currentState->translation[1] = previousState->translation[1] + deltaT * velocity[1];
	currentState->translation[2] = previousState->translation[2] + deltaT * velocity[2];

	currentState->rotation[0] = previousState->rotation[0] + deltaT  * angVelocity[0];
	currentState->rotation[1] = previousState->rotation[1] + deltaT  * angVelocity[1];
	currentState->rotation[2] = previousState->rotation[2] + deltaT  * angVelocity[2];

	//smoothly decrease movement velocity
	if(abs(velocity[0]) < 0.1 && abs(velocity[1]) < 0.1) {
		velocity[0] = 0;
		velocity[1] = 0;
		velocity[2] = 0;
	} else {
		velocity[0] -= velocity[0] * VELOCITY_DRAG * deltaT;
		velocity[1] -= velocity[1] * VELOCITY_DRAG * deltaT;
		velocity[2] -= velocity[2] * VELOCITY_DRAG * deltaT;
	}

	//smoothly decrease camera velocity to prevent stuttering
	if(abs(angVelocity[0]) < 0.1 && abs(angVelocity[1]) < 0.1) {
		angVelocity[0] = 0;
		angVelocity[1] = 0;
		angVelocity[2] = 0;
	} else {
		angVelocity[0] -= angVelocity[0] * VIEW_DRAG * deltaT;
		angVelocity[1] -= angVelocity[1] * VIEW_DRAG * deltaT;
		angVelocity[2] -= angVelocity[2] * VIEW_DRAG * deltaT;
	}

	// looking up and down is constrained (straight ahead is -90, straight down is 0)
	if(currentState->rotation[0] >= 0.0) {
		currentState->rotation[0] = 0.0;
	} else if (currentState->rotation[0] <= - 170.0) {
		currentState->rotation[0] = -170.0;
	}

	//update children
	for(std::vector<scene_node*>::iterator it = currentState->children.begin(); it != currentState->children.end(); ++it) {
		(*it)->update(deltaT);
	}
}

void FPCamera::forward() {
	sn_State* newestState = &sn_states[SimState::newestState];

	velocity[1] += std::cos(3.14 * newestState->rotation[2] / 180.0) * -MAX_VELOCITY;
	velocity[0] += std::sin(3.14 * newestState->rotation[2] / 180.0) * -MAX_VELOCITY;
}

void FPCamera::back() {
	sn_State* newestState = &sn_states[SimState::newestState];

	velocity[1] += std::cos(3.14 * newestState->rotation[2] / 180.0) * MAX_VELOCITY;
	velocity[0] += std::sin(3.14 * newestState->rotation[2] / 180.0) * MAX_VELOCITY;
}

void FPCamera::left() {
	sn_State* newestState = &sn_states[SimState::newestState];

	velocity[0] += std::cos(3.14 * newestState->rotation[2] / 180.0) * MAX_VELOCITY;
	velocity[1] += std::sin(3.14 * newestState->rotation[2] / 180.0) * -MAX_VELOCITY;
}

void FPCamera::right() {
	sn_State* newestState = &sn_states[SimState::newestState];

	velocity[0] += std::cos(3.14 * newestState->rotation[2] / 180.0) * -MAX_VELOCITY;
	velocity[1] += std::sin(3.14 * newestState->rotation[2] / 180.0) * MAX_VELOCITY;
}

void FPCamera::mouseView(int x, int y) {

	lastMouse[0] = currentMouse[0];
	lastMouse[1] = currentMouse[1];
	
	currentMouse[0] = x;
	currentMouse[1] = y;

	if (lastMouse[0] == -1) {
		return;
	}

	angVelocity[0] += ((float)(currentMouse[1] - lastMouse[1])) * MOUSE_SENSITIVITY;
	angVelocity[2] += ((float)(currentMouse[0] - lastMouse[0])) * MOUSE_SENSITIVITY;
}

void FPCamera::rotateCW() {

}

void FPCamera::rotateCCW() {

}

glm::mat4 FPCamera::getInverseNormalMatrix() {
	extern MatrixStack* sceneGraphMatrixStack;

	sn_State* currentState = &sn_states[SimState::currentRenderState];

	sceneGraphMatrixStack->pushMatrix();

	sceneGraphMatrixStack->rotated(currentState->rotation[0], 1, 0, 0);
	sceneGraphMatrixStack->rotated(currentState->rotation[1], 0, 1, 0);
	sceneGraphMatrixStack->rotated(currentState->rotation[2], 0, 0, 1);
	sceneGraphMatrixStack->translated(currentState->translation[0], currentState->translation[1], currentState->translation[2]);

	glm::mat4 normalMatrix = glm::mat4(sceneGraphMatrixStack->last());
	normalMatrix = glm::inverse(normalMatrix);
	normalMatrix = glm::transpose(normalMatrix);
	normalMatrix = glm::inverse(normalMatrix);

	sceneGraphMatrixStack->popMatrix();

	return normalMatrix;
}