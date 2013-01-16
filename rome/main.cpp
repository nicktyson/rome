#include <GL/glew.h> // GLEW
#include <GL/glut.h> // GLUT
#include <gl/freeglut.h> //FreeGLUT
//#include <Windows.h>
#include <process.h>
#include <string>
#include <iostream>
#include "main.h"
#include "MeshNode.h"

//desired fps (display and sim)
int DISPLAY_FRAME_RATE = 30;
int DISPLAY_FRAME_TIME = 1000 / DISPLAY_FRAME_RATE;
int SIM_RATE = 60;
int SIM_TIME = 1000 / SIM_RATE;

scene_node * rootNode;
MeshNode * cubeNode;

//contains init functions and the main loop
int main (int argc, char **argv) {
	glutInit(&argc, argv); // Initialize GLUT  
	glutInitDisplayMode (GLUT_DOUBLE); // Set up a double display buffer  
	glutInitWindowSize (500, 500); 
	glutInitWindowPosition (100, 150);  
	glutCreateWindow ("Rome graphics"); // Set the title for the window

	glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

	//setup glut functions
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyPressed);
	glutKeyboardUpFunc(keyUp);
	glutSpecialFunc(keySpecial);  
	glutSpecialUpFunc(keySpecialUp);

	//create geometry
	initScene();

	//start physics and movement thread
	_beginthread(simThreadFunc, 0, (void *)0);

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
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glTranslatef(0.0f, 0.0f, -5.0f);

	rootNode->draw();
	
	glutSwapBuffers();
}

//physics and movement
void updateSim() {

}

void simThreadFunc (void *) {
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
	glViewport(0, 0, (GLsizei)width, (GLsizei)height); // Set our viewport to the size of our window  
	glMatrixMode(GL_PROJECTION);  // need to adjust the project matrix
	glLoadIdentity();  
	gluPerspective(60, (GLfloat)width / (GLfloat)height, 1.0, 100.0); // Set the Field of view angle (in degrees), the aspect ratio of our window, and the new and far planes  
	glMatrixMode(GL_MODELVIEW);  
}

void keyPressed (unsigned char key, int x, int y) {

}

void keyUp (unsigned char key, int x, int y) {

}

void keySpecial (int key, int x, int y) {

}

void keySpecialUp (int key, int x, int y) {

}

void initScene() {
	//C:/Users/Nick/Desktop/meshtest.msh
	rootNode = new scene_node();

	cubeNode = new MeshNode();
	mesh * cubeMesh = new mesh();
	std::string location = "C:/Users/Nick/Desktop/meshtest.msh";
	cubeMesh->loadMesh(location);
	cubeNode->setMesh(cubeMesh);

	rootNode->addChild(cubeNode);
}