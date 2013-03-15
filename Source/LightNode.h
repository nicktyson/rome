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
	virtual void draw(Renderer* r);
	void setColor(float r, float g, float b);
	void setIntensity(float i);
	glm::vec4 eyespacePosition;
	std::vector<float> color;
	float intensity;
};

#endif