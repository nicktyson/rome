#include "TextureManager.h"
#include <gl/glfw.h>
#include <string>
#include <iostream>
#include <map>

std::map<std::string, GLuint> TextureManager::textures;

TextureManager::TextureManager() {

}

GLuint TextureManager::getTexture(std::string fileName) {
	std::map<std::string, GLuint>::iterator i = textures.find(fileName);
	//generate and return a new texture
	if(i == textures.end()) {
		GLuint location;
		glGenTextures(1, &location);
		glBindTexture(GL_TEXTURE_2D, location);
		glfwLoadTexture2D(fileName.c_str(), GLFW_BUILD_MIPMAPS_BIT);
		textures.insert(std::make_pair(fileName, location));
		return location;
	//return an existing texture
	} else {
		return i->second;
	}
}

void TextureManager::init() {
	textures.clear();
}