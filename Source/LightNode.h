#ifndef LIGHTNODE_H
#define LIGHTNODE_H

#include "scene_node.h"
#include "../Lib/glm/glm.hpp"
#include <vector>

class Renderer;

class LightNode : public scene_node {
public:
	LightNode();
	LightNode(float r, float g, float b, float i);
	virtual void draw(Renderer* r, bool isTransparentPass);
	virtual void update(double deltaT);
	virtual void stateUpdate(double deltaT);
	void setColor(float r, float g, float b);
	void setIntensity(float i);

	glm::vec4 eyespacePosition;

	struct ln_State {
		std::vector<float> color;
		float intensity;

		ln_State() : color(3) {
		}
	};

	std::vector<ln_State> ln_states;
};

#endif