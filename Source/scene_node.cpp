#include <GL/glew.h> // GLEW
#include <vector>
#include "scene_node.h"
#include "MatrixStack.h"

scene_node::scene_node() {
	translation.resize(3);
	rotation.resize(3);
	scaling.resize(3);
	scaling[0] = 1;
	scaling[1] = 1;
	scaling[2] = 1;
}

scene_node * scene_node::getParent() {
	return parent;
}

std::vector<scene_node *> scene_node::getChildren() {
	return children;
}

void scene_node::setParent(scene_node * newParent) {
	parent = newParent;
}

void scene_node::addChild(scene_node * newChild) {
	children.push_back(newChild);
}

std::vector<float> scene_node::getTranslation() {
	return translation;
}

std::vector<float> scene_node::getRotation() {
	return rotation;
}

std::vector<float> scene_node::getScaling() {
	return scaling;
}

void scene_node::setTranslation(float tx, float ty, float tz) {
	translation[0] = tx;
	translation[1] = ty;
	translation[2] = tz;
}

void scene_node::setRotation(float rx, float ry, float rz) {
	rotation[0] = rx;
	rotation[1] = ry;
	rotation[2] = rz;
}

void scene_node::setScaling(float sx, float sy, float sz) {
	scaling[0] = sx;
	scaling[1] = sy;
	scaling[2] = sz;
}

void scene_node::draw() {
	extern MatrixStack* sceneGraphMatrixStack;

	applyTransformation();

	for(std::vector<scene_node*>::iterator it = children.begin(); it != children.end(); ++it) {
		(*it)->draw();
	}

	sceneGraphMatrixStack->popMatrix();
	//glPopMatrix();
}

void scene_node::applyTransformation() {
	extern MatrixStack* sceneGraphMatrixStack;

	sceneGraphMatrixStack->pushMatrix();
	sceneGraphMatrixStack->translated(translation[0], translation[1], translation[2]);
	sceneGraphMatrixStack->rotated(rotation[2], 0, 0, 1);
	sceneGraphMatrixStack->rotated(rotation[1], 0, 1, 0);
	sceneGraphMatrixStack->rotated(rotation[0], 1, 0, 0);
	sceneGraphMatrixStack->scaled(scaling[0], scaling[1], scaling[2]);
	/*glPushMatrix();
	glTranslated(translation[0], translation[1], translation[2]);
	glRotated(rotation[2], 0, 0, 1);
	glRotated(rotation[1], 0, 1, 0);
	glRotated(rotation[0], 1, 0, 0);
	glScaled(scaling[0], scaling[1], scaling[2]);*/
}

void scene_node::update(double deltaT) {
	for(std::vector<scene_node*>::iterator it = children.begin(); it != children.end(); ++it) {
		(*it)->update(deltaT);
	}
}