#ifndef STATEMANAGER_H
#define STATEMANAGER_H

#include <vector>
#include "State.h"

class StateManager {
public:
	StateManager();
	void run();
	void pause();
	void restoreLast();
	void newSim();
private:
	State * currentState;
	State * lastState;
	State * nextState;
	std::vector<State*> states;
	State * sim;
	State * ps;
};

#endif