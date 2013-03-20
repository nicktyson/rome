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

	//create and compile the shaders
	GLuint vshader = prepareShader(vshader_loc, GL_VERTEX_SHADER);
	GLuint fshader = prepareShader(fshader_loc, GL_FRAGMENT_SHADER);

	//link the shaders to an OpenGL program
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

GLuint ShaderProgram::prepareShader(std::string location, GLenum type) {
	//load the shader into a char array
	std::ifstream shaderStream(location);
	std::stringstream shaderBuffer;
	shaderBuffer << shaderStream.rdbuf();
	std::string shaderString = shaderBuffer.str();
	const char* shaderCharCode = shaderString.c_str();
	shaderStream.close();

	//create and compile the shader
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &shaderCharCode, NULL);
	glCompileShader(shader);

	//find out if it compiled
	GLint compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	//if not, print the log
	if (compiled == GL_FALSE) {
		GLint logLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
		char* infoLog = new char[logLength + 1];
		glGetShaderInfoLog(shader, logLength, NULL, infoLog);

		//print shader location and log
		if (type == GL_VERTEX_SHADER) {
			std::cout << "Compile failure in vertex shader" << std::endl;
		} else {
			std::cout << "Compile failure in fragment shader" << std::endl;
		}
		std::cout << location << std::endl;
		std::cout << infoLog << std::endl;

		delete[] infoLog;
	}

	return shader;
}