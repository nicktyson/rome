#ifndef FPCAMERA_H
#define FPCAMERA_H

#include "Camera.h"
#include <vector>

class FPCamera : public Camera {
public:
	FPCamera();
	void draw();
	void update(double deltaT);
	void forward();
	void back();
	void left();
	void right();
	void mouseView(int x, int y);

private:
	std::vector<float> velocity;
	std::vector<float> angVelocity;
	std::vector<int> lastMouse;
	std::vector<int> currentMouse;
};

#endif