#include <GL/glew.h>
#include <iostream>
#include <string>
#include "MeshNode.h"

MeshNode::MeshNode() {
	objectMesh = new mesh();
}

MeshNode::MeshNode(std::string fileLocation) {
	objectMesh = new mesh(fileLocation);
}

mesh * MeshNode::getMesh() {
	return objectMesh;
}

void MeshNode::setMesh(mesh * newMesh) {
	objectMesh = newMesh;
}

void MeshNode::draw() {
	//std::cout << "hi" << std::endl;

	applyTransformation();

	//shortcut variables to prevent otherwise-atrocious verbosity
	//returns reference
	std::vector<float> meshVerts = objectMesh->getVertices();
	std::vector<int> cheatTris = objectMesh->getTriangles();
	std::vector<float> cheatNorms = objectMesh->getNormals();

	//draw triangles
	glBegin(GL_TRIANGLES);

	for(int i = 0; i < cheatTris.size(); i++) {
		glNormal3f(cheatNorms[3*cheatTris[i]], cheatNorms[3*cheatTris[i]+1], cheatNorms[3*cheatTris[i]+2]);
		glVertex3f(meshVerts[3*cheatTris[i]], meshVerts[3*cheatTris[i]+1], meshVerts[3*cheatTris[i]+2]);
	}

	glEnd();

	//draw children
	for(std::vector<scene_node*>::iterator it = children.begin(); it != children.end(); ++it) {
		(*it)->draw();
	}

	//undo changes to matrix
	glPopMatrix();
}

void MeshNode::update(double deltaT) {
	for(std::vector<scene_node*>::iterator it = children.begin(); it != children.end(); ++it) {
		(*it)->update(deltaT);
	}
}