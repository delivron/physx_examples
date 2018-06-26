#pragma once
#include <iostream>
#include <fstream>
#include "GL/glew.h"
#include "GL/freeglut.h"

using namespace std;

class Shader
{
public:
	Shader();
	~Shader();
	static void deactivate();
	void activate();
	bool load(const char *vertexFileName, const char *fragmentFileName);
	void setUniformMat4(const char *name, const float *matrix);
	void setUniform4fv(const char *name, const float *vector);
	void setUniform1f(const char *name, float value);
	void setUniform1i(const char* name, int value);

private:
	GLuint _programIndex;
	GLuint _vertexShaderIndex;
	GLuint _fragmentShaderIndex;

	bool compileShader(const char *fileName, GLenum type, GLuint &index);
	bool linkProgram();
};