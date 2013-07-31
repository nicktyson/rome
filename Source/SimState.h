#ifndef SIMSTATE_H
#define SIMSTATE_H

#include <gl/glew.h>
#include <gl/glfw.h>
#include <deque>
#include "State.h"

class State;
class StateManager;
class Scene;
class Camera;
class Renderer;

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
	void mouseWheelCallback(int pos);
	void keyOps();
	void initScene();
	static void GLFWCALL startThread(void * state);
	void simThreadFunc();
	void stateKeyOps();

	static int currentRenderState;
	static int currentUpdateState;
	static int newestState;
	
protected:
	void display();
	void updateSim(double deltaT);

	void updateRenderThreadState();
	void updateUpdateThreadState();

	Renderer* renderer;
	Scene* currentScene;
	bool shouldStopStateLoop;
	bool pauseSimThread;
	bool endSimThread;
	GLFWmutex pauseMutex;
	GLFWthread simThread;
	GLFWmutex tripleBufferMutex;

	static const int DISPLAY_FRAME_RATE;
	static const double DISPLAY_FRAME_TIME;
	static const int SIM_RATE;
	static const double SIM_TIME;
};

#endif