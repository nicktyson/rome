#include "StateManager.h"
#include "SimState.h"
#include "State.h"
#include "IntroState.h"
#include "PauseState.h"

StateManager::StateManager() {
	intro = new IntroState();
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

		if(lastState) {
			lastState->end();
			delete lastState;
		}
		lastState = currentState;
		currentState = nextState;
	}
}

void StateManager::pause() {
	nextState = new PauseState();
}

void StateManager::restoreLast() {
	nextState = lastState;
	lastState = NULL;
}

void StateManager::newSim() {
	nextState = new SimState();
}

State* StateManager::getCurrentState() {
	return currentState;
}