#ifndef CAMERA_H
#define CAMERA_H

#include "scene_node.h"
#include "../Lib/glm/glm.hpp"

class Camera : public scene_node {
public:
	Camera();
	virtual void forward() = 0;
	virtual void back() = 0;
	virtual void left() = 0;
	virtual void right() = 0;
	virtual void mouseView(int x, int y) = 0;
	virtual void rotateCW() = 0;
	virtual void rotateCCW() = 0;
	virtual void zoom(double dy);
	virtual glm::mat4 getInverseNormalMatrix() = 0;
	void addSceneRoot(scene_node* root);
};

#endif