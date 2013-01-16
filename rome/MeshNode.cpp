#include <GL/glew.h>
#include <iostream>
#include "MeshNode.h"

MeshNode::MeshNode() {

}

mesh * MeshNode::getMesh() {
	return objectMesh;
}

void MeshNode::setMesh(mesh * newMesh) {
	objectMesh = newMesh;
}

void MeshNode::draw() {
	//std::cout << "hi";

	applyTransformation();

	//shortcut variables to prevent otherwise-atrocious verbosity
	//returns reference
	std::vector<float> meshVerts = objectMesh->getVertices();
	std::vector<int> cheatTris = objectMesh->getTriangles();
	std::vector<float> cheatNorms = objectMesh->getNormals();

	glBegin(GL_TRIANGLES);

	for(int i = 0; i < objectMesh->getTriangles().size(); i++) {
		glNormal3f(cheatNorms[3*cheatTris[i]], cheatNorms[3*cheatTris[i]+1], cheatNorms[3*cheatTris[i]+2]);
		glVertex3f(meshVerts[3*cheatTris[i]], meshVerts[3*cheatTris[i]+1], meshVerts[3*cheatTris[i]+2]);
	}

	glEnd();

	glPopMatrix();
}