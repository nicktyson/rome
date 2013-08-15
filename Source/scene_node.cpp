#include <GL/glew.h> // GLEW
#include <vector>
#include "scene_node.h"
#include "SimState.h"
#include "MatrixStack.h"
#include "Renderer.h"

scene_node::scene_node() : sn_states(3) {
	for(int i = 0; i < 3; i++) {
		sn_State* state = &sn_states[i];
		state->scaling[0] = 1;
		state->scaling[1] = 1;
		state->scaling[2] = 1;
	}
}

scene_node * scene_node::getParent() {
	sn_State* currentState = &sn_states[SimState::currentUpdateState];

	return currentState->parent;
}

std::vector<scene_node *> scene_node::getChildren() {
	sn_State* currentState = &sn_states[SimState::currentUpdateState];

	return currentState->children;
}

void scene_node::clearChildren() {
	sn_State* currentState = &sn_states[SimState::currentUpdateState];

	currentState->children.clear();
}

void scene_node::setParent(scene_node * newParent) {
	sn_State* currentState = &sn_states[SimState::currentUpdateState];

	currentState->parent = newParent;
}

void scene_node::addChild(scene_node * newChild) {
	sn_State* currentState = &sn_states[SimState::currentUpdateState];

	currentState->children.push_back(newChild);
}

std::vector<float> scene_node::getTranslation() {
	sn_State* currentState = &sn_states[SimState::currentUpdateState];

	return currentState->translation;
}

std::vector<float> scene_node::getRotation() {
	sn_State* currentState = &sn_states[SimState::currentUpdateState];

	return currentState->rotation;
}

std::vector<float> scene_node::getScaling() {
	sn_State* currentState = &sn_states[SimState::currentUpdateState];

	return currentState->scaling;
}

void scene_node::setTranslation(float tx, float ty, float tz) {
	sn_State* currentState = &sn_states[SimState::currentUpdateState];

	currentState->translation[0] = tx;
	currentState->translation[1] = ty;
	currentState->translation[2] = tz;
}

void scene_node::setRotation(float rx, float ry, float rz) {
	sn_State* currentState = &sn_states[SimState::currentUpdateState];

	currentState->rotation[0] = rx;
	currentState->rotation[1] = ry;
	currentState->rotation[2] = rz;
}

void scene_node::setScaling(float sx, float sy, float sz) {
	sn_State* currentState = &sn_states[SimState::currentUpdateState];

	currentState->scaling[0] = sx;
	currentState->scaling[1] = sy;
	currentState->scaling[2] = sz;
}

void scene_node::draw(Renderer* r, bool isTransparentPass) {
	extern MatrixStack* sceneGraphMatrixStack;

	sn_State* currentState = &sn_states[SimState::currentRenderState];

	applyTransformation();

	for(std::vector<scene_node*>::iterator it = currentState->children.begin(); it != currentState->children.end(); ++it) {
		(*it)->draw(r, isTransparentPass);
	}

	sceneGraphMatrixStack->popMatrix();
}

void scene_node::applyTransformation() {
	extern MatrixStack* sceneGraphMatrixStack;

	sn_State* currentState = &sn_states[SimState::currentRenderState];

	sceneGraphMatrixStack->pushMatrix();
	sceneGraphMatrixStack->translated(currentState->translation[0], currentState->translation[1], currentState->translation[2]);
	sceneGraphMatrixStack->rotated(currentState->rotation[2], 0, 0, 1);
	sceneGraphMatrixStack->rotated(currentState->rotation[1], 0, 1, 0);
	sceneGraphMatrixStack->rotated(currentState->rotation[0], 1, 0, 0);
	sceneGraphMatrixStack->scaled(currentState->scaling[0], currentState->scaling[1], currentState->scaling[2]);
}

void scene_node::update(double deltaT) {
	sn_State* currentState = &sn_states[SimState::currentUpdateState];

	stateUpdate(deltaT);

	for(std::vector<scene_node*>::iterator it = currentState->children.begin(); it != currentState->children.end(); ++it) {
		(*it)->update(deltaT);
	}
}

void scene_node::stateUpdate(double deltaT) {
	sn_State* currentState = &sn_states[SimState::currentUpdateState];
	sn_State* newestState = &sn_states[SimState::newestState];

	currentState->parent = newestState->parent;
	currentState->children = newestState->children;

	currentState->translation = newestState->translation;
	currentState->rotation = newestState->rotation;
	currentState->scaling = newestState->scaling;
}