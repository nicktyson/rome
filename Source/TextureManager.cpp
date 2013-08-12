#include "TextureManager.h"
#include <gl/glfw.h>
#include <string>
#include <iostream>
#include <map>
#include "../Lib/stb_image.h"

std::map<std::string, GLuint> TextureManager::textures;

TextureManager::TextureManager() {

}

GLuint TextureManager::getTexture(std::string fileName) {
	std::map<std::string, GLuint>::iterator i = textures.find(fileName);
	//generate and return a new texture
	if(i == textures.end()) {
		GLuint location;

		glActiveTexture(GL_TEXTURE0);
		glGenTextures(1, &location);
		glBindTexture(GL_TEXTURE_2D, location);

		//old way - relies on TGA and is removed in newest GLFW release
		//glfwLoadTexture2D(fileName.c_str(), GLFW_BUILD_MIPMAPS_BIT);
		
		//new way
		int x, y, n;
		unsigned char* pixelData = loadImage(fileName, &x, &y, &n, 4); //stbi_load(fileName.c_str(), &x, &y, &n, 4);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*) pixelData);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(pixelData);
		
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

//loads an image and reverses the rows to be bottom-to-top instead of top-to-bottom
unsigned char* TextureManager::loadImage(std::string fileName, int* x, int* y, int* n, int nWanted) {

	unsigned char* pixelData = stbi_load(fileName.c_str(), x, y, n, nWanted);

	int width = *x * nWanted;
	int height = *y;

	//for each row
	for (int i = 0; i < height / 2; i++) {
		//for each byte in the row
		for (int j = 0; j < width; j++) {
			unsigned char temp = pixelData[width*i + j];
			pixelData[width*i + j] = pixelData[width*(height-i-1) + j];
			pixelData[width*(height-i-1) + j] = temp;
		}
	}

	return pixelData;
}