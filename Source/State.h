#ifndef STATE_H
#define STATE_H

#include <gl/glfw.h>
#include <vector>

class State {
public:
	State();
	virtual void initialize() = 0;
	virtual void run() = 0;
	virtual void resume() = 0;
	virtual void pause() = 0;
	virtual void end() = 0;
	virtual void keyCallback(int key, int state) = 0;
	virtual void simThreadFunc();
	bool isInitialized();
	static void GLFWCALL startThread(void * state);

protected:
	virtual void display() = 0;
	virtual void updateSim() = 0;
	std::vector<bool> keyState;
	bool initialized;
};


#endif