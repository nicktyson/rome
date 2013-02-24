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

	GLint status;
    glGetShaderiv(vshader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        std::string msg("Compile failure in vertex shader:\n");
        
        GLint infoLogLength;
        glGetShaderiv(vshader, GL_INFO_LOG_LENGTH, &infoLogLength);
        char* strInfoLog = new char[infoLogLength + 1];
        glGetShaderInfoLog(vshader, infoLogLength, NULL, strInfoLog);
        msg += strInfoLog;
        delete[] strInfoLog;

		std::cout << msg << std::endl;
	}

	glCompileShader(fshader);

	glGetShaderiv(fshader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        std::string msg("Compile failure in fragment shader:\n");
        
        GLint infoLogLength;
        glGetShaderiv(fshader, GL_INFO_LOG_LENGTH, &infoLogLength);
        char* strInfoLog = new char[infoLogLength + 1];
        glGetShaderInfoLog(fshader, infoLogLength, NULL, strInfoLog);
        msg += strInfoLog;
        delete[] strInfoLog;

		std::cout << msg << std::endl;
	}

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