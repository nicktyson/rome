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
	void newSim();
	State* getCurrentState();

protected:
	State* currentState;
	State* lastState;
	State* nextState;
	std::vector<State*> states;
	State* sim;
	State* ps;
	State* intro;
};

#endif