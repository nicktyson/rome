#ifndef FPCAMERA_H
#define FPCAMERA_H

#include "Camera.h"
#include <vector>
#include "../Lib/glm/glm.hpp"

class Renderer;

class FPCamera : public Camera {
public:
	FPCamera();
	void draw(Renderer* r, bool isTransparentPass);
	void update(double deltaT);
	void forward();
	void back();
	void left();
	void right();
	void mouseView(int x, int y);
	void rotateCW();
	void rotateCCW();
	glm::mat4 getInverseNormalMatrix();

private:
	std::vector<float> velocity;
	std::vector<float> angVelocity;
	std::vector<int> lastMouse;
	std::vector<int> currentMouse;

	static const float MAX_VELOCITY;
	static const float MOUSE_SENSITIVITY;
	static const float VELOCITY_DRAG;
	static const float VIEW_DRAG;
};

#endif