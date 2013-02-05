#include <gl/glew.h>
#include <vector>
#include <cmath>
#include "TPCamera.h"

TPCamera::TPCamera() {
	velocity.resize(3);
	//start zoomed out
	translation[2] = -6;
}

void TPCamera::draw() {
	applyTransformation();

	for(std::vector<scene_node*>::iterator it = children.begin(); it != children.end(); ++it) {
		(*it)->draw();
	}

	glPopMatrix();
}

void TPCamera::update(double deltaT) {
	translation[0] += deltaT * velocity[0];
	translation[1] += deltaT * velocity[1];
	translation[2] += deltaT * velocity[2];

	//smoothly decrease velocity
	if(abs(velocity[0]) < 0.1 && abs(velocity[1]) < 0.1) {
		velocity[0] = 0;
		velocity[1] = 0;
		velocity[2] = 0;
	} else {
		velocity[0] /= 1.1;
		velocity[1] /= 1.1;
		velocity[2] /= 1.1;
	}

	//update children
	for(std::vector<scene_node*>::iterator it = children.begin(); it != children.end(); ++it) {
		(*it)->update(deltaT);
	}

}

void TPCamera::forward() {
	velocity[1] = -8;
}

void TPCamera::back() {
	velocity[1] = 8;
}

void TPCamera::left() {
	velocity[0] = 8;
}

void TPCamera::right() {
	velocity[0] = -8;
}

void TPCamera::mouseView(int x, int y) {

}