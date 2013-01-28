#include <gl/glew.h>
#include <vector>
#include <cmath>
#include "TPCamera.h"

TPCamera::TPCamera() {
	velocity.resize(3);
	translation[2] = -6;
}

void TPCamera::draw() {
	applyTransformation();

	for(std::vector<scene_node*>::iterator it = children.begin(); it != children.end(); ++it) {
		(*it)->draw();
	}

	glPopMatrix();
}

void TPCamera::update() {
	translation[0] += velocity[0];
	translation[1] += velocity[1];
	translation[2] += velocity[2];

	if(abs(velocity[0]) < 0.05 && abs(velocity[1]) < 0.05) {
		velocity[0] = 0;
		velocity[1] = 0;
		velocity[2] = 0;
	} else {
		velocity[0] /= 1.07;
		velocity[1] /= 1.07;
		velocity[2] /= 1.07;
	}

	for(std::vector<scene_node*>::iterator it = children.begin(); it != children.end(); ++it) {
		(*it)->update();
	}

}

void TPCamera::forward() {
	velocity[1] = -0.1;
}

void TPCamera::back() {
	velocity[1] = 0.1;
}

void TPCamera::left() {
	velocity[0] = 0.1;
}

void TPCamera::right() {
	velocity[0] = -0.1;
}