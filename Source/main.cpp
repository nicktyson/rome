#include <GL/glew.h> //order glew -> windows -> glfw
#include <Windows.h> //get exe path
#include <GL/glfw.h>
#include <string>
#include <iostream>
#include "main.h"
#include "State.h"
#include "StateManager.h"

//desired fps (display and sim)
//keypresses are detected at the display rate
//times are in seconds

std::string ROME_PATH;
StateManager* manager;

//contains init functions and the main loop
int main(int argc, char **argv) {
	//create window
	glfwInit();
	glfwOpenWindow(800, 600, 8, 8, 8, 8, 24, 8, GLFW_WINDOW);
	glfwSetWindowTitle("Rome graphics");

	//gets perspective correct
	//reshape(800, 600);

	//move to SimState?
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

	glfwEnable(GLFW_KEY_REPEAT);

	//GLFW doesn't accept object member functions, so use local ones that redirect
	glfwSetWindowSizeCallback(reshape);
	glfwSetKeyCallback(keyCallback);

	//set ROME_PATH to the main project directory
	setupPath();

	//state management loop
	manager = new StateManager();
	manager->run();

}

//reshape the window
void GLFWCALL reshape (int width, int height) {  
	glViewport(0, 0, (GLint)width, (GLint)height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();  
	//field of view, aspect ratio of window, and the new and far planes
	gluPerspective(60, (GLfloat)width / (GLfloat)height, 1.0, 50.0);
	glMatrixMode(GL_MODELVIEW);
}

void GLFWCALL keyCallback(int key, int state) {
	State* currentState = manager->getCurrentState();
	currentState->keyCallback(key, state);
}

void setupPath() {
	char pathBuffer[MAX_PATH];
	GetModuleFileNameA(NULL, pathBuffer, MAX_PATH);
	std::string exePath(pathBuffer);

	//remove rome.exe and move up two folders
	exePath = exePath.substr(0, exePath.find_last_of("\\/"));
	exePath = exePath.substr(0, exePath.find_last_of("\\/"));
	exePath = exePath.substr(0, exePath.find_last_of("\\/"));

	//change backslashes to forward slash
	size_t subIndex = 0;
	while (true) {
		subIndex = exePath.find("\\");
		if (subIndex == std::string::npos) {
			break;
		}
		exePath.replace(subIndex, 1, "/");
	}

	ROME_PATH = exePath;
}