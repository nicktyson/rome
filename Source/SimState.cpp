#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include "SimState.h"
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>
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
int SimState::currentRenderState = 0;
int SimState::currentUpdateState = 0;
int SimState::newestState = 0;

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

	currentRenderState = 0;
	currentUpdateState = 0;
	newestState = 0;

	currentScene = new TestScene();
	initScene();

	currentUpdateState = 1;

	std::mutex pauseMutex();
	std::mutex tripleBufferMutex();

	std::thread simThread(&SimState::simThreadFunc, this);
	simThread.detach();
	initialized = true;
}

void SimState::run() {
	
	int frameCount = 0;
	double fpsTimeSum = 0;

	while (!shouldStopStateLoop) {
		double loopStartTime = glfwGetTime();

		display();
		
		//secondary thread possible
		swapBuffers();
		
		//main thread only
		glfwPollEvents();

		//clamp display frame rate
		double loopCurrentTime = glfwGetTime();
		double timeDif = loopCurrentTime - loopStartTime;
		if (DISPLAY_FRAME_TIME > timeDif) {
			std::chrono::milliseconds duration((int) ((DISPLAY_FRAME_TIME - timeDif) * 1000));
			std::this_thread::sleep_for(duration);
		}

		//calculate and display FPS once per second
		frameCount++;
		fpsTimeSum += glfwGetTime() - loopStartTime;
		if (fpsTimeSum >= 1) {
			std::cout << (frameCount / (fpsTimeSum)) << "\n";
			frameCount = 0;
			fpsTimeSum = 0;
		}

		updateRenderThreadState();
		//std::cout << "Render  step" << std::endl;
	}
}

void SimState::resume() {
	shouldStopStateLoop = false;
	pauseSimThread = false;
	pauseMutex.unlock();
}

void SimState::pause() {
	pauseMutex.unlock();
	pauseSimThread = true;
	for(int i = 0; i < 300; i++) {
		keyState[i] = 0;
	}
}

void SimState::end() {
	endSimThread = true;
	pauseSimThread = false;
	pauseMutex.unlock();
	simThread.join();
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

void SimState::mousePosCallback(double x, double y) {
	currentScene->cameraMouseView(x, y);
}

void SimState::mouseWheelCallback(double dx, double dy) {
	currentScene->zoomCamera(dy);
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

void SimState::simThreadFunc() {

	double simStartTime;
	double simCurrentTime;
	double timeDif;
	double previousFrameStart = glfwGetTime();
	double deltaT;

	while (true) {
		if(pauseSimThread) {
			pauseMutex.lock();
			pauseMutex.unlock();
			previousFrameStart = glfwGetTime();
		}

		if(endSimThread) {
			return;
		}

		simStartTime = glfwGetTime();
		deltaT = simStartTime - previousFrameStart;

		updateSim(deltaT);

		keyOps();
		stateKeyOps();

		//clamp rate
		simCurrentTime = glfwGetTime();
		timeDif = simCurrentTime - simStartTime;
		if (SIM_TIME > timeDif) {
			std::chrono::milliseconds duration((int) ((SIM_TIME - timeDif) * 1000));
			std::this_thread::sleep_for(duration);
		}

		previousFrameStart = simStartTime;

		updateUpdateThreadState();

		//std::cout << "sim step" << std::endl;
	}
}

void SimState::updateRenderThreadState() {
	tripleBufferMutex.lock();

	currentRenderState = newestState;

	tripleBufferMutex.unlock();
}

void SimState::updateUpdateThreadState() {
	tripleBufferMutex.lock();

	newestState = currentUpdateState;

	if (newestState != 0 && currentRenderState != 0) {
		currentUpdateState = 0;
	} else if (newestState != 1 && currentRenderState != 1) {
		currentUpdateState = 1;
	} else if (newestState != 2 && currentRenderState != 2) {
		currentUpdateState = 2;
	}

	tripleBufferMutex.unlock();
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

	if (keyState[GLFW_KEY_ESCAPE]) {
		manager->changeState(StateManager::END);
		shouldStopStateLoop = true;
		keyState[GLFW_KEY_ESCAPE] = false;
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