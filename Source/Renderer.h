#ifndef RENDERER_H
#define RENDERER_H

#include <gl\glew.h>
#include <vector>
#include "ScreenQuad.h"

class scene_node;
class ShaderProgram;
class LightNode;

class Renderer {
public:
	Renderer();
	void render(scene_node* root);
	void addLight(LightNode * light);

protected:
	void init();
	void initPostShaders();
	void firstPass(scene_node* root);
	void deferredPass();
	void postProcess();

	GLuint fbo;
	GLuint depthBuffer;
	GLuint diffuseBuffer;
	GLuint specularBuffer;
	GLuint positionBuffer;
	GLuint normalBuffer;
	GLuint finalBuffer;
	ScreenQuad fullscreenQuad;
	ShaderProgram* uberShader;
	ShaderProgram* postprocessShader;
	std::vector<LightNode*> lights;

	static const int MAX_LIGHTS;
};

#endif