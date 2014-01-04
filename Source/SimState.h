#ifndef SIMSTATE_H
#define SIMSTATE_H

#include <gl/glew.h>
#include <glfw/glfw3.h>
#include <deque>
#include <thread>
#include <mutex>
#include <condition_variable>
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
	void mousePosCallback(double x, double y);
	void mouseWheelCallback(double dx, double dy);
	void keyOps();
	void initScene();
	void secondThreadFunc();

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

	//changed by callback; tells main thread to initiate pausing
	bool shouldPause;

	bool secondShouldPause;
	std::condition_variable shouldRun;
	std::mutex shouldRunLock;

	bool secondThreadDone;
	std::condition_variable secondThreadIsDone;
	std::mutex secondThreadDoneLock;

	std::thread secondThread;
	std::mutex tripleBufferMutex;

	static const int DISPLAY_FRAME_RATE;
	static const double DISPLAY_FRAME_TIME;
	static const int SIM_RATE;
	static const double SIM_TIME;
};

#endif