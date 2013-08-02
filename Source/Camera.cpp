#include "Camera.h"

Camera::Camera() {

}

void Camera::zoom(int pos) {

}

void Camera::addSceneRoot(scene_node* root) {
	sn_states[0].children.push_back(root);
	sn_states[1].children.push_back(root);
	sn_states[2].children.push_back(root);
}