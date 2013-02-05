#ifndef SIMSTATE_H
#define SIMSTATE_H

#include <gl/glfw.h>
#include "State.h"

class State;
class StateManager;
class scene_node;
class Camera;

class SimState : public State {
public:
	SimState();
	void initialize(StateManager* mngr);
	void run();
	void resume();
	void pause();
	void end();
	void keyCallback(int key, int state);
	void mousePosCallback(int x, int y);
	void keyOps();
	void initScene();
	static void GLFWCALL startThread(void * state);
	void simThreadFunc();
	
protected:
	void display();
	void updateSim(double deltaT);
	Camera* camera;
	int DISPLAY_FRAME_RATE;
	double DISPLAY_FRAME_TIME;
	int SIM_RATE;
	double SIM_TIME;
	bool shouldStopStateLoop;
	bool pauseSimThread;
	bool endSimThread;
	GLFWmutex pauseMutex;
	GLFWthread simThread;
};

#endif