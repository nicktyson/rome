#ifndef PAUSESTATE_H
#define PAUSESTATE_H

#include <gl/glfw.h>
#include "State.h"

class State;
class StateManager;

class PauseState : public State {
public:
	PauseState();
	void initialize(StateManager* mngr);
	void run();
	void resume();
	void pause();
	void end();
	void keyCallback(int key, int state);
	void keyOps();
	
protected:
	void display();
	void updateSim();
	int DISPLAY_FRAME_RATE;
	double DISPLAY_FRAME_TIME;
	bool shouldResume;
};

#endif