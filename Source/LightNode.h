#ifndef LIGHTNODE_H
#define LIGHTNODE_H

#include "scene_node.h"
#include "../Lib/glm/glm.hpp"
#include <vector>

class Renderer;

class LightNode : public scene_node {
public:
	LightNode();
	virtual void draw(Renderer* r);
	void setColor(float r, float g, float b);
	glm::vec4 eyespacePosition;
	std::vector<float> color;
};

#endif