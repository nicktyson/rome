#ifndef SCENE_NODE_H
#define SCENE_NODE_H

#include <vector>

class Renderer;

class scene_node {
public:
	scene_node();
	scene_node * getParent();
	std::vector<scene_node *> getChildren();
	void clearChildren();
	void setParent(scene_node * newParent);
	void addChild(scene_node * newChild);
	std::vector<float> getTranslation();
	std::vector<float> getRotation();
	std::vector<float> getScaling();
	void setTranslation(float tx, float ty, float tz);
	void setRotation(float rx, float ry, float rz);
	void setScaling(float sx, float sy, float sz);
	virtual void draw(Renderer* r, bool isTransparentPass);
	void applyTransformation();
	virtual void update(double deltaT);
	virtual void stateUpdate(double deltaT);

protected:
	struct sn_State {
		scene_node* parent;
		std::vector<scene_node*> children;
		std::vector<float> translation;
		std::vector<float> rotation;
		std::vector<float> scaling;

		sn_State() : translation(3), rotation(3), scaling(3)
		{	}
	};

	std::vector<sn_State> sn_states;
};

#endif