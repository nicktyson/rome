#include "State.h"
#include <gl/glfw.h>

State::State() {
	keyState.resize(256);
}

void State::startThread(void * state) {
	State * stptr = static_cast<State*>(state);
	stptr->simThreadFunc();
}

void State::simThreadFunc() {

}

bool State::isInitialized() {
	return initialized;
}