#include <iostream>
#include <algorithm>
#include "Renderer.h"
#include "Scene.h"
#include "Materials/Material.h"
#include "Materials/MaterialList.h"
#include "MatrixStack.h"
#include "ScreenQuad.h"
#include "ShaderProgram.h"
#include "LightNode.h"
#include "../Lib/glm/glm.hpp"
#include "../Lib/glm/gtc/type_ptr.hpp"

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

	//specular buffer: rgb = specular color
	glGenTextures(1, &specularBuffer);
	glBindTexture(GL_TEXTURE_RECTANGLE, specularBuffer);
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


	//buffers for transparency via depth peeling
	createBuffer(&transparencyDepthBuffer1, false);
	createBuffer(&transparencyDepthBuffer2, false);
	createBuffer(&transparencyColorBuffer1, true);
	createBuffer(&transparencyColorBuffer2, true);
	createBuffer(&transparencyColorBuffer3, true);
	createBuffer(&transparencyColorBuffer4, true);


	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::createBuffer(GLuint* buffer, bool color) {
	glGenTextures(1, buffer);
	glBindTexture(GL_TEXTURE_RECTANGLE, *buffer);

	if (color) {
		glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGBA16F,  800, 600, 0, GL_RGBA, GL_FLOAT, NULL);
	} else {
		glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_DEPTH_COMPONENT32,  800, 600, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
	}
}

void Renderer::initPostShaders() {
	uberShader = new ShaderProgram("ubershader");
	compositeTransparencyShader = new ShaderProgram("composite_transparency");
	//postprocessShader = new ShaderProgram("bloom");
	postprocessShader = new ShaderProgram("plain_post");
}

void Renderer::render(Scene* scene) {
	lights.clear();

	firstPass(scene);
	deferredPass(scene);
	transparencyPass(scene);

	postProcess();
}

void Renderer::firstPass(Scene* scene) {
	extern MatrixStack* sceneGraphMatrixStack;

	//bind the fbo
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	//attach textures to fbo for writing
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_RECTANGLE, diffuseBuffer, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_RECTANGLE, specularBuffer, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_RECTANGLE, positionBuffer, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_RECTANGLE, normalBuffer, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_RECTANGLE, depthBuffer, 0);

	//tell fbo to draw to the diffuse, position, and normal textures
	GLenum buffer[4];
	buffer[0] = GL_COLOR_ATTACHMENT0;
	buffer[1] = GL_COLOR_ATTACHMENT1;
	buffer[2] = GL_COLOR_ATTACHMENT2;
	buffer[3] = GL_COLOR_ATTACHMENT3;
	glDrawBuffers(4, &buffer[0]);

	//reset stuff
	glViewport(0, 0, 800, 600);
	glClearColor(0.4f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	sceneGraphMatrixStack->loadIdentity();
	
	//draw the scene
	//nodes bind their own shaders, uniforms, and vao
	scene->draw(this, false);

	//detach textures from fbo
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_RECTANGLE, 0, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_RECTANGLE, 0, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_RECTANGLE, 0, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_RECTANGLE, 0, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_RECTANGLE, 0, 0);

	//unbind fbo
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::deferredPass(Scene* scene) {
	//bind fbo
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	//attach texture to fbo for writing
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_RECTANGLE, finalBuffer, 0);

	//make fbo draw to the final texture
	GLenum buffer[1];
	buffer[0] = GL_COLOR_ATTACHMENT0;
	glDrawBuffers(1, &buffer[0]);

	//reset stuff
	glViewport(0, 0, 800, 600);
	glClearColor(0.0f, 0.4f, 0.0f, 1.0f);
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

	glm::mat4 eyeToWorldNormalMatrix = scene->getEyeToWorldNormalMatrix();
	glUniformMatrix4fv(uberShader->getUniformLocation("eyeToWorldNormalMatrix"), 1, GL_FALSE, glm::value_ptr(eyeToWorldNormalMatrix));

	//set environment map
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_CUBE_MAP, scene->getCubeMap());
	glUniform1i(uberShader->getUniformLocation("environmentCubeMap"), 4);

	//give access to the diffuse, specular, position, and normal textures as uniform samplers
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_RECTANGLE, diffuseBuffer);
	glUniform1i(uberShader->getUniformLocation("diffuseBuffer"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_RECTANGLE, specularBuffer);
	glUniform1i(uberShader->getUniformLocation("specularBuffer"), 1);
	
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_RECTANGLE, positionBuffer);
	glUniform1i(uberShader->getUniformLocation("positionBuffer"), 2);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_RECTANGLE, normalBuffer);
	glUniform1i(uberShader->getUniformLocation("normalBuffer"), 3);

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

