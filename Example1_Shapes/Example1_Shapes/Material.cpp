#include "Material.h"

// Конструктор класса.
Material::Material()
{
	for (int i = 0; i < 4; i++)
		_ambient[i] = _diffuse[i] = _specular[i] = (i == 3) ? 1.0f : 0.0f;

	_shininess = 1.0f;
}

// Применить материал.
void Material::apply(Shader *shader)
{
	shader->setUniform4fv("mAmbient", _ambient);
	shader->setUniform4fv("mDiffuse", _diffuse);
	shader->setUniform4fv("mSpecular", _specular);
	shader->setUniform1f("mShininess", _shininess);
}

// Установить диффузные составляющие материала.
void Material::setDiffuse(GLfloat r, GLfloat g, GLfloat b)
{
	_diffuse[0] = r;
	_diffuse[1] = g;
	_diffuse[2] = b;
	_diffuse[3] = 1.0f;
}

// Установить фоновые составляющие материала.
void Material::setAmbient(GLfloat r, GLfloat g, GLfloat b)
{
	_ambient[0] = r;
	_ambient[1] = g;
	_ambient[2] = b;
	_ambient[3] = 1.0f;
}

// Установить зеркальные составляющие материала.
void Material::setSpecular(GLfloat r, GLfloat g, GLfloat b)
{
	_specular[0] = r;
	_specular[1] = g;
	_specular[2] = b;
	_specular[3] = 1.0f;
}

// Задать степень отполированности.
void Material::setShininess(GLfloat shininess)
{
	_shininess = shininess;
}