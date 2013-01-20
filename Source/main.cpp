#include <GL/glew.h>
#include <GL/glut.h>
#include <gl/freeglut.h>
#include <Windows.h> //get exe path; CRITICAL_SECTION
#include <process.h> //_beginthread
#include <string>
#include <iostream>
#include <math.h>
#include "main.h"
#include "scene_node.h"
#include "MeshNode.h"
#include "VitalEntity.h"

//desired fps (display and sim)
//keypresses are detected at the display rate
const int DISPLAY_FRAME_RATE = 30;
const int DISPLAY_FRAME_TIME = 1000 / DISPLAY_FRAME_RATE;
const int SIM_RATE = 60;
const int SIM_TIME = 1000 / SIM_RATE;

std::string ROME_PATH;

CRITICAL_SECTION keyLock;

std::vector<bool> keyState(256);
std::vector<bool> keyStateSpecial(256);

scene_node * rootNode;
//MeshNode * cubeNode;

//contains init functions and the main loop
int main (int argc, char **argv) {
	//init glut and window
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE); // Set up a double display buffer  
	glutInitWindowSize (800, 600); 
	glutInitWindowPosition (100, 50);  
	glutCreateWindow ("Rome graphics"); // Set the title for the window

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

	//set glut functions
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyPressed);
	glutKeyboardUpFunc(keyUp);
	glutSpecialFunc(keySpecial);  
	glutSpecialUpFunc(keySpecialUp);

	//set ROME_PATH to the main project directory
	setupPath();

	//create geometry
	initScene();

	//start multithreading
	InitializeCriticalSection(&keyLock);
	_beginthread(simThreadFunc, 0, NULL);

	int frameCount = 0;
	int fpsTimeSum = 0;

	//main loop
	while (true) {
		int loopStartTime = glutGet(GLUT_ELAPSED_TIME);

		glutPostRedisplay();
		glutMainLoopEvent();

		//clamp display frame rate
		int loopCurrentTime = glutGet(GLUT_ELAPSED_TIME);
		int timeDif = loopCurrentTime - loopStartTime;
		if (DISPLAY_FRAME_TIME > timeDif) {
			Sleep(DISPLAY_FRAME_TIME - timeDif);
		}

		//calculate and display FPS once per second
		frameCount++;
		fpsTimeSum += glutGet(GLUT_ELAPSED_TIME) - loopStartTime;
		if (fpsTimeSum >= 1000) {
			std::cout << (frameCount * 1000 / (fpsTimeSum)) << "\n";
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

	glTranslatef(0.0f, 0.0f, -5.0f);

	rootNode->draw();
	
	glutSwapBuffers();
}

//physics and movement
void updateSim() {
	EnterCriticalSection(&keyLock);

	if (keyState['a']) {
		std::cout << "a" << std::endl;
		keyState['a'] = false;
	}

	LeaveCriticalSection(&keyLock);

	rootNode->update();
}

void simThreadFunc (void * dummy) {
	while (true) {
		int simStartTime = glutGet(GLUT_ELAPSED_TIME);

		updateSim();

		int simCurrentTime = glutGet(GLUT_ELAPSED_TIME);
		int timeDif = simCurrentTime - simStartTime;
		if (SIM_TIME > timeDif) {
			Sleep(SIM_TIME - timeDif);
		}
	}
}

//reshape the window
void reshape (int width, int height) {  
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);  
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();  
	//field of view, aspect ratio of window, and the new and far planes
	gluPerspective(60, (GLfloat)width / (GLfloat)height, 1.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
}

void keyPressed (unsigned char key, int x, int y) {
	EnterCriticalSection(&keyLock);
	keyState[key] = true;
	LeaveCriticalSection(&keyLock);
}

void keyUp (unsigned char key, int x, int y) {
	EnterCriticalSection(&keyLock);
	keyState[key] = false;
	LeaveCriticalSection(&keyLock);
}

void keySpecial (int key, int x, int y) {
	EnterCriticalSection(&keyLock);
	keyStateSpecial[key] = true;
	LeaveCriticalSection(&keyLock);
}

void keySpecialUp (int key, int x, int y) {
	EnterCriticalSection(&keyLock);
	keyStateSpecial[key] = false;
	LeaveCriticalSection(&keyLock);
}

void initScene() {
	rootNode = new scene_node();
	std::string location = ROME_PATH + "/Assets/Meshes/test_icosphere.msh";
	std::string cubeLocation = ROME_PATH + "/Assets/Meshes/test_cube.msh";
	//make two offset objects
	MeshNode * childNode = new MeshNode(location);
	childNode->setTranslation(1, 1, 0);
	rootNode->addChild(childNode);

	VitalEntity * secondChild = new VitalEntity(cubeLocation, 0.01, 0, 0);
	secondChild->setTranslation(-2, -2, 0);
	secondChild->setRotation(20, 45, 0);
	childNode->addChild(secondChild);


	//make a ring of spheres
	//for(double i = 0; i < 6; ++i) {
	//	MeshNode * newNode = new MeshNode(location);
	//	rootNode->addChild(newNode);
	//	newNode->setTranslation(2*sin(i*6.28/12), 2*cos(i*6.28/12), 0);
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