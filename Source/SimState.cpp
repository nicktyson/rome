#include "SimState.h"
#include <GL/glfw.h>
#include <iostream>
#include "MeshNode.h"
#include "VitalEntity.h"

SimState::SimState() {
	DISPLAY_FRAME_RATE = 30;
	DISPLAY_FRAME_TIME = 1.0 / DISPLAY_FRAME_RATE;
	SIM_RATE = 60;
	SIM_TIME = 1.0 / SIM_RATE;
	initialized = false;
}

void SimState::initialize() {
	initScene();
	pauseMutex = glfwCreateMutex();
	GLFWthread simThread;
	simThread = glfwCreateThread(startThread, this);
	initialized = true;
}

State* SimState::run() {
	shouldPause = false;

	int frameCount = 0;
	double fpsTimeSum = 0;

	while (!shouldPause) {
		double loopStartTime = glfwGetTime();

		keyOps();
		display();
		glfwSwapBuffers();

		//clamp display frame rate
		double loopCurrentTime = glfwGetTime();
		double timeDif = loopCurrentTime - loopStartTime;
		if (DISPLAY_FRAME_TIME > timeDif) {
			glfwSleep(DISPLAY_FRAME_TIME - timeDif);
		}

		//calculate and display FPS once per second
		frameCount++;
		fpsTimeSum += glfwGetTime() - loopStartTime;
		if (fpsTimeSum >= 1) {
			std::cout << (frameCount / (fpsTimeSum)) << "\n";
			frameCount = 0;
			fpsTimeSum = 0;
		}
	}

	pause();

	return;
}

void SimState::resume() {
	shouldPause = false;
	glfwUnlockMutex(pauseMutex);
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

	float randRot = rand() % 90;
	VitalEntity * secondChild = new VitalEntity(cubeLocation, 0.01, 0, 0);
	secondChild->setTranslation(-2, -2, 0);
	secondChild->setRotation(20, randRot, 0);
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

void SimState::simThreadFunc() {

	while (true) {
		if(shouldPause) {
			glfwLockMutex(pauseMutex);
			glfwUnlockMutex(pauseMutex);
		}

		double simStartTime = glfwGetTime();

		updateSim();

		//clamp rate
		double simCurrentTime = glfwGetTime();
		double timeDif = simCurrentTime - simStartTime;
		if (SIM_TIME > timeDif) {
			glfwSleep(SIM_TIME - timeDif);
		}
	}
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

	if (keyState['P']) {
		glfwLockMutex(pauseMutex);
		shouldPause = true;
		keyState['P'] = false;
	}
}