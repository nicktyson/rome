#ifndef MAIN_H
#define MAIN_H

class GLFWwindow;

GLFWwindow* mainWindow;

void reshape(GLFWwindow* w, int width, int height);
void keyCallback(GLFWwindow* w, int key, int scancode, int state, int mods);
void mousePosCallback(GLFWwindow* w, double x, double y);
void mouseWheelCallback(GLFWwindow* w, double xOffset, double yOffset);
void setupPath();
void initShaders();
void swapBufs();

#endif