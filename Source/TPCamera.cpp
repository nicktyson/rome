#include <gl/glew.h>
#include <vector>
#include <cmath>
#include <iostream>
#include "TPCamera.h"
#include "MatrixStack.h"
#include "Renderer.h"
#include "SimState.h"
#include "../Lib/glm/glm.hpp"
#include "../Lib/glm/gtc/matrix_inverse.hpp"

const float TPCamera::MAX_VELOCITY = 7.0; // meters/second
const float TPCamera::DRAG = 5.0;
const float TPCamera::MAX_ROTATE_VELOCITY = 30.0;
const float TPCamera::VIEW_DRAG = 12.0;
const float TPCamera::ZOOM_VELOCITY = 3.0;
const float TPCamera::ZOOM_DRAG = 4.0;

TPCamera::TPCamera() {
	velocity.resize(3);
	angVelocity.resize(3);
	mousePos.resize(2);
	mousePos[0] = 400;
	mousePos[1] = 300;
	zoomLevel = 0;

	//start zoomed out
	for (int i = 0; i < 3; i++) {
		sn_states[i].translation[2] = -6;
		//sn_states[i].rotation[0] = 90.0;
	}
}

void TPCamera::draw(Renderer* r, bool isTransparentPass) {
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

void TPCamera::update(double deltaT) {
	sn_State* currentState = &sn_states[SimState::currentUpdateState];
	sn_State* previousState = &sn_states[SimState::newestState];

	scene_node::stateUpdate(deltaT);

	//see if mouse is against the side of the screen
	if (mousePos[0] < 10) {
		velocity[0] += MAX_VELOCITY;
	} else if (mousePos[0] > 790) {
		velocity[0] -= MAX_VELOCITY;
	}

	if (mousePos[1] < 10) {
		velocity[1] -= MAX_VELOCITY;
	} else if (mousePos[1] > 590) {
		velocity[1] += MAX_VELOCITY;
	}

	//moving two directions at once shouldn't be faster
	float inputVelocity = std::abs(velocity[0]) + std::abs(velocity[1]);
	if (inputVelocity > MAX_VELOCITY) {
		velocity[0] = velocity[0] / inputVelocity * MAX_VELOCITY;
		velocity[1] = velocity[1] / inputVelocity * MAX_VELOCITY;
	}

	//update position
	currentState->translation[0] = previousState->translation[0] + deltaT * velocity[0];
	currentState->translation[1] = previousState->translation[1] + deltaT * velocity[1];
	currentState->translation[2] = previousState->translation[2] + deltaT * velocity[2];

	//update rotation
	currentState->rotation[0] = previousState->rotation[0] + deltaT  * angVelocity[0];
	currentState->rotation[1] = previousState->rotation[1] + deltaT  * angVelocity[1];
	currentState->rotation[2] = previousState->rotation[2] + deltaT  * angVelocity[2];

	//smoothly decrease velocity
	if(abs(velocity[0]) < 0.1 && abs(velocity[1]) < 0.1) {
		velocity[0] = 0;
		velocity[1] = 0;
	} else {
		velocity[0] -= velocity[0] * DRAG * deltaT;
		velocity[1] -= velocity[1] * DRAG * deltaT;
	}

	//smoothly slow down zooming
	if(abs(velocity[2]) < 0.1) {
		velocity[2] = 0;
	} else {
		velocity[2] -= velocity[2] * ZOOM_DRAG * deltaT;
	}

	//smoothly decrease camera angular velocity to prevent stuttering
	if(abs(angVelocity[2]) < 0.1) {
		angVelocity[0] = 0;
		angVelocity[1] = 0;
		angVelocity[2] = 0;
	} else {
		angVelocity[0] -= angVelocity[0] * VIEW_DRAG * deltaT;
		angVelocity[1] -= angVelocity[1] * VIEW_DRAG * deltaT;
		angVelocity[2] -= angVelocity[2] * VIEW_DRAG * deltaT;
	}

	//update children
	for(std::vector<scene_node*>::iterator it = currentState->children.begin(); it != currentState->children.end(); ++it) {
		(*it)->update(deltaT);
	}

}

void TPCamera::forward() {
	sn_State* newestState = &sn_states[SimState::newestState];

	velocity[1] += std::cos(3.14 * newestState->rotation[2] / 180.0) * -MAX_VELOCITY;
	velocity[0] += std::sin(3.14 * newestState->rotation[2] / 180.0) * -MAX_VELOCITY;
}

void TPCamera::back() {
	sn_State* newestState = &sn_states[SimState::newestState];

	velocity[1] += std::cos(3.14 * newestState->rotation[2] / 180.0) * MAX_VELOCITY;
	velocity[0] += std::sin(3.14 * newestState->rotation[2] / 180.0) * MAX_VELOCITY;
}

void TPCamera::left() {
	sn_State* newestState = &sn_states[SimState::newestState];

	velocity[0] += std::cos(3.14 * newestState->rotation[2] / 180.0) * MAX_VELOCITY;
	velocity[1] += std::sin(3.14 * newestState->rotation[2] / 180.0) * -MAX_VELOCITY;
}

void TPCamera::right() {
	sn_State* newestState = &sn_states[SimState::newestState];

	velocity[0] += std::cos(3.14 * newestState->rotation[2] / 180.0) * -MAX_VELOCITY;
	velocity[1] += std::sin(3.14 * newestState->rotation[2] / 180.0) * MAX_VELOCITY;
}

void TPCamera::mouseView(int x, int y) {
	mousePos[0] = x;
	mousePos[1] = y;
}

void TPCamera::rotateCW() {
	angVelocity[2] += MAX_ROTATE_VELOCITY;
}

void TPCamera::rotateCCW() {
	angVelocity[2] += -MAX_ROTATE_VELOCITY;
}

void TPCamera::zoom(double dy) {
	zoomLevel += dy;
	velocity[2] += ZOOM_VELOCITY * dy;
}

//
glm::mat4 TPCamera::getInverseNormalMatrix() {
	extern MatrixStack* sceneGraphMatrixStack;

	sn_State* currentState = &sn_states[SimState::currentRenderState];

	sceneGraphMatrixStack->pushMatrix();

	sceneGraphMatrixStack->loadIdentity();
	
	sceneGraphMatrixStack->rotated(90.0, 1, 0, 0);

	sceneGraphMatrixStack->rotated(-currentState->rotation[2], 0, 0, 1);
	sceneGraphMatrixStack->rotated(-currentState->rotation[1], 0, 1, 0);
	sceneGraphMatrixStack->rotated(-currentState->rotation[0], 1, 0, 0);

	//sceneGraphMatrixStack->translated(-currentState->translation[0], -currentState->translation[1], -currentState->translation[2]);

	//sceneGraphMatrixStack->rotated(90.0, 1, 0, 0);

	glm::mat4 normalMatrix = glm::mat4(sceneGraphMatrixStack->last());
	normalMatrix = glm::inverse(normalMatrix);
	normalMatrix = glm::transpose(normalMatrix);
	//normalMatrix = glm::inverse(normalMatrix);

	sceneGraphMatrixStack->popMatrix();

	return normalMatrix;
}