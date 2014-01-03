#include <GL/glew.h> //order glew -> windows -> glfw
#include <Windows.h> //get exe path
#include <glfw/glfw3.h>
#include <string>
#include <iostream>
#include "main.h"
#include "State.h"
#include "StateManager.h"
#include "ShaderProgram.h"
#include "Materials/Material.h"
#include "Materials/GreenTestMaterial.h"
#include "Materials/NormalMaterial.h"
#include "Materials\MaterialList.h"
#include "MatrixStack.h"

std::string ROME_PATH;
StateManager* manager;
MaterialList* materialList;
MatrixStack* sceneGraphMatrixStack;
MatrixStack* projectionMatrixStack;

//contains init functions and the main loop
int main(int argc, char **argv) {
	//create window
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	mainWindow = glfwCreateWindow(800, 600, "Rome graphics", NULL, NULL);
	
	glfwMakeContextCurrent(mainWindow);

	std::cout << glfwGetWindowAttrib(mainWindow, GLFW_CONTEXT_VERSION_MAJOR) << "." << glfwGetWindowAttrib(mainWindow, GLFW_CONTEXT_VERSION_MINOR) << std::endl;

	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		std::cout << "GLEW failed to initialize" << std::endl;
	}


	sceneGraphMatrixStack = new MatrixStack();
	projectionMatrixStack = new MatrixStack();

	//gets perspective correct
	reshape(mainWindow, 800, 600);

	//set ROME_PATH to the main project directory
	setupPath();

	//load and compile shaders
	initShaders();

	//move to SimState or Renderer?
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

	//creates each state and gives it the main window
	manager = new StateManager(mainWindow);

	//GLFW doesn't accept object member functions, so use local ones that redirect
	glfwSetWindowSizeCallback(mainWindow, reshape);
	glfwSetKeyCallback(mainWindow, keyCallback);
	glfwSetCursorPosCallback(mainWindow, mousePosCallback);
	glfwSetScrollCallback(mainWindow, mouseWheelCallback);

	//state management loop
	manager->run();

	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}

void swapBufs() {
	glfwSwapBuffers(mainWindow);
}

//reshape the window
void reshape (GLFWwindow* w, int width, int height) {  
	glViewport(0, 0, (GLint)width, (GLint)height);
	projectionMatrixStack->loadIdentity();
	//field of view, aspect ratio of window, and the new and far planes
	projectionMatrixStack->perspective(60.0, (float)width / (float)height, 1.0, 300.0);
}

void keyCallback(GLFWwindow* w, int key, int scancode, int state, int mods) {
	State* currentState = manager->getCurrentState();
	currentState->keyCallback(key, state);
}

void mousePosCallback(GLFWwindow* w, double x, double y) {
	State* currentState = manager->getCurrentState();
	currentState->mousePosCallback(x, y);
}

void mouseWheelCallback(GLFWwindow* w, double xOffset, double yOffset) {
	State* currentState = manager->getCurrentState();
	currentState->mouseWheelCallback(xOffset, yOffset);
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
	std::cout << ROME_PATH << std::endl;
}

void initShaders() {
	materialList = new MaterialList();
}