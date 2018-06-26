#pragma once
#include "Shader.h"
#include "Camera.h"
#include "GL/freeglut.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

using namespace glm;

class Light
{
public:
	Light();
	void apply(Shader *shader);
	void setPosition(GLfloat x, GLfloat y, GLfloat z);
	void setAmbient(GLfloat r, GLfloat g, GLfloat b);
	void setDiffuse(GLfloat r, GLfloat g, GLfloat b);
	void setSpecular(GLfloat r, GLfloat g, GLfloat b);
	static void setCamera(Camera *camera);

private:
	GLfloat _position[4];
	GLfloat _ambient[4];
	GLfloat _diffuse[4];
	GLfloat _specular[4];
	static Camera *_camera;
};