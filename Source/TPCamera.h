#ifndef TPCAMERA_H
#define TPCAMERA_H

#include "Camera.h"
#include <vector>

class TPCamera : public Camera {
public:
	TPCamera();
	void draw();
	void update(double deltaT);
	void forward();
	void back();
	void left();
	void right();

private:
	std::vector<float> velocity;
};

#endif