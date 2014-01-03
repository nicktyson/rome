#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "State.h"
#include "StateManager.h"

std::vector<State::KeyStates> State::keyState;
std::vector<int> State::releasedKeys;

State::State() {
	keyState.resize(348);
}

bool State::isInitialized() {
	return initialized;
}

void State::initialize(StateManager* mngr) {

}

void State::keyCallback(int key, int action) {
	if (key >= 0 && key < 348) {
		if (action == GLFW_PRESS) {
			keyState[key] = PRESS;
		} else if (action == GLFW_REPEAT) {
			keyState[key] = REPEAT;
		} else if (action == GLFW_RELEASE) {
			keyState[key] = RELEASE;
			releasedKeys.push_back(key);
		}
	}
}

void State::nonifyKeys() {
	for (int i = 0; i < 348; i++) {
		keyState[i] = NONE;
	}
}

void State::nonifyReleasedKeys() {
	for (int key : releasedKeys) {
		keyState[key] = NONE;
	}
	releasedKeys.clear();
}

void State::mousePosCallback(double x, double y) {

}

void State::mouseWheelCallback(double dx, double dy) {

}

void State::setWindow(GLFWwindow* w) {
	window = w;
}

void State::swapBuffers() {
	glfwSwapBuffers(window);
}