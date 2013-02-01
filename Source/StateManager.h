#ifndef STATEMANAGER_H
#define STATEMANAGER_H

#include <vector>
class State;

class StateManager {
public:
	StateManager();
	enum States {
		SIM,
		INTRO,
		PAUSE,
		LAST
	};
	State* getCurrentState();
	void run();
	void changeState(States newState);

protected:
	State* currentState;
	State* lastState;
	State* nextState;
	State* sim;
	State* ps;
	State* intro;
};

#endif