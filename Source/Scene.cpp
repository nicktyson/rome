#include "Scene.h"
#include <string>
#include "Camera.h"
#include "FPCamera.h"
#include "TPCamera.h"
#include "scene_node.h"
#include "Skybox.h"
#include "MatrixStack.h"
#include "SimState.h"
#include "TextureManager.h"
#include "../Lib/stb_image.h"
#include "../Lib/glm/glm.hpp"
#include "../Lib/glm/gtc/matrix_inverse.hpp"

Scene::Scene() : scene_states(3) {
	for (int i = 0; i < 3; i++) {
		scene_State* state = &scene_states[i];
		//cubeMap = 0;
		state->sceneRoot = new scene_node();
		state->hasSkybox = false;
	}
}

void Scene::init() {
	initScene();
	initCameras();
}

void Scene::initCameras() {
	scene_State* currentState = &scene_states[SimState::currentUpdateState];

	Camera* tmpCam = new TPCamera();

	tmpCam->addSceneRoot(currentState->sceneRoot);
	cameras.push_back(tmpCam);

	tmpCam = new FPCamera();
	tmpCam->addSceneRoot(currentState->sceneRoot);
	cameras.push_back(tmpCam);

	for (int i = 0; i < 3; i++) {
		scene_State* state = &scene_states[i];
		state->camera = cameras.front();
		state->cameraType = THIRDPERSON;
	}
	
	cameras.pop_front();
}

void Scene::initScene() {

}

void Scene::setCubeMap(std::string cubeMapName) {
	//delete existing cubemap
	/*if (cubeMap != 0) {
		glDeleteTextures(1, &cubeMap);
	}*/
	
	//create the 6 filename strings
	std::string name = cubeMapName;
	std::string extension = ".png";

	unsigned periodPos = cubeMapName.find_last_of(".");
	if (periodPos != std::string::npos) {
		name = name.substr(0, periodPos);
		extension = name.substr(periodPos, name.length() - periodPos);
	}
	
	std::string px = name + "_px" + extension;
	std::string nx = name + "_nx" + extension;
	std::string py = name + "_py" + extension;
	std::string ny = name + "_ny" + extension;
	std::string pz = name + "_pz" + extension;
	std::string nz = name + "_nz" + extension;

	//generate and fill the texture
	GLuint location;
	glGenTextures(1, &location);
	glBindTexture(GL_TEXTURE_CUBE_MAP, location);

	int x, y, n;
	unsigned char* pixelData = TextureManager::loadImage(px, &x, &y, &n, 3);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB16F, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*) pixelData);
	stbi_image_free(pixelData);

	pixelData = TextureManager::loadImage(nx, &x, &y, &n, 3);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB16F, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*) pixelData);
	stbi_image_free(pixelData);

	pixelData = TextureManager::loadImage(py, &x, &y, &n, 3);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB16F, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*) pixelData);
	stbi_image_free(pixelData);

	pixelData = TextureManager::loadImage(ny, &x, &y, &n, 3);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB16F, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*) pixelData);
	stbi_image_free(pixelData);

	pixelData = TextureManager::loadImage(pz, &x, &y, &n, 3);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB16F, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*) pixelData);
	stbi_image_free(pixelData);

	pixelData = TextureManager::loadImage(nz, &x, &y, &n, 3);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB16F, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*) pixelData);
	stbi_image_free(pixelData);

	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	scene_State* currentState = &scene_states[SimState::currentUpdateState];
	currentState->cubeMap = location;
}

void Scene::setRoot(scene_node* rootNode) {
	scene_State* currentState = &scene_states[SimState::currentUpdateState];
	currentState->sceneRoot = rootNode;
}

void Scene::setSkybox(Skybox* skybx) {
	scene_State* currentState = &scene_states[SimState::currentUpdateState];

	currentState->hasSkybox = true;
	currentState->skybox = skybx;
}

GLuint Scene::getCubeMap() {
	scene_State* currentState = &scene_states[SimState::currentRenderState];

	return currentState->cubeMap;
}

glm::mat4 Scene::getEyeToWorldNormalMatrix() {
	scene_State* currentState = &scene_states[SimState::currentRenderState];

	return currentState->camera->getInverseNormalMatrix();
}

void Scene::draw(Renderer* r, bool isTransparentPass) {
	scene_State* currentState = &scene_states[SimState::currentRenderState];
	
	currentState->camera->draw(r, isTransparentPass);

	if (currentState->hasSkybox) {
		currentState->skybox->draw(r, isTransparentPass);
	}
}

void Scene::update(double deltaT) {
	scene_State* currentState = &scene_states[SimState::currentUpdateState];
	
	stateUpdate(deltaT);

	currentState->camera->update(deltaT);

	if (currentState->hasSkybox) {
		currentState->skybox->update(deltaT);
	}
}

void Scene::stateUpdate(double deltaT) {
	scene_State* currentState = &scene_states[SimState::currentUpdateState];
	scene_State* newestState = &scene_states[SimState::newestState];

	currentState->camera = newestState->camera;
	currentState->cameraType = newestState->cameraType;
	currentState->cubeMap = newestState->cubeMap;
	currentState->hasSkybox = newestState->hasSkybox;
	currentState->sceneRoot = newestState->sceneRoot;
	currentState->skybox = newestState->skybox;
}

void Scene::switchCameras() {
	scene_State* currentState = &scene_states[SimState::currentUpdateState];
	
	cameras.push_back(currentState->camera);
	currentState->camera = cameras.front();
	cameras.pop_front();

	switch(currentState->cameraType) {
	case FIRSTPERSON:
		currentState->cameraType = THIRDPERSON;
		break;
	case THIRDPERSON:
		currentState->cameraType = FIRSTPERSON;
		break;
	}
}

void Scene::rotateCameraCW() {
	scene_State* currentState = &scene_states[SimState::currentUpdateState];

	if (currentState->cameraType == THIRDPERSON) {
		currentState->camera->rotateCW();
	}
}

void Scene::rotateCameraCCW() {
	scene_State* currentState = &scene_states[SimState::currentUpdateState];

	if (currentState->cameraType == THIRDPERSON) {
		currentState->camera->rotateCCW();
	}
}

void Scene::cameraForward() {
	scene_State* currentState = &scene_states[SimState::currentUpdateState];

	currentState->camera->forward();
}

void Scene::cameraBack() {
	scene_State* currentState = &scene_states[SimState::currentUpdateState];

	currentState->camera->back();
}

void Scene::cameraRight() {
	scene_State* currentState = &scene_states[SimState::currentUpdateState];

	currentState->camera->right();
}

void Scene::cameraLeft() {
	scene_State* currentState = &scene_states[SimState::currentUpdateState];

	currentState->camera->left();
}

void Scene::zoomCamera(double dy) {
	scene_State* currentState = &scene_states[SimState::currentUpdateState];

	currentState->camera->zoom(dy);
}

void Scene::cameraMouseView(int x, int y) {
	scene_State* currentState = &scene_states[SimState::currentUpdateState];

	currentState->camera->mouseView(x, y);
}

Camera* Scene::getCurrentCamera() {
	scene_State* currentState = &scene_states[SimState::currentUpdateState];

	return currentState->camera;
}