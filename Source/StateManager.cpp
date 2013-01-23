#include "StateManager.h"
#include "SimState.h"

StateManager::StateManager() {
	sim = new SimState();
	currentState = sim;
	nextState = currentState;
	lastState = currentState;
}

void StateManager::run() {
	while(true) {
		if (!currentState->isInitialized()) {
			currentState->initialize();
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