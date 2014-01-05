#ifndef PAUSESTATE_H
#define PAUSESTATE_H

#include "State.h"

class State;
class StateManager;

class PauseState : public State {
public:
	PauseState();
	void initialize(StateManager* mngr);
	void run();
	void resume();
	void end();
	void keyOps();
	
protected:
	void display();
	void updateSim(double deltaT);
	int DISPLAY_FRAME_RATE;
	double DISPLAY_FRAME_TIME;
};

#endif