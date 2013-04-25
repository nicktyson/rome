#include <gl/glew.h>
#include <GL/glfw.h>
#include "SimState.h"
#include <iostream>
#include <vector>
#include "StateManager.h"
#include "MeshNode.h"
#include "VitalEntity.h"
#include "LightNode.h"
#include "Camera.h"
#include "FPCamera.h"
#include "TPCamera.h"
#include "Materials\MaterialList.h"
#include "MatrixStack.h"
#include "Renderer.h"

std::vector<bool> State::keyState;

//desired fps (display and sim)
//keypresses are detected at the display rate
//times are in seconds
const int SimState::DISPLAY_FRAME_RATE = 60;
const double SimState::DISPLAY_FRAME_TIME = 1.0 / DISPLAY_FRAME_RATE;
const int SimState::SIM_RATE = 120;
const double SimState::SIM_TIME = 1.0 / SIM_RATE;

SimState::SimState() {
	initialized = false;
	shouldStopStateLoop = false;
	pauseSimThread = false;
	endSimThread = false;
}

void SimState::initialize(StateManager* mngr) {
	manager = mngr;

	renderer = new Renderer();

	root = new scene_node();

	initCameras();

	initScene();
	pauseMutex = glfwCreateMutex();
	simThread = glfwCreateThread(startThread, this);
	initialized = true;
}

void SimState::run() {
	
	int frameCount = 0;
	double fpsTimeSum = 0;

	while (!shouldStopStateLoop) {
		double loopStartTime = glfwGetTime();

		keyOps();
		display();
		glfwSwapBuffers();

		//clamp display frame rate
		double loopCurrentTime = glfwGetTime();
		double timeDif = loopCurrentTime - loopStartTime;
		if (DISPLAY_FRAME_TIME > timeDif) {
			glfwSleep(DISPLAY_FRAME_TIME - timeDif);
		}

		//calculate and display FPS once per second
		frameCount++;
		fpsTimeSum += glfwGetTime() - loopStartTime;
		if (fpsTimeSum >= 1) {
			std::cout << (frameCount / (fpsTimeSum)) << "\n";
			frameCount = 0;
			fpsTimeSum = 0;
		}
	}
}

void SimState::resume() {
	shouldStopStateLoop = false;
	pauseSimThread = false;
	glfwUnlockMutex(pauseMutex);
}

void SimState::pause() {
	glfwLockMutex(pauseMutex);
	pauseSimThread = true;
	for(int i = 0; i < 300; i++) {
		keyState[i] = 0;
	}
}

void SimState::end() {
	endSimThread = true;
	pauseSimThread = false;
	glfwUnlockMutex(pauseMutex);
	glfwWaitThread(simThread, GLFW_WAIT);
}

void SimState::keyCallback(int key, int state) {
	if(key >= 0 && key < 300) {
		if(state == GLFW_PRESS) {
			keyState[key] = true;
		} else {
			keyState[key] = false;
		}
	}
}

void SimState::mousePosCallback(int x, int y) {
	camera->mouseView(x, y);
}

void SimState::mouseWheelCallback(int pos) {
	camera->zoom(pos);
}

void SimState::initScene() {
	extern std::string ROME_PATH;

	std::string location = ROME_PATH + "/Assets/Meshes/hires_icosphere.msh";
	std::string cubeLocation = ROME_PATH + "/Assets/Meshes/test_cube.msh";
	std::string castleLocation = ROME_PATH + "/Assets/Meshes/castle.msh";
	std::string quadLocation = ROME_PATH + "/Assets/Meshes/test_quad.msh";

	std::string testTextureLocation = ROME_PATH + "/Assets/Textures/test.tga";

	//make two offset objects
	MeshNode * childNode = new MeshNode(cubeLocation, MaterialList::GREENTEST);
	childNode->setTranslation(0, 0, 0);
	root->addChild(childNode);

	//float randRot = rand() % 90;
	VitalEntity * secondChild = new VitalEntity(location, 1, 0, 0);
	secondChild->setTranslation(2, 2, 0);
	//secondChild->setRotation(20, randRot, 0);
	childNode->addChild(secondChild);

	//add a floor
	MeshNode* floor = new MeshNode(quadLocation, MaterialList::LAMBERTIAN);
	floor->setMaterialProperties(1.0, 1.0, 1.0, 1.0, 1.0, testTextureLocation);
	floor->setScaling(5, 5, 1.0);
	//floor->setTranslation(0, 0, -1.1);
	root->addChild(floor);

	//add a castle
	//MeshNode* castle = new MeshNode(castleLocation, MaterialList::LAMBERTIAN);
	//castle->setScaling(2.0, 2.0, 2.0);
	//castle->setTranslation(0.0, 0.0, -2.0);
	//root->addChild(castle);

	//make a ring of spheres
	for(double i = 0; i < 6; ++i) {
		MeshNode * newNode = new MeshNode(location, MaterialList::BLINNPHONG);
		newNode->setMaterialProperties(0.0, 0.4, 0.0, 40.0, 1.0);
		root->addChild(newNode);
		newNode->setTranslation(3*sin(i*6.28/6), 3*cos(i*6.28/6), 0);
	}

	//add a light
	LightNode* light = new LightNode();
	light->setTranslation(16.0, 16.0, 6.0);
	root->addChild(light);

	//a second (blue) light to test shaders
	//LightNode* light2 = new LightNode();
	//light2->setTranslation(-16.0, -16.0, 6.0);
	//light2->color[0] = 0.0;
	//light2->color[1] = 0.0;
	//root->addChild(light2);
}

