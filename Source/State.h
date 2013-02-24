#ifndef STATE_H
#define STATE_H

#include <vector>

class StateManager;

class State {
public:
	State();
	virtual void initialize(StateManager* mngr);
	virtual void run() = 0;
	virtual void resume() = 0;
	virtual void pause() = 0;
	virtual void end() = 0;
	virtual void keyCallback(int key, int state) = 0;
	virtual void mousePosCallback(int x, int y);
	virtual void mouseWheelCallback(int pos);
	bool isInitialized();

protected:
	virtual void display() = 0;
	virtual void updateSim(double deltaT) = 0;
	static std::vector<bool> keyState;
	bool initialized;
	bool shouldStopStateLoop;
	StateManager* manager;
};


#endif