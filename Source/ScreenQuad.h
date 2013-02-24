#ifndef SCREENQUAD_H
#define SCREENQUAD_H

#include <gl/glew.h>

class ScreenQuad {
public:
	ScreenQuad();
	void init();
	void draw();

protected:
	GLuint vbo;
	GLuint vao;
	GLuint indexBuffer;
};

#endif