void SimState::initCameras() {
	cameraType = THIRDPERSON;

	camera = new TPCamera();
	camera->addChild(root);
	cameras.push_back(camera);

	camera = new FPCamera();
	camera->addChild(root);
	cameras.push_back(camera);

	camera = cameras.front();
	cameras.pop_front();
}

void SimState::display() {
	renderer->render(camera);	
}

void SimState::updateSim(double deltaT) {
	camera->update(deltaT);
}

void SimState::startThread(void * state) {
	SimState * stptr = static_cast<SimState*>(state);
	stptr->simThreadFunc();
}

void SimState::simThreadFunc() {

	double simStartTime;
	double simCurrentTime;
	double timeDif;
	double previousFrameStart = glfwGetTime();
	double deltaT;

	while (true) {
		if(pauseSimThread) {
			glfwLockMutex(pauseMutex);
			glfwUnlockMutex(pauseMutex);
			previousFrameStart = glfwGetTime();
		}

		if(endSimThread) {
			return;
		}

		stateKeyOps();

		simStartTime = glfwGetTime();
		deltaT = simStartTime - previousFrameStart;

		updateSim(deltaT);

		//clamp rate
		simCurrentTime = glfwGetTime();
		timeDif = simCurrentTime - simStartTime;
		if (SIM_TIME > timeDif) {
			glfwSleep(SIM_TIME - timeDif);
		}

		previousFrameStart = simStartTime;
	}
}

void SimState::keyOps() {
	//keyState['X'] = false should be used for toggle keys, like camera modes
	//it should also be used for keys that should pause briefly before repeating
	//remove it to for keys that should repeat smoothly, every frame, like movement
	//Modifier keys should NOT have the line
	if (keyState['N'] && keyState['B']) {
		std::cout << "N + B" << std::endl;
		keyState['N'] = false;
	} else if (keyState['N']) {
		std::cout << "N" << std::endl;
		keyState['N'] = false;
	} else if (keyState['B']) {
		//do nothing; modifier key
	}

	if (keyState['P']) {
		manager->changeState(StateManager::PAUSE);
		shouldStopStateLoop = true;
		keyState['P'] = false;
	}

	if (keyState[GLFW_KEY_ESC]) {
		manager->changeState(StateManager::END);
		shouldStopStateLoop = true;
		keyState[GLFW_KEY_ESC] = false;
	}
}

void SimState::stateKeyOps() {
	if (keyState['W']) {
		camera->forward();
	}
	if (keyState['A']) {
		camera->left();
	}
	if (keyState['S']) {
		camera->back();
	}
	if (keyState['D']) {
		camera->right();
	}

	if (keyState['Q']) {
		switch (cameraType) {
		case FIRSTPERSON:
			break;
		case THIRDPERSON:
			camera->rotateCCW();
			break;
		}

	}
	if (keyState['E']) {
		switch (cameraType) {
		case FIRSTPERSON:
			break;
		case THIRDPERSON:
			camera->rotateCW();
			break;
		}
	}

	if (keyState['V']) {
		switchCameras();
		keyState['V'] = false;
	}
}

void SimState::switchCameras() {
	cameras.push_back(camera);
	camera = cameras.front();
	cameras.pop_front();

	switch(cameraType) {
	case FIRSTPERSON:
		cameraType = THIRDPERSON;
		break;
	case THIRDPERSON:
		cameraType = FIRSTPERSON;
		break;
	}
}