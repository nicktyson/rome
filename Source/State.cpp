#include "State.h"
#include <gl/glfw.h>

State::State() {
	keyState.resize(256);
}

void State::startThread(State * state) {
	state->simThreadFunc();
}

void State::simThreadFunc(void * dummy) {

}