#include "VitalEntity.h"
#include "SimState.h"

VitalEntity::VitalEntity() {
	velocity.resize(3);
	angularVelocity.resize(3);
}

VitalEntity::VitalEntity(std::string fileLocation) : MeshNode(fileLocation) {
	velocity.resize(3);
	angularVelocity.resize(3);
}

VitalEntity::VitalEntity(std::string fileLocation, float vx, float vy, float vz) : MeshNode(fileLocation) {
	velocity.resize(3);
	velocity[0] = vx;
	velocity[1] = vy;
	velocity[2] = vz;
	angularVelocity.resize(3);
}

void VitalEntity::update(double deltaT) {
	sn_State* currentSNState = &sn_states[SimState::currentUpdateState];
	sn_State* previousSNState = &sn_states[SimState::newestState];

	scene_node::stateUpdate(deltaT);

	decideBehavior();

	std::vector<float>* currentTranslation = &currentSNState->translation;
	std::vector<float>* previousTranslation = &previousSNState->translation;
	std::vector<float>* currentRotation = &currentSNState->rotation;
	std::vector<float>* previousRotation = &previousSNState->rotation;

	//x1 = x0 + vt
	(*currentTranslation)[0] = (*previousTranslation)[0] + deltaT * velocity[0];
	(*currentTranslation)[1] = (*previousTranslation)[1] + deltaT * velocity[1];
	(*currentTranslation)[2] = (*previousTranslation)[2] + deltaT * velocity[2];

	(*currentRotation)[0] = (*previousRotation)[0] + deltaT * angularVelocity[0];
	(*currentRotation)[1] = (*previousRotation)[1] + deltaT * angularVelocity[1];
	(*currentRotation)[2] = (*previousRotation)[2] + deltaT * angularVelocity[2];

	for(std::vector<scene_node*>::iterator it = currentSNState->children.begin(); it != currentSNState->children.end(); ++it) {
		(*it)->update(deltaT);
	}
}

void VitalEntity::decideBehavior() {

}