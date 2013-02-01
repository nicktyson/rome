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
}

void StateManager::run() {
	while(true) {
		if (!currentState->isInitialized()) {
			currentState->initialize(this);
		}

		currentState->run();

		//if(lastState) {
		//	lastState->end();
		//	delete lastState;
		//}

		lastState = currentState;
		currentState = nextState;
	}
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
	}
}