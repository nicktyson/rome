#ifndef CAMERA_H
#define CAMERA_H

#include "scene_node.h"

class Camera : public scene_node {
public:
	Camera();
	virtual void forward() = 0;
	virtual void back() = 0;
	virtual void left() = 0;
	virtual void right() = 0;
};

#endif