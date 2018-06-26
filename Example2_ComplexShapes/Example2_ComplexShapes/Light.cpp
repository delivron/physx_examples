#include "Light.h"

Camera *Light::_camera = nullptr;

// Конструктор класса.
Light::Light()
{
	for (int i = 0; i < 4; i++)
		_position[i] = _ambient[i] = _diffuse[i] = _specular[i] = (i == 3) ? 1.0f : 0.0f;
}

// Применить параметры источника света.
void Light::apply(Shader *shader)
{
	vec4 coord(0.0f, 0.0f, 0.0f, 1.0f);

	if (_camera != nullptr)
	{
		mat4 modelMatrix = make_mat4x4(_camera->getViewMatrix());
		coord = modelMatrix * make_vec4(_position);
	}
	
	shader->setUniform4fv("lAmbient", _ambient);
	shader->setUniform4fv("lDiffuse", _diffuse);
	shader->setUniform4fv("lSpecular", _specular);
	shader->setUniform4fv("lPosition", value_ptr(coord));
}

// Задать позицию источника света.
void Light::setPosition(GLfloat x, GLfloat y, GLfloat z)
{
	_position[0] = x;
	_position[1] = y;
	_position[2] = z;
}

// Задать фоновую составляющую источника.
void Light::setAmbient(GLfloat r, GLfloat g, GLfloat b)
{
	_ambient[0] = r;
	_ambient[1] = g;
	_ambient[2] = b;
}

// Задать диффузную составляющую источника.
void Light::setDiffuse(GLfloat r, GLfloat g, GLfloat b)
{
	_diffuse[0] = r;
	_diffuse[1] = g;
	_diffuse[2] = b;
}

// Задать зеркальную составляющую источника.
void Light::setSpecular(GLfloat r, GLfloat g, GLfloat b)
{
	_specular[0] = r;
	_specular[1] = g;
	_specular[2] = b;
}

// Задать указатель на камеру.
void Light::setCamera(Camera *camera)
{
	_camera = camera;
}