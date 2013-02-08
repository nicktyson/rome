#ifndef MAIN_H
#define MAIN_H

#include <gl/glfw.h>

void GLFWCALL reshape(int width, int height);
void GLFWCALL keyCallback(int key, int state);
void GLFWCALL mousePosCallback(int x, int y);
void GLFWCALL mouseWheelCallback(int pos);
void setupPath();

#endif