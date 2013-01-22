#ifndef SIMSTATE_H
#define SIMSTATE_H

#include "State.h"
#include "scene_node.h"

class SimState : public State {
public:
	SimState();
	void initialize();
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
	scene_node * rootNode;
	int DISPLAY_FRAME_RATE;
	double DISPLAY_FRAME_TIME;
	int SIM_RATE;
	double SIM_TIME;
};

#endif