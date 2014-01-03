#ifndef STATE_H
#define STATE_H

#include <vector>

class StateManager;
class GLFWwindow;

class State {
public:
	State();
	virtual void initialize(StateManager* mngr);
	virtual void run() = 0;
	virtual void resume() = 0;
	virtual void pause() = 0;
	virtual void end() = 0;
	virtual void keyCallback(int key, int action);
	virtual void mousePosCallback(double x, double y);
	virtual void mouseWheelCallback(double dx, double dy);
	bool isInitialized();
	void setWindow(GLFWwindow* w);

	enum KeyStates {
		PRESS,
		REPEAT,
		RELEASE,
		NONE
	};

protected:
	virtual void display() = 0;
	virtual void updateSim(double deltaT) = 0;
	static std::vector<KeyStates> keyState;
	static std::vector<int> releasedKeys;
	void nonifyKeys();
	void nonifyReleasedKeys();
	bool initialized;
	bool shouldStopStateLoop;
	StateManager* manager;
	GLFWwindow* window;
	void swapBuffers();
};

#endif