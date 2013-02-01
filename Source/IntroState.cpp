#include "IntroState.h"
#include <GL/glfw.h>
#include <iostream>
#include "StateManager.h"

IntroState::IntroState() {
	DISPLAY_FRAME_RATE = 30;
	DISPLAY_FRAME_TIME = 1.0 / DISPLAY_FRAME_RATE;
	initialized = false;
}

void IntroState::initialize(StateManager* mngr) {
	manager = mngr;
	initialized = true;
}

void IntroState::run() {
	shouldEnd = false;
	while(!shouldEnd) {
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
	}

	pause();
}

void IntroState::resume() {

}

void IntroState::pause() {
	manager->changeState(StateManager::SIM);
}

void IntroState::end() {
	
}

void IntroState::keyCallback(int key, int state) {
	if(key >= 0 && key < 256) {
		if(state == GLFW_PRESS) {
			keyState[key] = true;
		} else {
			keyState[key] = false;
		}
	}
}

void IntroState::keyOps() {
	if (keyState['S']) {
		shouldEnd = true;
		keyState['S'] = false;
	}
}

void IntroState::display() {
	glClearColor(0.0f, 0.0f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void IntroState::updateSim(double deltaT) {

}