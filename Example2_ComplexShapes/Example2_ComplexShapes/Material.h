#pragma once
#include <map>
#include <string>
#include "Shader.h"
#include "Texture.h"
#include "GL/freeglut.h"

using namespace std;

class Material
{
public:
	typedef map<string, Material*> Materials;

	Material();
	void apply(Shader *shader);
	void setDiffuse(GLfloat r, GLfloat g, GLfloat b);
	void setAmbient(GLfloat r, GLfloat g, GLfloat b);
	void setSpecular(GLfloat r, GLfloat g, GLfloat b);
	void setShininess(GLfloat p);
	void setTexture(Texture *texture);

private:
	GLfloat _ambient[4];
	GLfloat _diffuse[4];
	GLfloat _specular[4];
	GLfloat _shininess;
	Texture *_texture;
};