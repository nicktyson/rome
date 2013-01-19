#ifndef VITALENTITY_H
#define VITALENTITY_H

#include <string>
#include "MeshNode.h"

class VitalEntity : public MeshNode {
public:
	VitalEntity();
	VitalEntity(std::string fileLocation);
	virtual void update();

private:

};

#endif