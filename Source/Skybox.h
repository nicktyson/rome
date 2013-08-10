#ifndef SKYBOX_H
#define SKYBOX_H

#include "MeshNode.h"
#include <string>

class Scene;
class Renderer;

class Skybox : public MeshNode {
public:
	Skybox(Scene* scn, std::string fileLocation);
	virtual void draw(Renderer* r, bool isTransparentPass);
	virtual void update(double deltaT);

protected:
	Scene* scene;
};


#endif