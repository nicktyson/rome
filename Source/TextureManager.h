#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <gl/glew.h>
#include <map>
#include <string>

class TextureManager {
public:
	static GLuint getTexture(std::string fileName);
	static std::map<std::string, GLuint> textures;
	static void init();
	static unsigned char* loadImage(std::string fileName, int* x, int* y, int* n, int nWanted);

private:
	TextureManager();

};

#endif