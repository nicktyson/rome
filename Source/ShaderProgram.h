#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <gl/glew.h>
#include <string>

class ShaderProgram {
public:
	ShaderProgram();
	ShaderProgram(std::string shaderName);
	void use();
	void unuse();

protected:
	GLuint openglProgram;
	void init(std::string shaderName);
};



#endif