#ifndef MAIN_H
#define MAIN_H

#include <gl/glfw.h>

void GLFWCALL simThreadFunc(void * dummy);
void GLFWCALL reshape(int width, int height);
void GLFWCALL keyCallback(int key, int state);
void setupPath();

#endif