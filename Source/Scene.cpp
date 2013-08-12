#include "Scene.h"
#include <string>
#include "Camera.h"
#include "FPCamera.h"
#include "TPCamera.h"
#include "scene_node.h"
#include "Skybox.h"
#include "MatrixStack.h"
#include "TextureManager.h"
#include "../Lib/stb_image.h"
#include "../Lib/glm/glm.hpp"
#include "../Lib/glm/gtc/matrix_inverse.hpp"

Scene::Scene() {
	//cubeMap = 0;
	sceneRoot = new scene_node();
	hasSkybox = false;
}

void Scene::init() {
	initScene();
	initCameras();
}

void Scene::initCameras() {
	cameraType = THIRDPERSON;

	camera = new TPCamera();
	camera->addSceneRoot(sceneRoot);
	cameras.push_back(camera);

	camera = new FPCamera();
	camera->addSceneRoot(sceneRoot);
	cameras.push_back(camera);

	camera = cameras.front();
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

	cubeMap = location;
}

void Scene::setRoot(scene_node* rootNode) {
	sceneRoot = rootNode;
}

void Scene::setSkybox(Skybox* skybx) {
	hasSkybox = true;
	skybox = skybx;
}

GLuint Scene::getCubeMap() {
	return cubeMap;
}

glm::mat4 Scene::getEyeToWorldNormalMatrix() {
	return camera->getInverseNormalMatrix();
}

void Scene::draw(Renderer* r, bool isTransparentPass) {
	camera->draw(r, isTransparentPass);

	if (hasSkybox) {
		skybox->draw(r, isTransparentPass);
	}
}

void Scene::update(double deltaT) {
	camera->update(deltaT);

	if (hasSkybox) {
		skybox->update(deltaT);
	}
}

void Scene::switchCameras() {
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

void Scene::rotateCameraCW() {
	if (cameraType == THIRDPERSON) {
		camera->rotateCW();
	}
}

void Scene::rotateCameraCCW() {
	if (cameraType == THIRDPERSON) {
		camera->rotateCCW();
	}
}

void Scene::cameraForward() {
	camera->forward();
}

void Scene::cameraBack() {
	camera->back();
}

void Scene::cameraRight() {
	camera->right();
}

void Scene::cameraLeft() {
	camera->left();
}

void Scene::zoomCamera(int pos) {
	camera->zoom(pos);
}

void Scene::cameraMouseView(int x, int y) {
	camera->mouseView(x, y);
}

Camera* Scene::getCurrentCamera() {
	return camera;
}