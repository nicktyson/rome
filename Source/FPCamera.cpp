#include <gl/glew.h>
#include "FPCamera.h"
#include <vector>
#include <iostream>
#include <cmath>

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
	rotation[0] = -90;
	//back away from the default objects
	translation[1] = 6;
}

void FPCamera::draw() {
	glPushMatrix();

	glRotated(rotation[0], 1, 0, 0);
	glRotated(rotation[1], 0, 1, 0);
	glRotated(rotation[2], 0, 0, 1);

	glTranslated(translation[0], translation[1], translation[2]);


	for(std::vector<scene_node*>::iterator it = children.begin(); it != children.end(); ++it) {
		(*it)->draw();
	}

	glPopMatrix();
}

void FPCamera::update(double deltaT) {
	//moving two directions at once shouldn't be faster
	float inputVelocity = std::abs(velocity[0]) + std::abs(velocity[1]) + std::abs(velocity[2]);
	if (inputVelocity > 8.0) {
		velocity[0] = velocity[0] / inputVelocity * 8.0;
		velocity[1] = velocity[1] / inputVelocity * 8.0;
		velocity[2] = velocity[2] / inputVelocity * 8.0;
	}

	//update position and camera angle
	translation[0] += deltaT * velocity[0];
	translation[1] += deltaT * velocity[1];
	translation[2] += deltaT * velocity[2];

	rotation[0] += deltaT  * angVelocity[0];
	rotation[1] += deltaT  * angVelocity[1];
	rotation[2] += deltaT  * angVelocity[2];

	//smoothly decrease movement velocity
	if(abs(velocity[0]) < 0.1 && abs(velocity[1]) < 0.1) {
		velocity[0] = 0;
		velocity[1] = 0;
		velocity[2] = 0;
	} else {
		velocity[0] /= 1.2;
		velocity[1] /= 1.2;
		velocity[2] /= 1.2;
	}

	//smoothly decrease camera velocity to prevent stuttering
	if(abs(angVelocity[0]) < 0.1 && abs(angVelocity[1]) < 0.1) {
		angVelocity[0] = 0;
		angVelocity[1] = 0;
		angVelocity[2] = 0;
	} else {
		angVelocity[0] /= 1.1;
		angVelocity[1] /= 1.1;
		angVelocity[2] /= 1.1;
	}

	// looking up and down is constrained (straight ahead is -90, straight down is 0)
	if(rotation[0] >= 0.0) {
		rotation[0] = 0.0;
	} else if (rotation[0] <= - 170.0) {
		rotation[0] = -170.0;
	}

	//update children
	for(std::vector<scene_node*>::iterator it = children.begin(); it != children.end(); ++it) {
		(*it)->update(deltaT);
	}
}

void FPCamera::forward() {
	velocity[1] += std::cos(3.14 * rotation[2] / 180.0) * -8;
	velocity[0] += std::sin(3.14 * rotation[2] / 180.0) * -8;
}

void FPCamera::back() {
	velocity[1] += std::cos(3.14 * rotation[2] / 180.0) * 8;
	velocity[0] += std::sin(3.14 * rotation[2] / 180.0) * 8;
	
}

void FPCamera::left() {
	velocity[0] += std::cos(3.14 * rotation[2] / 180.0) * 8;
	velocity[1] += std::sin(3.14 * rotation[2] / 180.0) * -8;
}

void FPCamera::right() {
	velocity[0] += std::cos(3.14 * rotation[2] / 180.0) * -8;
	velocity[1] += std::sin(3.14 * rotation[2] / 180.0) * 8;
}

void FPCamera::mouseView(int x, int y) {

	lastMouse[0] = currentMouse[0];
	lastMouse[1] = currentMouse[1];
	
	currentMouse[0] = x;
	currentMouse[1] = y;

	if (lastMouse[0] == -1) {
		return;
	}

	angVelocity[0] += ((float)(currentMouse[1] - lastMouse[1])) * 6.0f;
	angVelocity[2] += ((float)(currentMouse[0] - lastMouse[0])) * 6.0f;
}