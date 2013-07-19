#include <gl/glew.h>
#include <GL/glfw.h>
#include "SimState.h"
#include <iostream>
#include <vector>
#include "StateManager.h"
#include "MeshNode.h"
#include "VitalEntity.h"
#include "LightNode.h"
#include "Scene.h"
#include "TestScene.h"
#include "Materials\MaterialList.h"
#include "MatrixStack.h"
#include "Renderer.h"

std::vector<bool> State::keyState;

//desired fps (display and sim)
//keypresses are detected at the display rate
//times are in seconds
const int SimState::DISPLAY_FRAME_RATE = 60;
const double SimState::DISPLAY_FRAME_TIME = 1.0 / DISPLAY_FRAME_RATE;
const int SimState::SIM_RATE = 120;
const double SimState::SIM_TIME = 1.0 / SIM_RATE;

SimState::SimState() {
	initialized = false;
	shouldStopStateLoop = false;
	pauseSimThread = false;
	endSimThread = false;
}

void SimState::initialize(StateManager* mngr) {
	manager = mngr;

	renderer = new Renderer();

	currentScene = new TestScene();
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
		currentScene->update(1.0 / 60.0);
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
	currentScene->cameraMouseView(x, y);
}

void SimState::mouseWheelCallback(int pos) {
	currentScene->zoomCamera(pos);
}

void SimState::initScene() {
	currentScene->init();
}

void SimState::display() {
	renderer->render(currentScene);	
}

void SimState::updateSim(double deltaT) {
	currentScene->update(deltaT);
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

		stateKeyOps();

		simStartTime = glfwGetTime();
		deltaT = simStartTime - previousFrameStart;

		//updateSim(deltaT);

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

	if (keyState[GLFW_KEY_ESC]) {
		manager->changeState(StateManager::END);
		shouldStopStateLoop = true;
		keyState[GLFW_KEY_ESC] = false;
	}
}

void SimState::stateKeyOps() {
	if (keyState['W']) {
		currentScene->cameraForward();
	}
	if (keyState['A']) {
		currentScene->cameraLeft();
	}
	if (keyState['S']) {
		currentScene->cameraBack();
	}
	if (keyState['D']) {
		currentScene->cameraRight();
	}

	if (keyState['Q']) {
		currentScene->rotateCameraCCW();
	}
	if (keyState['E']) {
		currentScene->rotateCameraCW();
	}

	if (keyState['V']) {
		currentScene->switchCameras();
		keyState['V'] = false;
	}
}