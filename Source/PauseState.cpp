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
	//glfwMakeContextCurrent(window);
}

void PauseState::run() {
	
	while(!shouldStopStateLoop) {
		double loopStartTime = glfwGetTime();

		keyOps();
		display();
		swapBuffers();
		glfwPollEvents();

		//clamp display frame rate
		double loopCurrentTime = glfwGetTime();
		double timeDif = loopCurrentTime - loopStartTime;
		if (DISPLAY_FRAME_TIME > timeDif) {
			std::chrono::milliseconds duration((int)((DISPLAY_FRAME_TIME - timeDif) * 1000));
			std::this_thread::sleep_for(duration);
		}
	}

	nonifyKeys();
}

void PauseState::resume() {
	shouldStopStateLoop = false;
	//glfwMakeContextCurrent(window);
}

void PauseState::end() {

}

void PauseState::keyOps() {
	if (keyState[GLFW_KEY_P] == PRESS) {
		manager->changeState(StateManager::LAST);
		shouldStopStateLoop = true;
	}

	nonifyReleasedKeys();
}

void PauseState::display() {
	glClearColor(0.0f, 0.4f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	//std::cout << "pause state drawing" << std::endl;
}

void PauseState::updateSim(double deltaT) {

}