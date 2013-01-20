#include <GL/glew.h> //order glew -> windows -> glfw
#include <Windows.h> //get exe path
#include <GL/glfw.h>
#include <string>
#include <iostream>
#include <math.h>
#include "main.h"
#include "scene_node.h"
#include "MeshNode.h"
#include "VitalEntity.h"

//desired fps (display and sim)
//keypresses are detected at the display rate
//times are in seconds
const int DISPLAY_FRAME_RATE = 30;
const double DISPLAY_FRAME_TIME = 1.0 / DISPLAY_FRAME_RATE;
const int SIM_RATE = 60;
const double SIM_TIME = 1.0 / SIM_RATE;

std::string ROME_PATH;

GLFWmutex keyLock;

std::vector<bool> keyState(256);
std::vector<bool> keyStateSpecial(256);

scene_node * rootNode;

//contains init functions and the main loop
int main (int argc, char **argv) {
	//create window
	glfwInit();
	glfwOpenWindow(800, 600, 8, 8, 8, 8, 24, 8, GLFW_WINDOW);
	glfwSetWindowTitle("Rome graphics");

	//gets perspective correct
	//reshape(800, 600);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

	glfwEnable(GLFW_KEY_REPEAT);

	glfwSetWindowSizeCallback(reshape);
	glfwSetKeyCallback(keyCallback);

	//set ROME_PATH to the main project directory
	setupPath();

	//create geometry
	initScene();

	//start multithreading
	keyLock = glfwCreateMutex();
	GLFWthread simThread;
	simThread = glfwCreateThread(simThreadFunc, NULL);

	int frameCount = 0;
	double fpsTimeSum = 0;

	//main loop
	while (true) {
		double loopStartTime = glfwGetTime();

		//glutPostRedisplay();
		//glutMainLoopEvent();

		display();
		glfwSwapBuffers();

		//clamp display frame rate
		double loopCurrentTime = glfwGetTime();
		double timeDif = loopCurrentTime - loopStartTime;
		if (DISPLAY_FRAME_TIME > timeDif) {
			glfwSleep(DISPLAY_FRAME_TIME - timeDif);
		}

		//calculate and display FPS once per second
		frameCount++;
		fpsTimeSum += glfwGetTime() - loopStartTime;
		if (fpsTimeSum >= 1) {
			std::cout << (frameCount / (fpsTimeSum)) << "\n";
			frameCount = 0;
			fpsTimeSum = 0;
		}
	}
}

//traverse the scene graph and draw everything
void display() {
	glClearColor(0.4f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	gluLookAt(0, 0, 5, 0, 0, 0, 0, 1, 0);

	rootNode->draw();
}

//physics and movement
void updateSim() {
	glfwLockMutex(keyLock);

	if (keyState['A']) {
		std::cout << "A" << std::endl;
		keyState['A'] = false;
	}

	glfwUnlockMutex(keyLock);

	rootNode->update();
}

void GLFWCALL simThreadFunc (void * dummy) {
	while (true) {
		double simStartTime = glfwGetTime();

		updateSim();

		double simCurrentTime = glfwGetTime();
		double timeDif = simCurrentTime - simStartTime;
		if (SIM_TIME > timeDif) {
			glfwSleep(SIM_TIME - timeDif);
		}
	}
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
	glfwLockMutex(keyLock);
	if(key >= 0 && key < 256 && state == GLFW_PRESS) {
		keyState[key] = true;
	}
	glfwUnlockMutex(keyLock);
}

void initScene() {
	rootNode = new scene_node();
	std::string location = ROME_PATH + "/Assets/Meshes/test_icosphere.msh";
	std::string cubeLocation = ROME_PATH + "/Assets/Meshes/test_cube.msh";

	//make two offset objects
	MeshNode * childNode = new MeshNode(location);
	childNode->setTranslation(1, 1, 0);
	rootNode->addChild(childNode);

	VitalEntity * secondChild = new VitalEntity(cubeLocation, 0, 0, 0);
	secondChild->setTranslation(-2, -2, 0);
	secondChild->setRotation(20, 45, 0);
	childNode->addChild(secondChild);


	//make a ring of spheres
	//for(double i = 0; i < 6; ++i) {
	//	MeshNode * newNode = new MeshNode(location);
	//	rootNode->addChild(newNode);
	//	newNode->setTranslation(3*sin(i*6.28/6), 3*cos(i*6.28/6), 0);
	//}
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