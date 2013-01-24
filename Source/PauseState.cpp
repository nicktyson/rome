#include "PauseState.h"
#include <GL/glfw.h>
#include <iostream>
#include "StateManager.h"

PauseState::PauseState() {
	DISPLAY_FRAME_RATE = 30;
	DISPLAY_FRAME_TIME = 1.0 / DISPLAY_FRAME_RATE;
	initialized = false;
}

void PauseState::initialize(StateManager* mngr) {
	manager = mngr;
	initialized = true;
}

void PauseState::run() {
	shouldResume = false;
	while(!shouldResume) {
		double loopStartTime = glfwGetTime();

		keyOps();
		display();
		glfwPollEvents();

		//clamp display frame rate
		double loopCurrentTime = glfwGetTime();
		double timeDif = loopCurrentTime - loopStartTime;
		if (DISPLAY_FRAME_TIME > timeDif) {
			glfwSleep(DISPLAY_FRAME_TIME - timeDif);
		}
	}

	pause();
}

void PauseState::resume() {

}

void PauseState::pause() {
	manager->restoreLast();
}

void PauseState::end() {

}

void PauseState::keyCallback(int key, int state) {
	if(key >= 0 && key < 256) {
		if(state == GLFW_PRESS) {
			keyState[key] = true;
		} else {
			keyState[key] = false;
		}
	}
}

void PauseState::keyOps() {
	if (keyState['P']) {
		shouldResume = true;
		keyState['P'] = false;
	}
}

void PauseState::display() {
	//glClearColor(0.0f, 0.4f, 0.0f, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT);
}

void PauseState::updateSim() {

}