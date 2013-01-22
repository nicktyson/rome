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
#include "State.h"
#include "SimState.h"

//desired fps (display and sim)
//keypresses are detected at the display rate
//times are in seconds
const int DISPLAY_FRAME_RATE = 30;
const double DISPLAY_FRAME_TIME = 1.0 / DISPLAY_FRAME_RATE;
const int SIM_RATE = 60;
const double SIM_TIME = 1.0 / SIM_RATE;

std::string ROME_PATH;

State * currentState;

//contains init functions and the main loop
int main(int argc, char **argv) {
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

	//start multithreading
	//GLFWthread simThread;
	//simThread = glfwCreateThread(simThreadFunc, NULL);

	int frameCount = 0;
	double fpsTimeSum = 0;

	//main loop
	while (true) {
		double loopStartTime = glfwGetTime();

		//keyOps();
		//display();
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

void GLFWCALL simThreadFunc (void * dummy) {
	while (true) {
		double simStartTime = glfwGetTime();

		//updateSim();

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