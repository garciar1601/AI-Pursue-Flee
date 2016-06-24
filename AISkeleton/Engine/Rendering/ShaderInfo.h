#ifndef SHADER_INFO_H
#define SHADER_INFO_H
#include <GL\glew.h>
struct ShaderInfo{
	GLuint vertexShaderID;
	GLuint fragmentShaderID;
	GLuint programID;
	ShaderInfo() : vertexShaderID(0), fragmentShaderID(0), programID(0) {}
};

#endif