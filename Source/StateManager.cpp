#include "StateManager.h"
#include <iostream>
#include "State.h"
#include "SimState.h"
#include "IntroState.h"
#include "PauseState.h"


StateManager::StateManager() {
	//w = window;

	intro = new IntroState();
	ps = new PauseState();
	sim = new SimState();
	

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

		currentState->pause();

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