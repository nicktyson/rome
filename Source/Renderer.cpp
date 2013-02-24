#include "Renderer.h"
#include "scene_node.h"
#include "MatrixStack.h"
#include "ScreenQuad.h"
#include "ShaderProgram.h"

Renderer::Renderer() : fullscreenQuad(){
	init();
	initPostShaders();
}

void Renderer::init() {
	// set up framebuffer and textures
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	// diffuse color
	glGenTextures(1, &diffuseBuffer);
	glBindTexture(GL_TEXTURE_2D, diffuseBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,  800, 600, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, diffuseBuffer, 0); 

	// depth
	glGenTextures(1, &depthBuffer);
	glBindTexture(GL_TEXTURE_2D, depthBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32,  800, 600, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthBuffer, 0);

	// final - result of 2nd pass
	glGenTextures(1, &finalBuffer);
	glBindTexture(GL_TEXTURE_2D, finalBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,  800, 600, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, finalBuffer, 0);
}

void Renderer::initPostShaders() {
	uberShader = new ShaderProgram("ubershader");
	postprocessShader = new ShaderProgram("");
}

void Renderer::render(scene_node* root) {
	firstPass(root);
	deferredPass();
	postProcess();
}

void Renderer::firstPass(scene_node* root) {
	extern MatrixStack* sceneGraphMatrixStack;

	//bind the fbo
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	//reset stuff
	glViewport(0, 0, 800, 600);
	glClearColor(0.4f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	sceneGraphMatrixStack->loadIdentity();

	//tell fbo to draw to the diffuse texture
	GLenum buffer[1];
	buffer[0] = GL_COLOR_ATTACHMENT0;
	glDrawBuffers(1, &buffer[0]);
	
	//draw the scene
	//nodes bind their own shaders, uniforms, and vao
	root->draw();

	//unbind fbo
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::deferredPass() {
	//bind fbo
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	//reset stuff
	glViewport(0, 0, 800, 600);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//make fbo draw to the final texture
	GLenum buffer[1];
	buffer[0] = GL_COLOR_ATTACHMENT1;
	glDrawBuffers(1, &buffer[0]);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	//bind ubershader
	uberShader->use();

	//set uniforms (mostly just lights) - don't need immediately

	//give access to the diffuse texture as uniform sampler
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuseBuffer);
	glUniform1i(0, 0);

	//draw quad to the final texture
	fullscreenQuad.draw();

	//unbind shader
	uberShader->unuse();

	//unbind fbo
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::postProcess() {
	//reset stuff
	glViewport(0, 0, 800, 600);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	//bind post-process shader

	//set uniforms
	//access final texture as uniform sampler
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, finalBuffer);
	glUniform1i(2, 0);

	//draw quad with post-processed final texture
	fullscreenQuad.draw();

	//unbind shader
}