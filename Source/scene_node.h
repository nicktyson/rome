#ifndef SCENE_NODE_H
#define SCENE_NODE_H

#include <vector>

class scene_node {
public:
	scene_node();
	scene_node * getParent();
	std::vector<scene_node *> getChildren();
	void setParent(scene_node * newParent);
	void addChild(scene_node * newChild);
	bool doesHaveMesh();
	std::vector<float> getTranslation();
	std::vector<float> getRotation();
	std::vector<float> getScaling();
	void setTranslation(float tx, float ty, float tz);
	void setRotation(float rx, float ry, float rz);
	void setScaling(float sx, float sy, float sz);
	virtual void draw();
	void applyTransformation();
	virtual void update();

protected:
	bool hasMesh;
	scene_node * parent;
	std::vector<scene_node *> children;
	std::vector<float> translation;
	std::vector<float> rotation;
	std::vector<float> scaling;
};

#endif