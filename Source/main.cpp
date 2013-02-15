#include <GL/glew.h> //order glew -> windows -> glfw
#include <Windows.h> //get exe path
#include <GL/glfw.h>
#include <string>
#include <iostream>
#include "main.h"
#include "State.h"
#include "StateManager.h"
#include "ShaderProgram.h"
#include "Materials/GreenTestMaterial.h"

//desired fps (display and sim)
//keypresses are detected at the display rate
//times are in seconds

std::string ROME_PATH;
StateManager* manager;
GreenTestMaterial* material;

//contains init functions and the main loop
int main(int argc, char **argv) {
	//create window
	glfwInit();
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 2);
	glfwOpenWindow(800, 600, 8, 8, 8, 8, 24, 8, GLFW_WINDOW);
	glfwSetWindowTitle("Rome graphics");
	
	glewInit();

	//gets perspective correct
	//reshape(800, 600);

	//set ROME_PATH to the main project directory
	setupPath();

	//load and compile shaders
	initShaders();

	//move to SimState?
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

	manager = new StateManager();

	glfwEnable(GLFW_KEY_REPEAT);
	//GLFW doesn't accept object member functions, so use local ones that redirect
	glfwSetWindowSizeCallback(reshape);
	glfwSetKeyCallback(keyCallback);
	glfwSetMousePosCallback(mousePosCallback);
	glfwSetMouseWheelCallback(mouseWheelCallback);

	//state management loop
	manager->run();

	//calls KeyCallback apparently
	glfwCloseWindow();
	glfwTerminate();
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

void GLFWCALL mousePosCallback(int x, int y) {
	State* currentState = manager->getCurrentState();
	currentState->mousePosCallback(x, y);
}

void GLFWCALL mouseWheelCallback(int pos) {
	State* currentState = manager->getCurrentState();
	currentState->mouseWheelCallback(pos);
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

void initShaders() {
	material = new GreenTestMaterial();
}