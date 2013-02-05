#include "SimState.h"
#include <GL/glfw.h>
#include <iostream>
#include "StateManager.h"
#include "MeshNode.h"
#include "VitalEntity.h"
#include "Camera.h"
#include "FPCamera.h"
#include "TPCamera.h"

std::vector<bool> State::keyState;

SimState::SimState() {
	DISPLAY_FRAME_RATE = 60;
	DISPLAY_FRAME_TIME = 1.0 / DISPLAY_FRAME_RATE;
	SIM_RATE = 120;
	SIM_TIME = 1.0 / SIM_RATE;
	initialized = false;
	shouldStopStateLoop = false;
	pauseSimThread = false;
	endSimThread = false;
}

void SimState::initialize(StateManager* mngr) {
	manager = mngr;
	initScene();
	pauseMutex = glfwCreateMutex();
	simThread = glfwCreateThread(startThread, this);
	initialized = true;

}

void SimState::run() {

	int frameCount = 0;
	double fpsTimeSum = 0;

	while (!shouldStopStateLoop) {
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
}

void SimState::resume() {
	shouldStopStateLoop = false;
	pauseSimThread = false;
	glfwUnlockMutex(pauseMutex);
}

void SimState::pause() {
	glfwLockMutex(pauseMutex);
	pauseSimThread = true;
	for(int i = 0; i < 300; i++) {
		keyState[i] = 0;
	}
}

void SimState::end() {
	endSimThread = true;
	pauseSimThread = false;
	glfwUnlockMutex(pauseMutex);
	glfwWaitThread(simThread, GLFW_WAIT);
}

void SimState::keyCallback(int key, int state) {
	if(key >= 0 && key < 300) {
		if(state == GLFW_PRESS) {
			keyState[key] = true;
		} else {
			keyState[key] = false;
		}
	}
}

void SimState::mousePosCallback(int x, int y) {
	camera->mouseView(x, y);
}

void SimState::initScene() {
	extern std::string ROME_PATH;

	camera = new FPCamera();
	std::string location = ROME_PATH + "/Assets/Meshes/test_icosphere.msh";
	std::string cubeLocation = ROME_PATH + "/Assets/Meshes/test_cube.msh";

	//make two offset objects
	MeshNode * childNode = new MeshNode(location);
	childNode->setTranslation(0, 0, 0);
	camera->addChild(childNode);

	//float randRot = rand() % 90;
	VitalEntity * secondChild = new VitalEntity(cubeLocation, 0, 0, 0);
	secondChild->setTranslation(0, 1, 0);
	secondChild->setScaling(1, 1, 2);
	//secondChild->setRotation(20, randRot, 0);
	childNode->addChild(secondChild);

	//add a floor
	MeshNode * floor = new MeshNode(cubeLocation);
	floor->setScaling(5, 5, 0.1);
	floor->setTranslation(0, 0, -4);
	camera->addChild(floor);

	//make a ring of spheres
	/*for(double i = 0; i < 6; ++i) {
		MeshNode * newNode = new MeshNode(location);
		camera->addChild(newNode);
		newNode->setTranslation(3*sin(i*6.28/6), 3*cos(i*6.28/6), 0);
	}*/
}

void SimState::display() {
	glClearColor(0.4f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	camera->draw();
}

void SimState::updateSim(double deltaT) {
	camera->update(deltaT);
}

void SimState::startThread(void * state) {
	SimState * stptr = static_cast<SimState*>(state);
	stptr->simThreadFunc();
}

void SimState::simThreadFunc() {

	double simStartTime;
	double simCurrentTime;
	double timeDif;
	double previousFrameStart = glfwGetTime();
	double deltaT;

	while (true) {
		if(pauseSimThread) {
			glfwLockMutex(pauseMutex);
			glfwUnlockMutex(pauseMutex);
			previousFrameStart = glfwGetTime();
		}

		if(endSimThread) {
			return;
		}

		simStartTime = glfwGetTime();
		deltaT = simStartTime - previousFrameStart;

		updateSim(deltaT);

		//clamp rate
		simCurrentTime = glfwGetTime();
		timeDif = simCurrentTime - simStartTime;
		if (SIM_TIME > timeDif) {
			glfwSleep(SIM_TIME - timeDif);
		}

		previousFrameStart = simStartTime;
	}
}

void SimState::keyOps() {
	//keyState['X'] = false should be used for toggle keys, like camera modes
	//it should also be used for keys that should pause briefly before repeating
	//remove it to for keys that should repeat smoothly, every frame, like movement
	//Modifier keys should NOT have the line
	if (keyState['N'] && keyState['B']) {
		std::cout << "N + B" << std::endl;
		keyState['N'] = false;
	} else if (keyState['N']) {
		std::cout << "N" << std::endl;
		keyState['N'] = false;
	} else if (keyState['B']) {
		//do nothing; modifier key
	}

	if (keyState['P']) {
		manager->changeState(StateManager::PAUSE);
		shouldStopStateLoop = true;
		keyState['P'] = false;
	}

	if (keyState['W']) {
		camera->forward();
	}
	if (keyState['A']) {
		camera->left();
	}
	if (keyState['S']) {
		camera->back();
	}
	if (keyState['D']) {
		camera->right();
	}

	if (keyState[GLFW_KEY_ESC]) {
		manager->changeState(StateManager::END);
		shouldStopStateLoop = true;
		keyState[GLFW_KEY_ESC] = false;
	}
}