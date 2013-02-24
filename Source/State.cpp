#include "State.h"
#include "StateManager.h"

State::State() {
	keyState.resize(300);
}

bool State::isInitialized() {
	return initialized;
}

void State::initialize(StateManager* mngr) {

}

void State::mousePosCallback(int x, int y) {

}

void State::mouseWheelCallback(int pos) {

}