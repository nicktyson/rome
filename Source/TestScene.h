#ifndef TESTSCENE_H
#define TESTSCENE_H

#include "Scene.h"

class TestScene : public Scene {
public:
	TestScene();
	virtual void init();

protected:
	virtual void initScene();
};



#endif