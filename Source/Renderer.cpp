#include "Renderer.h"
#include "scene_node.h"
#include "MatrixStack.h"

Renderer::Renderer() {
	init();
}

void Renderer::init() {
	//set up framebuffer and textures
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glGenTextures(1, &diffuseBuffer);
	glBindTexture(GL_TEXTURE_2D, diffuseBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,  800, 600, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, diffuseBuffer, 0); 

	glGenTextures(1, &depthBuffer);
	glBindTexture(GL_TEXTURE_2D, depthBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32,  800, 600, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthBuffer, 0);

	glGenTextures(1, &finalBuffer);
	glBindTexture(GL_TEXTURE_2D, finalBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,  800, 600, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, finalBuffer, 0);
}

void Renderer::render(scene_node* root) {
	firstPass(root);
	deferredPass();
	postProcess();
}

void Renderer::firstPass(scene_node* root) {
	extern MatrixStack* sceneGraphMatrixStack;

	glClearColor(0.4f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	sceneGraphMatrixStack->loadIdentity();

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	GLenum buffer[1];
	buffer[0] = GL_COLOR_ATTACHMENT0;
	glDrawBuffers(1, &buffer[0]);

	root->draw();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::deferredPass() {
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	GLenum buffer[1];
	buffer[0] = GL_COLOR_ATTACHMENT1;
	glDrawBuffers(1, &buffer[0]);

	//glDisable(GL_DEPTH_TEST);
	//glDisable(GL_BLEND);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, finalBuffer);
	glUniform1i(2, 0);

	// draw quad

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::postProcess() {
	//draw quad
}