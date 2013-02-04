#include "IntroState.h"
#include <GL/glfw.h>
#include <iostream>
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
}

void IntroState::resume() {
	shouldStopStateLoop = false;
}

void IntroState::pause() {
	for(int i = 0; i < 300; i++) {
		keyState[i] = 0;
	}
}

void IntroState::end() {
	
}

void IntroState::keyCallback(int key, int state) {
	if(key >= 0 && key < 300) {
		if(state == GLFW_PRESS) {
			keyState[key] = true;
		} else {
			keyState[key] = false;
		}
	}
}

void IntroState::keyOps() {
	if (keyState['S']) {
		manager->changeState(StateManager::SIM);
		shouldStopStateLoop = true;
		keyState['S'] = false;
	}
}

void IntroState::display() {
	glClearColor(0.0f, 0.0f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void IntroState::updateSim(double deltaT) {

}