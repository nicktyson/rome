#include <iostream>
#include <algorithm>
#include "Renderer.h"
#include "scene_node.h"
#include "MatrixStack.h"
#include "ScreenQuad.h"
#include "ShaderProgram.h"
#include "LightNode.h"

const int Renderer::MAX_LIGHTS = 20;

Renderer::Renderer() : lights(MAX_LIGHTS){
	init();
	initPostShaders();
}

void Renderer::init() {
	// set up framebuffer and textures
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	// diffuse buffer: rgb = diffuse color, a = material id
	glGenTextures(1, &diffuseBuffer);
	glBindTexture(GL_TEXTURE_RECTANGLE, diffuseBuffer);
	glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGBA16F, 800, 600, 0, GL_RGBA, GL_FLOAT, NULL);

	// position buffer: rgb = position.xyz
	glGenTextures(1, &positionBuffer);
	glBindTexture(GL_TEXTURE_RECTANGLE, positionBuffer);
	glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGBA16F, 800, 600, 0, GL_RGBA, GL_FLOAT, NULL);

	// normals (currently all 3 dimensions)
	glGenTextures(1, &normalBuffer);
	glBindTexture(GL_TEXTURE_RECTANGLE, normalBuffer);
	glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGBA16F, 800, 600, 0, GL_RGBA, GL_FLOAT, NULL);

	// depth
	glGenTextures(1, &depthBuffer);
	glBindTexture(GL_TEXTURE_RECTANGLE, depthBuffer);
	glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_DEPTH_COMPONENT32,  800, 600, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
	
	// final - result of 2nd pass
	glGenTextures(1, &finalBuffer);
	glBindTexture(GL_TEXTURE_RECTANGLE, finalBuffer);
	glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGBA16F,  800, 600, 0, GL_RGBA, GL_FLOAT, NULL);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::initPostShaders() {
	uberShader = new ShaderProgram("ubershader");
	postprocessShader = new ShaderProgram("bloom");
}

void Renderer::render(scene_node* root) {
	lights.clear();

	firstPass(root);
	deferredPass();
	postProcess();
}

void Renderer::firstPass(scene_node* root) {
	extern MatrixStack* sceneGraphMatrixStack;

	//bind the fbo
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	//attach textures to fbo for writing
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_RECTANGLE, diffuseBuffer, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_RECTANGLE, positionBuffer, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_RECTANGLE, normalBuffer, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_RECTANGLE, depthBuffer, 0);

	//tell fbo to draw to the diffuse, position, and normal textures
	GLenum buffer[3];
	buffer[0] = GL_COLOR_ATTACHMENT0;
	buffer[1] = GL_COLOR_ATTACHMENT1;
	buffer[2] = GL_COLOR_ATTACHMENT2;
	glDrawBuffers(3, &buffer[0]);

	//reset stuff
	glViewport(0, 0, 800, 600);
	glClearColor(0.4f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	sceneGraphMatrixStack->loadIdentity();
	
	//draw the scene
	//nodes bind their own shaders, uniforms, and vao
	root->draw(this);

	//detach textures from fbo
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_RECTANGLE, 0, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_RECTANGLE, 0, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_RECTANGLE, 0, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_RECTANGLE, 0, 0);

	//unbind fbo
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::deferredPass() {
	//bind fbo
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	//attach textures to fbo for writing
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_RECTANGLE, finalBuffer, 0);

	//make fbo draw to the final texture
	GLenum buffer[1];
	buffer[0] = GL_COLOR_ATTACHMENT0;
	glDrawBuffers(1, &buffer[0]);

	//reset stuff
	glViewport(0, 0, 800, 600);
	glClearColor(0.0f, 0.0f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	//bind ubershader
	uberShader->use();

	//set uniforms (mostly just lights)
	int numLights = std::min((int)lights.size(), MAX_LIGHTS);
	glUniform1i(0, numLights);
	for (int i = 0; i < numLights; i++) {
		glUniform3f(1+i, lights[i]->eyespacePosition.x, lights[i]->eyespacePosition.y, lights[i]->eyespacePosition.z);
		glUniform3f(21+i, lights[i]->color[0], lights[i]->color[1], lights[i]->color[2]);
		glUniform1f(41+i, lights[i]->intensity);
	}

	//give access to the diffuse, position, and normal textures as uniform samplers
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_RECTANGLE, diffuseBuffer);
	glUniform1i(uberShader->getUniformLocation("diffuseBuffer"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_RECTANGLE, positionBuffer);
	glUniform1i(uberShader->getUniformLocation("positionBuffer"), 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_RECTANGLE, normalBuffer);
	glUniform1i(uberShader->getUniformLocation("normalBuffer"), 2);

	glActiveTexture(GL_TEXTURE0);

	//draw quad to the final texture
	fullscreenQuad.draw();

	//unbind shader
	uberShader->unuse();

	//detach final texture from fbo
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_RECTANGLE, 0, 0);

	//unbind fbo
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::postProcess() {
	//reset stuff
	glViewport(0, 0, 800, 600);
	glClearColor(0.4f, 0.4f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	//bind post-process shader
	postprocessShader->use();

	//set uniforms
	//access final texture as uniform sampler
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_RECTANGLE, finalBuffer);
	glUniform1i(0, 0);

	//draw quad with post-processed final texture
	fullscreenQuad.draw();

	//unbind shader
	postprocessShader->unuse();
}

void Renderer::addLight(LightNode* light) {
	lights.push_back(light);
}