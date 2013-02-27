#ifndef RENDERER_H
#define RENDERER_H

#include <gl\glew.h>
#include "ScreenQuad.h"

class scene_node;
class ShaderProgram;

class Renderer {
public:
	Renderer();
	void render(scene_node* root);

protected:
	void init();
	void initPostShaders();
	void firstPass(scene_node* root);
	void deferredPass();
	void postProcess();

	GLuint fbo;
	GLuint depthBuffer;
	GLuint diffuseBuffer;
	GLuint positionBuffer;
	GLuint normalBuffer;
	GLuint finalBuffer;
	ScreenQuad fullscreenQuad;
	ShaderProgram* uberShader;
	ShaderProgram* postprocessShader;
};

#endif