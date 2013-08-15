#ifndef SCENE_H
#define SCENE_H

#include <string>
#include <deque>
#include "../Lib/glm/glm.hpp"
#include "scene_node.h"
#include "Renderer.h"

class Camera;
class Skybox;

class Scene {
public:
	Scene();
	virtual void init();

	enum Cameras {
		FIRSTPERSON,
		THIRDPERSON
	};
	void switchCameras();
	void rotateCameraCW();
	void rotateCameraCCW();
	void cameraForward();
	void cameraBack();
	void cameraRight();
	void cameraLeft();
	void zoomCamera(int pos);
	void cameraMouseView(int x, int y);
	Camera* getCurrentCamera();

	void setCubeMap(std::string cubeMapName);
	void setRoot(scene_node* rootNode);
	void setSkybox(Skybox* skybx);

	glm::mat4 getEyeToWorldNormalMatrix();
	GLuint getCubeMap();
	void update(double deltaT);
	virtual void stateUpdate(double deltaT);
	void draw(Renderer* r, bool isTransparentPass);

protected:
	void initCameras();
	virtual void initScene();
	
	struct scene_State {
		Cameras cameraType;
		Camera* camera;
	
		scene_node* sceneRoot;
		GLuint cubeMap;

		bool hasSkybox;
		Skybox* skybox;
	};

	std::vector<scene_State> scene_states;

	std::deque<Camera*> cameras;
};

#endif