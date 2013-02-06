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
	void mouseView(int x, int y);

private:
	std::vector<float> velocity;
	std::vector<int> mousePos;

	static const float MAX_VELOCITY;
	static const float DRAG;
};

#endif