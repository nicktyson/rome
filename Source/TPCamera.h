#ifndef TPCAMERA_H
#define TPCAMERA_H

#include "Camera.h"
#include <vector>
#include "../Lib/glm/glm.hpp"

class Renderer;

class TPCamera : public Camera {
public:
	TPCamera();
	void draw(Renderer* r, bool isTransparentPass);
	void update(double deltaT);
	void forward();
	void back();
	void left();
	void right();
	void mouseView(int x, int y);
	void rotateCW();
	void rotateCCW();
	void zoom(double dy);
	glm::mat4 getInverseNormalMatrix();

private:
	std::vector<float> velocity;
	std::vector<float> angVelocity;
	std::vector<int> mousePos;
	int zoomLevel;

	static const float MAX_VELOCITY;
	static const float MAX_ROTATE_VELOCITY;
	static const float DRAG;
	static const float VIEW_DRAG;
	static const float ZOOM_VELOCITY;
	static const float ZOOM_DRAG;
};

#endif