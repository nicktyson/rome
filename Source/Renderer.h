#ifndef RENDERER_H
#define RENDERER_H

#include <gl\glew.h>
#include <vector>
#include "ScreenQuad.h"

class ShaderProgram;
class LightNode;
class Scene;

class Renderer {
public:
	Renderer();
	void render(Scene* scene);
	void addLight(LightNode * light);

protected:
	void init();
	void initPostShaders();
	void firstPass(Scene* scene);
	void deferredPass(Scene* scene);
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