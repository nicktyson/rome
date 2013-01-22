#include "SimState.h"
#include <GL/glfw.h>
#include <iostream>
#include "MeshNode.h"
#include "VitalEntity.h"

SimState::SimState() {

}

void SimState::initialize() {

}

void SimState::run() {

}

void SimState::resume() {

}

void SimState::pause() {

}

void SimState::end() {

}

void SimState::keyCallback(int key, int state) {
	if(key >= 0 && key < 256) {
		if(state == GLFW_PRESS) {
			keyState[key] = true;
		} else {
			keyState[key] = false;
		}
	}
}

void SimState::initScene() {
	extern std::string ROME_PATH;

	rootNode = new scene_node();
	std::string location = ROME_PATH + "/Assets/Meshes/test_icosphere.msh";
	std::string cubeLocation = ROME_PATH + "/Assets/Meshes/test_cube.msh";

	//make two offset objects
	MeshNode * childNode = new MeshNode(location);
	childNode->setTranslation(1, 1, 0);
	rootNode->addChild(childNode);

	VitalEntity * secondChild = new VitalEntity(cubeLocation, 0.01, 0, 0);
	secondChild->setTranslation(-2, -2, 0);
	secondChild->setRotation(20, 45, 0);
	childNode->addChild(secondChild);

	//make a ring of spheres
	//for(double i = 0; i < 6; ++i) {
	//	MeshNode * newNode = new MeshNode(location);
	//	rootNode->addChild(newNode);
	//	newNode->setTranslation(3*sin(i*6.28/6), 3*cos(i*6.28/6), 0);
	//}
}

void SimState::display() {
	glClearColor(0.4f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	gluLookAt(0, 0, 5, 0, 0, 0, 0, 1, 0);

	rootNode->draw();
}

void SimState::updateSim() {
	rootNode->update();
}

void SimState::simThreadFunc(void * dummy) {

}

void SimState::keyOps() {
	//keyState['X'] = false should be used for toggle keys, like camera modes
	//it should also be used for keys that should pause briefly before repeating
	//remove it to for keys that should repeat smoothly, every frame, like movement
	//Modifier keys should NOT have the line
	if (keyState['A'] && keyState['B']) {
		std::cout << "A + B" << std::endl;
		keyState['A'] = false;
	} else if (keyState['A']) {
		std::cout << "A" << std::endl;
		keyState['A'] = false;
	} else if (keyState['B']) {
		//do nothing; modifier key
	}
}