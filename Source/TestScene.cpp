#include "TestScene.h"
#include <string>
#include "MeshNode.h"
#include "VitalEntity.h"
#include "LightNode.h"

TestScene::TestScene() {
	
};

void TestScene::init() {
	initCameras();
	initScene();
}

void TestScene::initScene() {
	extern std::string ROME_PATH;

	std::string location = ROME_PATH + "/Assets/Meshes/test_sphere_hires.msh";
	std::string cubeLocation = ROME_PATH + "/Assets/Meshes/test_cube.msh";
	std::string quadLocation = ROME_PATH + "/Assets/Meshes/test_quad.msh";

	std::string testTextureLocation = ROME_PATH + "/Assets/Textures/stone.png";
	std::string testNormalLocation = ROME_PATH + "/Assets/Textures/stone_normal.png";
	std::string transTextureLocation = ROME_PATH + "/Assets/Textures/stone_transparent.png";

	std::string cubeMapLocation = ROME_PATH + "/Assets/Textures/grace";

	//make two offset objects
	MeshNode* childNode = new MeshNode(cubeLocation, MaterialList::SOLIDTRANSPARENT);
	childNode->setMaterialColor(1.0, 1.0, 1.0);
	childNode->setMaterialAlpha(0.4);
	childNode->setMaterialProperties(40.0, 0.2, 4.0);
	childNode->setMaterialTexture(transTextureLocation);
	//childNode->setMaterialNormalMap(testNormalLocation);
	childNode->setTranslation(0, 0, 0);
	sceneRoot->addChild(childNode);

	////float randRot = rand() % 90;
	//VitalEntity * secondChild = new VitalEntity(location, 1, 0, 0);
	//secondChild->setTranslation(2, 2, 0);
	////secondChild->setRotation(20, randRot, 0);
	//childNode->addChild(secondChild);

	//add a floor
	MeshNode* floor = new MeshNode(quadLocation, MaterialList::BLINNPHONG);
	floor->setMaterialColor(1.0, 1.0, 1.0, 1.0, 0.0, 0.0);
	floor->setMaterialProperties(40.0, 1.0, 1.0);
	floor->setMaterialTexture(testTextureLocation);
	floor->setMaterialNormalMap(testNormalLocation);
	floor->setScaling(5, 5, 1.0);
	floor->setTranslation(0, 0, -1.1);
	sceneRoot->addChild(floor);

	//make a ring of spheres
	for(double i = 0; i < 6; ++i) {
		MeshNode* newNode = new MeshNode(location, MaterialList::SOLIDTRANSPARENT);
		newNode->setMaterialColor(0.0, 1.0, 1.0);
		newNode->setMaterialAlpha(0.4);
	//	newNode->setMaterialProperties(40.0, 1.0, 1.0);
		newNode->setMaterialTexture(testTextureLocation);
	//	newNode->setMaterialNormalMap(testNormalLocation);
		newNode->setTranslation(3*sin(i*6.28/6), 3*cos(i*6.28/6), 0);
		sceneRoot->addChild(newNode);
	}

	//add a light
	LightNode* light = new LightNode();
	light->setTranslation(16.0, 16.0, 6.0);
	sceneRoot->addChild(light);

	//cube mapping
	setCubeMap(cubeMapLocation);
};