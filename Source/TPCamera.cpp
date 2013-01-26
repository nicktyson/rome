#include <gl/glew.h>
#include <vector>
#include "TPCamera.h"

TPCamera::TPCamera() {
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
	for(std::vector<scene_node*>::iterator it = children.begin(); it != children.end(); ++it) {
		(*it)->update();
	}
}

void TPCamera::forward() {
	translation[1] -= 0.1;
}

void TPCamera::back() {
	translation[1] += 0.1;
}

void TPCamera::left() {
	translation[0] += 0.1;
}

void TPCamera::right() {
	translation[0] -= 0.1;
}