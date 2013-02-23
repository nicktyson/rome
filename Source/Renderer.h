#ifndef RENDERER_H
#define RENDERER_H

#include <gl\glew.h>

class scene_node;

class Renderer {
public:
	Renderer();
	void render(scene_node* root);

protected:
	void init();
	void firstPass(scene_node* root);
	void deferredPass();
	void postProcess();

	GLuint fbo;
	GLuint depthBuffer;
	GLuint diffuseBuffer;
	GLuint finalBuffer;
};

#endif