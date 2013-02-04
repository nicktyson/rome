#include "State.h"
#include "StateManager.h"
#include <gl/glfw.h>

State::State() {
	keyState.resize(300);
}

bool State::isInitialized() {
	return initialized;
}

void State::initialize(StateManager* mngr) {

}