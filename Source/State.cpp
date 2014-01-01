#include "GL/glew.h"
#include "GLFW/glfw3.h"
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