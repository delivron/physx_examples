#pragma once
#include <iostream>
#include <string>
#include "GL/glew.h"
#include "GL/freeglut.h"
#include "IL/il.h"
#include "IL/ilu.h"
#include "IL/ilut.h"
#include "Shader.h"

using namespace std;

class Texture
{
public:
	Texture();
	~Texture();
	void load(const char *fileName);
	void apply(Shader *shader, const char *texUniform, int texBlock = GL_TEXTURE0);
	static void	disable();

private:
	GLuint _index;
};