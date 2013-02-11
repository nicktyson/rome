#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <gl/glew.h>

class ShaderProgram {
public:
	ShaderProgram();
	void use();

protected:
	GLuint openglProgram;
};



#endif