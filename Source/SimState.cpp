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
	nonifyKeys();
}

void SimState::end() {
	endSimThread = true;
	pauseSimThread = false;
	pauseMutex.unlock();
	simThread.join();
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
	// keyState['X'] = NONE should be used for keys that should pause briefly before repeating 
	// (ie only act on the frames when the key press is reported) or keys that should "toggle"
	// "smooth" keys like movement don't need it

	if (keyState[GLFW_KEY_P] == PRESS) {
		manager->changeState(StateManager::PAUSE);
		shouldStopStateLoop = true;
	}

	if (keyState[GLFW_KEY_ESCAPE] == PRESS) {
		manager->changeState(StateManager::END);
		shouldStopStateLoop = true;
	}

	if (keyState[GLFW_KEY_W] == PRESS || keyState[GLFW_KEY_W] == REPEAT) {
		currentScene->cameraForward();
	}
	if (keyState[GLFW_KEY_A] == PRESS || keyState[GLFW_KEY_A] == REPEAT) {
		currentScene->cameraLeft();
	}
	if (keyState[GLFW_KEY_S] == PRESS || keyState[GLFW_KEY_S] == REPEAT) {
		currentScene->cameraBack();
	}
	if (keyState[GLFW_KEY_D] == PRESS || keyState[GLFW_KEY_D] == REPEAT) {
		currentScene->cameraRight();
	}

	if (keyState[GLFW_KEY_Q] == PRESS || keyState[GLFW_KEY_Q] == REPEAT) {
		currentScene->rotateCameraCCW();
	}
	if (keyState[GLFW_KEY_E] == PRESS || keyState[GLFW_KEY_E] == REPEAT) {
		currentScene->rotateCameraCW();
	}

	if (keyState[GLFW_KEY_V] == PRESS) {
		currentScene->switchCameras();
		keyState[GLFW_KEY_V] = NONE;
	}

	nonifyReleasedKeys();
}