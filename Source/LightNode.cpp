#include "LightNode.h"
#include "MatrixStack.h"

LightNode::LightNode() {
	color.resize(3);
}

void LightNode::draw() {
	extern MatrixStack* sceneGraphMatrixStack;

	applyTransformation();

	for(std::vector<scene_node*>::iterator it = children.begin(); it != children.end(); ++it) {
		(*it)->draw();
	}

	sceneGraphMatrixStack->popMatrix();
}

void LightNode::setColor(float r, float g, float b) {
	color[0] = r;
	color[1] = g;
	color[2] = b;
}