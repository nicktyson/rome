#ifndef TPCAMERA_H
#define TPCAMERA_H

#include "Camera.h"

class TPCamera : public Camera {
public:
	TPCamera();
	void draw();
	void update();
	void forward();
	void back();
	void left();
	void right();
};

#endif