#include "PauseState.h"
#include <GLfw/glfw3.h>
#include <chrono>
#include <iostream>
#include <thread>
#include "StateManager.h"

PauseState::PauseState() {
	DISPLAY_FRAME_RATE = 30;
	DISPLAY_FRAME_TIME = 1.0 / DISPLAY_FRAME_RATE;
	initialized = false;
	shouldStopStateLoop = false;
}

void PauseState::initialize(StateManager* mngr) {
	manager = mngr;
	initialized = true;
}

void PauseState::run() {
	
	while(!shouldStopStateLoop) {
		double loopStartTime = glfwGetTime();

		keyOps();
		display();
		glfwPollEvents();

		//clamp display frame rate
		double loopCurrentTime = glfwGetTime();
		double timeDif = loopCurrentTime - loopStartTime;
		if (DISPLAY_FRAME_TIME > timeDif) {
			std::chrono::milliseconds duration((int)((DISPLAY_FRAME_TIME - timeDif) * 1000));
			std::this_thread::sleep_for(duration);
		}
	}

}

void PauseState::resume() {
	shouldStopStateLoop = false;
}

void PauseState::pause() {
	for(int i = 0; i < 300; i++) {
		keyState[i] = 0;
	}
}

void PauseState::end() {

}

void PauseState::keyCallback(int key, int state) {
	if(key >= 0 && key < 300) {
		if(state == GLFW_PRESS) {
			keyState[key] = true;
		} else {
			keyState[key] = false;
		}
	}
}

void PauseState::keyOps() {
	if (keyState['P']) {
		manager->changeState(StateManager::LAST);
		shouldStopStateLoop = true;
		keyState['P'] = false;
	}
}

void PauseState::display() {
	//glClearColor(0.0f, 0.4f, 0.0f, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT);
}

void PauseState::updateSim(double deltaT) {

}