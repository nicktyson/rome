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
	void initScene();
	void simThreadFunc();
	void keyOps();
	
protected:
	void display();
	void updateSim();
	Camera* camera;
	int DISPLAY_FRAME_RATE;
	double DISPLAY_FRAME_TIME;
	int SIM_RATE;
	double SIM_TIME;
	bool shouldPause;
	GLFWmutex pauseMutex;
};

#endif