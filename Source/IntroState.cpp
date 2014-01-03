#include "IntroState.h"
#include <GLFW/glfw3.h>
#include <chrono>
#include <iostream>
#include <thread>
#include "StateManager.h"

IntroState::IntroState() {
	DISPLAY_FRAME_RATE = 30;
	DISPLAY_FRAME_TIME = 1.0 / DISPLAY_FRAME_RATE;
	initialized = false;
	shouldStopStateLoop = false;
}

void IntroState::initialize(StateManager* mngr) {
	manager = mngr;
	initialized = true;
}

void IntroState::run() {
	
	while(!shouldStopStateLoop) {
		double loopStartTime = glfwGetTime();
		glfwPollEvents();
		keyOps();
		display();
		swapBuffers();

		//clamp display frame rate
		double loopCurrentTime = glfwGetTime();
		double timeDif = loopCurrentTime - loopStartTime;
		if (DISPLAY_FRAME_TIME > timeDif) {
			std::chrono::milliseconds duration((int)((DISPLAY_FRAME_TIME - timeDif) * 1000));
			std::this_thread::sleep_for(duration);
		}
	}
}

void IntroState::resume() {
	shouldStopStateLoop = false;
}

void IntroState::pause() {
	nonifyKeys();
}

void IntroState::end() {
	
}

void IntroState::keyOps() {
	if (keyState['S'] == PRESS) {
		manager->changeState(StateManager::SIM);
		shouldStopStateLoop = true;
	}
}

void IntroState::display() {
	glClearColor(0.0f, 0.0f, 0.9f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void IntroState::updateSim(double deltaT) {

}