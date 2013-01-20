#ifndef MAIN_H
#define MAIN_H

#include <gl/glfw.h>

void initScene();
void display();
void updateSim();
void GLFWCALL simThreadFunc(void * dummy);
void GLFWCALL reshape(int width, int height);
void GLFWCALL keyCallback(int key, int state);
void keyPressed(unsigned char key, int x, int y);
void keyUp(unsigned char key, int x, int y);
void setupPath();

#endif