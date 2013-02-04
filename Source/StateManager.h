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
		LAST,
		END
	};
	State* getCurrentState();
	void run();
	void changeState(States newState);
	void end();

protected:
	State* currentState;
	State* lastState;
	State* nextState;
	State* sim;
	State* ps;
	State* intro;
	bool shouldEnd;
};

#endif