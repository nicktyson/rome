#include "StateManager.h"
#include "SimState.h"
#include "State.h"

StateManager::StateManager() {
	sim = new SimState();
	currentState = sim;
	nextState = currentState;
	lastState = currentState;
}

void StateManager::run() {
	while(true) {
		if (!currentState->isInitialized()) {
			currentState->initialize(this);
		}

		currentState->run();

		lastState = currentState;
		currentState = nextState;
	}
}

void StateManager::pause() {

}

void StateManager::restoreLast() {
	nextState = lastState;
}

void StateManager::newSim() {
	nextState = new SimState();
}

State* StateManager::getCurrentState() {
	return currentState;
}