#ifndef VITALENTITY_H
#define VITALENTITY_H

#include <string>
#include <vector>
#include "MeshNode.h"

class VitalEntity : public MeshNode {
public:
	VitalEntity();
	//constructor w/ mesh location
	VitalEntity(std::string fileLocation);
	//constructor w/ mesh location and velocity
	VitalEntity(std::string fileLocation, float vx, float vy, float vz);
	virtual void update(double deltaT);
	virtual void decideBehavior();

protected:
	std::vector<float> velocity;
	std::vector<float> angularVelocity;
};

#endif