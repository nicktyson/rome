#ifndef INTROSTATE_H
#define INTROSTATE_H

#include <glfw/glfw3.h>
#include "State.h"

class State;
class StateManager;

void swapBufs();

class IntroState : public State {
public:
	IntroState();
	void initialize(StateManager* mngr);
	void run();
	void resume();
	void pause();
	void end();
	void keyCallback(int key, int state);
	void keyOps();
	
protected:
	void display();
	void updateSim(double deltaT);
	int DISPLAY_FRAME_RATE;
	double DISPLAY_FRAME_TIME;
};

#endif