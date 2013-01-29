#include "VitalEntity.h"

VitalEntity::VitalEntity() {
	velocity.resize(3);
}

VitalEntity::VitalEntity(std::string fileLocation) : MeshNode(fileLocation) {
	velocity.resize(3);
}

VitalEntity::VitalEntity(std::string fileLocation, float vx, float vy, float vz) : MeshNode(fileLocation) {
	velocity.resize(3);
	velocity[0] = vx;
	velocity[1] = vy;
	velocity[2] = vz;
}

void VitalEntity::update(double deltaT) {
	translation[0] += deltaT * velocity[0];
	translation[1] += deltaT * velocity[1];
	translation[2] += deltaT * velocity[2];

	for(std::vector<scene_node*>::iterator it = children.begin(); it != children.end(); ++it) {
		(*it)->update(deltaT);
	}
}