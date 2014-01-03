#include "TestScene.h"
#include <string>
#include <iostream>
#include "MeshNode.h"
#include "VitalEntity.h"
#include "LightNode.h"
#include "Skybox.h"
#include "SimState.h"

TestScene::TestScene() {
	
};

void TestScene::init() {
	initCameras();
	initScene();
}

void TestScene::initScene() {
	scene_State* currentState = &scene_states[SimState::currentUpdateState];

	extern std::string ROME_PATH;

	std::string location = ROME_PATH + "/Assets/Meshes/test_sphere_hires.msh";
	std::string cubeLocation = ROME_PATH + "/Assets/Meshes/test_cube.msh";
	std::string quadLocation = ROME_PATH + "/Assets/Meshes/test_quad.msh";

	std::string testTextureLocation = ROME_PATH + "/Assets/Textures/stone.png";
	std::string testNormalLocation = ROME_PATH + "/Assets/Textures/stone_normal.png";
	std::string transTextureLocation = ROME_PATH + "/Assets/Textures/stone_transparent.png";

	std::string skyboxLocation = ROME_PATH + "/Assets/Meshes/test_cube_skybox.msh";
	std::string skyboxTexLocation = ROME_PATH + "/Assets/Textures/skybox.png";
	std::string cubeMapLocation = ROME_PATH + "/Assets/Textures/debug";

	//make two offset objects
	MeshNode* childNode = new MeshNode(location, MaterialList::BLINNPHONG);
	childNode->setMaterialColor(1.0, 1.0, 1.0);
	childNode->setMaterialAlpha(0.4);
	childNode->setMaterialProperties(40.0, 0.2, 4.0);
	//childNode->setMaterialTexture(transTextureLocation);
	//childNode->setMaterialNormalMap(testNormalLocation);
	childNode->setTranslation(0, 0, 0);
	currentState->sceneRoot->addChild(childNode);

	////float randRot = rand() % 90;
	VitalEntity * secondChild = new VitalEntity(location, 1, 0, 0);
	secondChild->setTranslation(2, 2, 0);
	//secondChild->setRotation(20, randRot, 0);
	childNode->addChild(secondChild);

	//add a floor
	MeshNode* floor = new MeshNode(quadLocation, MaterialList::BLINNPHONG);
	floor->setMaterialColor(1.0, 1.0, 1.0, 1.0, 0.0, 0.0);
	floor->setMaterialProperties(40.0, 1.0, 1.0);
	floor->setMaterialTexture(testTextureLocation);
	floor->setMaterialNormalMap(testNormalLocation);
	floor->setScaling(5, 5, 1.0);
	floor->setTranslation(0, 0, -1.1);
	currentState->sceneRoot->addChild(floor);

	//make a ring of spheres
	for(double i = 0; i < 6; ++i) {
		MeshNode* newNode = new MeshNode(location, MaterialList::SOLIDTRANSPARENT);
		newNode->setMaterialColor(0.0, 1.0, 1.0);
		newNode->setMaterialAlpha(0.4);
		//newNode->setMaterialProperties(40.0, 1.0, 1.0);
		newNode->setMaterialTexture(transTextureLocation);
		//newNode->setMaterialNormalMap(testNormalLocation);
		newNode->setTranslation(3*sin(i*6.28/6), 3*cos(i*6.28/6), 0);
		currentState->sceneRoot->addChild(newNode);
	}

	Skybox* sky = new Skybox(this, skyboxLocation);
	sky->setMaterialColor(1.0, 1.0, 1.0, 0.0, 0.0, 0.0);
	sky->setMaterialTexture(skyboxTexLocation);
	sky->setScaling(100.0, 100.0, 100.0);
	setSkybox(sky);

	//add a light
	LightNode* light = new LightNode();
	light->setTranslation(16.0, 16.0, 6.0);
	currentState->sceneRoot->addChild(light);

	//cube mapping
	setCubeMap(cubeMapLocation);
};