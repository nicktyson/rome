#ifndef MAIN_H
#define MAIN_H

void initScene();
void display();
void updateSim();
void simThreadFunc(void *);
void reshape(int width, int height);
void keyPressed(unsigned char key, int x, int y);
void keyUp(unsigned char key, int x, int y);
void keySpecial(int key, int x, int y);
void keySpecialUp(int key, int x, int y);

#endif