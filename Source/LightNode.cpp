#include "LightNode.h"
#include <iostream>
#include "MatrixStack.h"
#include "Renderer.h"
#include "SimState.h"
#include "../Lib/glm/glm.hpp"

LightNode::LightNode() : ln_states(3) {
	setColor(1.0, 1.0, 1.0);
	setIntensity(1.0);
}

LightNode::LightNode(float r, float g, float b, float i) : ln_states(3) {
	setColor(r, g, b);
	setIntensity(i);
}

void LightNode::draw(Renderer* r, bool isTransparentPass) {
	extern MatrixStack* sceneGraphMatrixStack;
	//ln_State* currentState = &ln_states[SimState::currentUpdateState];
	//sn_State* currentSNState = &sn_states[SimState::currentUpdateState];
	sn_State* currentRenderState = &sn_states[SimState::currentRenderState];

	applyTransformation();

	eyespacePosition = glm::vec4(0.0, 0.0, 0.0, 1.0);
	eyespacePosition = sceneGraphMatrixStack->last() * eyespacePosition;

	r->addLight(this);

	for(std::vector<scene_node*>::iterator it = currentRenderState->children.begin(); it != currentRenderState->children.end(); ++it) {
		(*it)->draw(r, isTransparentPass);
	}

	sceneGraphMatrixStack->popMatrix();
}

void LightNode::update(double deltaT) {
	stateUpdate(deltaT);
	scene_node::stateUpdate(deltaT);
}

void LightNode::stateUpdate(double deltaT) {
	ln_State* currentState = &ln_states[SimState::currentUpdateState];
	ln_State* newestState = &ln_states[SimState::newestState];

	currentState->color = newestState->color;
	currentState->intensity = newestState->intensity;
}

void LightNode::setColor(float r, float g, float b) {
	ln_State* currentState = &ln_states[SimState::currentUpdateState];
	currentState->color[0] = r;
	currentState->color[1] = g;
	currentState->color[2] = b;
}

void LightNode::setIntensity(float i) {
	ln_State* currentState = &ln_states[SimState::currentUpdateState];

	currentState->intensity = i;
}