#pragma once
#include "Shader.h"
#include "GL/freeglut.h"

class Material
{
public:
	Material();
	void apply(Shader *shader);
	void setDiffuse(GLfloat r, GLfloat g, GLfloat b);
	void setAmbient(GLfloat r, GLfloat g, GLfloat b);
	void setSpecular(GLfloat r, GLfloat g, GLfloat b);
	void setShininess(GLfloat p);

private:
	GLfloat _ambient[4];
	GLfloat _diffuse[4];
	GLfloat _specular[4];
	GLfloat _shininess;
};