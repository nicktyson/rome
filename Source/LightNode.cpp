#include "LightNode.h"
#include "MatrixStack.h"
#include "Renderer.h"
#include "../Lib/glm/glm.hpp"

LightNode::LightNode() {
	color.resize(3);
	setColor(1.0, 1.0, 1.0);
	setIntensity(1.0);
}

LightNode::LightNode(float r, float g, float b, float i) {
	color.resize(3);
	setColor(r, g, b);
	setIntensity(i);
}

void LightNode::draw(Renderer* r) {
	extern MatrixStack* sceneGraphMatrixStack;

	applyTransformation();

	eyespacePosition = glm::vec4(0.0, 0.0, 0.0, 1.0);
	eyespacePosition = sceneGraphMatrixStack->last() * eyespacePosition;

	r->addLight(this);

	for(std::vector<scene_node*>::iterator it = children.begin(); it != children.end(); ++it) {
		(*it)->draw(r);
	}

	sceneGraphMatrixStack->popMatrix();
}

void LightNode::setColor(float r, float g, float b) {
	color[0] = r;
	color[1] = g;
	color[2] = b;
}

void LightNode::setIntensity(float i) {
	intensity = i;
}