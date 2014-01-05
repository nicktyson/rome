#include "GL\glew.h"
#include "GLFW\glfw3.h"
#include "StateManager.h"
#include <iostream>
#include "State.h"
#include "SimState.h"
#include "IntroState.h"
#include "PauseState.h"


StateManager::StateManager() {
	intro = new IntroState();
	ps = new PauseState();
	sim = new SimState();
	

	currentState = intro;
	nextState = currentState;
	lastState = NULL;

	shouldEnd = false;
}

StateManager::StateManager(GLFWwindow* w) {
	intro = new IntroState();
	ps = new PauseState();
	sim = new SimState();

	intro->setWindow(w);
	ps->setWindow(w);
	sim->setWindow(w);

	currentState = intro;
	nextState = currentState;
	lastState = NULL;

	shouldEnd = false;
}

void StateManager::run() {
	while(!shouldEnd) {
		if (!currentState->isInitialized()) {
			currentState->initialize(this);
		} else {
			currentState->resume();
		}

		currentState->run();

		lastState = currentState;
		currentState = nextState;
	}

	end();
}

void StateManager::end() {
	currentState = intro;
	ps->end();
	intro->end();
	sim->end();

	delete ps;
	//delete intro;
	delete sim;
}


State* StateManager::getCurrentState() {
	return currentState;
}

void StateManager::changeState(StateManager::States newState) {
	switch(newState) {
	case SIM:
		nextState = sim;
		break;
	case PAUSE:
		nextState = ps;
		break;
	case INTRO:
		nextState = intro;
		break;
	case LAST:
		nextState = lastState;
		lastState = NULL;
		break;
	case END:
		shouldEnd = true;
		break;
	}
}