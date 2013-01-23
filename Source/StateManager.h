#ifndef STATEMANAGER_H
#define STATEMANAGER_H

#include <vector>
class State;

class StateManager {
public:
	StateManager();
	void run();
	void pause();
	void restoreLast();
	State* getCurrentState();
	void newSim();

protected:
	State* currentState;
	State* lastState;
	State* nextState;
	std::vector<State*> states;
	State* sim;
	State* ps;
};

#endif