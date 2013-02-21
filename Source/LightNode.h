#ifndef LIGHTNODE_H
#define LIGHTNODE_H

#include "scene_node.h"
#include <vector>

class LightNode : public scene_node {
public:
	LightNode();
	virtual void draw();
	void setColor(float r, float g, float b);

private:
	std::vector<float> color;
};

#endif