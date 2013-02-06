#include <gl/glew.h>
#include <vector>
#include <cmath>
#include <iostream>
#include "TPCamera.h"

const float TPCamera::MAX_VELOCITY = 5.0; // meters/second
const float TPCamera::DRAG = 5.0;

TPCamera::TPCamera() {
	velocity.resize(3);
	mousePos.resize(2);
	mousePos[0] = 400;
	mousePos[1] = 300;

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
	float inputVelocity = std::abs(velocity[0]) + std::abs(velocity[1]) + std::abs(velocity[2]);
	if (inputVelocity > MAX_VELOCITY) {
		velocity[0] = velocity[0] / inputVelocity * MAX_VELOCITY;
		velocity[1] = velocity[1] / inputVelocity * MAX_VELOCITY;
		velocity[2] = velocity[2] / inputVelocity * MAX_VELOCITY;
	}

	//update position
	translation[0] += deltaT * velocity[0];
	translation[1] += deltaT * velocity[1];
	translation[2] += deltaT * velocity[2];

	//smoothly decrease velocity
	if(abs(velocity[0]) < 0.1 && abs(velocity[1]) < 0.1) {
		velocity[0] = 0;
		velocity[1] = 0;
		velocity[2] = 0;
	} else {
		velocity[0] -= velocity[0] * DRAG * deltaT;
		velocity[1] -= velocity[1] * DRAG * deltaT;
		velocity[2] -= velocity[2] * DRAG * deltaT;
	}

	//update children
	for(std::vector<scene_node*>::iterator it = children.begin(); it != children.end(); ++it) {
		(*it)->update(deltaT);
	}

}

void TPCamera::forward() {
	velocity[1] += -MAX_VELOCITY;
}

void TPCamera::back() {
	velocity[1] += MAX_VELOCITY;
}

void TPCamera::left() {
	velocity[0] += MAX_VELOCITY;
}

void TPCamera::right() {
	velocity[0] += -MAX_VELOCITY;
}

void TPCamera::mouseView(int x, int y) {
	mousePos[0] = x;
	mousePos[1] = y;
}