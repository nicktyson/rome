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
	void createBuffer(GLuint* buffer, bool color);
	void initPostShaders();
	void firstPass(Scene* scene);
	void deferredPass(Scene* scene);
	void transparencyPass(Scene* scene);
	void depthPeel(Scene* scene, GLuint opaqueDepthBuffer, GLuint previousPeelDepthBuffer, GLuint currentPeelDepthBuffer, GLuint colorBuffer);
	void postProcess();

	GLuint fbo;
	GLuint depthBuffer;
	GLuint diffuseBuffer;
	GLuint specularBuffer;
	GLuint positionBuffer;
	GLuint normalBuffer;
	GLuint finalBuffer;

	GLuint transparencyDepthBuffer1;
	GLuint transparencyDepthBuffer2;
	GLuint transparencyColorBuffer1;
	GLuint transparencyColorBuffer2;
	GLuint transparencyColorBuffer3;
	GLuint transparencyColorBuffer4;

	ScreenQuad fullscreenQuad;
	ShaderProgram* uberShader;
	ShaderProgram* postprocessShader;
	ShaderProgram* compositeTransparencyShader;
	std::vector<LightNode*> lights;

	static const int MAX_LIGHTS;
};

#endif