//this pass handles transparent/translucent objects
void Renderer::transparencyPass(Scene* scene) {
	extern MatrixStack* sceneGraphMatrixStack;

	//bind the fbo
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	//tell fbo to draw to the first color attachment
	GLenum buffer[1];
	buffer[0] = GL_COLOR_ATTACHMENT0;
	glDrawBuffers(1, &buffer[0]);

	//set first depth buffer
	//replace with an in-shader solution!
	std::vector<GLubyte> nearData(800*600, 0.0);
	glBindTexture(GL_TEXTURE_RECTANGLE, transparencyDepthBuffer1);
	glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_DEPTH_COMPONENT32,  800, 600, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, &nearData[0]);

	//four layers
	depthPeel(scene, depthBuffer, transparencyDepthBuffer1, transparencyDepthBuffer2, transparencyColorBuffer1);
	depthPeel(scene, depthBuffer, transparencyDepthBuffer2, transparencyDepthBuffer1, transparencyColorBuffer2);
	depthPeel(scene, depthBuffer, transparencyDepthBuffer1, transparencyDepthBuffer2, transparencyColorBuffer3);
	depthPeel(scene, depthBuffer, transparencyDepthBuffer2, transparencyDepthBuffer1, transparencyColorBuffer4);

	//composite layers
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_RECTANGLE, diffuseBuffer, 0);
	
	glViewport(0, 0, 800, 600);
	glClearColor(0.0f, 0.0f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	compositeTransparencyShader->use();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_RECTANGLE, finalBuffer);
	glUniform1i(compositeTransparencyShader->getUniformLocation("opaqueBuffer"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_RECTANGLE, transparencyColorBuffer1);
	glUniform1i(compositeTransparencyShader->getUniformLocation("colorBuffer1"), 1);
	
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_RECTANGLE, transparencyColorBuffer2);
	glUniform1i(compositeTransparencyShader->getUniformLocation("colorBuffer2"), 2);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_RECTANGLE, transparencyColorBuffer3);
	glUniform1i(compositeTransparencyShader->getUniformLocation("colorBuffer3"), 3);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_RECTANGLE, transparencyColorBuffer4);
	glUniform1i(compositeTransparencyShader->getUniformLocation("colorBuffer4"), 3);

	glActiveTexture(GL_TEXTURE0);

	//draw quad to the final texture
	fullscreenQuad.draw();

	//unbind shader
	compositeTransparencyShader->unuse();

	//detach final texture from fbo
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_RECTANGLE, 0, 0);

	//unbind fbo
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::depthPeel(Scene* scene, GLuint opaqueDepthBuffer, GLuint previousPeelDepthBuffer, GLuint currentPeelDepthBuffer, GLuint colorBuffer) {
	extern MaterialList* materialList;
	extern MatrixStack* sceneGraphMatrixStack;

	//attach textures to fbo for writing
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_RECTANGLE, colorBuffer, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_RECTANGLE, currentPeelDepthBuffer, 0);	

	//reset stuff
	glViewport(0, 0, 800, 600);
	glClearColor(0.1f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	//glDisable(GL_CULL_FACE);

	sceneGraphMatrixStack->loadIdentity();
	
	materialList->getMaterial(materialList->SOLIDTRANSPARENT)->setTransparencyUniforms(opaqueDepthBuffer, previousPeelDepthBuffer);

	//draw the scene
	//nodes bind their own shaders, uniforms, and vao
	scene->draw(this, true);

	//detach textures from fbo
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_RECTANGLE, 0, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_RECTANGLE, 0, 0);

	//glEnable(GL_CULL_FACE);
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
	glBindTexture(GL_TEXTURE_RECTANGLE, diffuseBuffer);
	glUniform1i(0, 0);

	//draw quad with post-processed final texture
	fullscreenQuad.draw();

	//unbind shader
	postprocessShader->unuse();
}

void Renderer::addLight(LightNode* light) {
	lights.push_back(light);
}