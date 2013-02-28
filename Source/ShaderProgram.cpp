#include <gl/glew.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <ios>
#include "ShaderProgram.h"

ShaderProgram::ShaderProgram() {
	init("green_test");
}

ShaderProgram::ShaderProgram(std::string shaderName) {
	init(shaderName);
}

void ShaderProgram::init(std::string shaderName) {
	// make paths
	extern std::string ROME_PATH;
	std::string vshader_loc = ROME_PATH + "/Source/Shaders/" + shaderName + ".vp";
	std::string fshader_loc = ROME_PATH + "/Source/Shaders/" + shaderName + ".fp";

	// load vertex shader into char array
	std::ifstream vertstream(vshader_loc);
	std::stringstream vbuffer;
	vbuffer << vertstream.rdbuf();
	std::string vstring = vbuffer.str();
	const char* vcode = vstring.c_str();
	vertstream.close();

	//load fragment shader into char array
	std::ifstream fragstream(fshader_loc);
	std::stringstream fbuffer;
	fbuffer << fragstream.rdbuf();
	std::string fstring = fbuffer.str();
	const char* fcode = fstring.c_str();
	fragstream.close();

	GLuint vshader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fshader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vshader, 1, &vcode, NULL);
	glShaderSource(fshader, 1, &fcode, NULL);

	glCompileShader(vshader);
	glCompileShader(fshader);	

	openglProgram = glCreateProgram();
	glAttachShader(openglProgram, vshader);
	glAttachShader(openglProgram, fshader);

	glLinkProgram(openglProgram);

	glDetachShader(openglProgram, vshader);
	glDetachShader(openglProgram, fshader);	
}

void ShaderProgram::use() {
	glUseProgram(openglProgram);
}

void ShaderProgram::unuse() {
	glUseProgram(0);
}

GLuint ShaderProgram::getUniformLocation(char* name) {
	return glGetUniformLocation(openglProgram, name);
}