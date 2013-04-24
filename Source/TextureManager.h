#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <gl/glew.h>
#include <map>
#include <string>

class TextureManager {
public:
	static GLuint getTexture(std::string fileName);

protected:
	static std::map<std::string, GLuint> textures;
	static void init();

private:
	TextureManager();

};

#endif