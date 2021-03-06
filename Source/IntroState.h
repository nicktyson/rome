#ifndef INTROSTATE_H
#define INTROSTATE_H

#include "State.h"

class State;
class StateManager;

class IntroState : public State {
public:
	IntroState();
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