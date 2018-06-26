#include "Texture.h"

// Конструктор текстуры.
Texture::Texture()
{
	_index = 0;
}

// Деструктор текстуры.
Texture::~Texture()
{
	if (_index != 0)
		glDeleteTextures(1, &_index);
}

// Загрузить текстуру.
void Texture::load(const char *imageName)
{
	ILuint imageIndex;
	ILubyte *texels;
	int width;
	int height;
	int format;
	int type;

	ilGenImages(1, &imageIndex);
	ilBindImage(imageIndex);

	if (!ilLoadImage((ILstring)imageName))
	{
		cout << "[Texture]: error uploading file \"" << imageName << "\"" << endl;
		return;
	}

	glClientActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &_index);
	glBindTexture(GL_TEXTURE_2D, _index);
	glPixelStorei(GL_UNPACK_ALIGNMENT, GL_TRUE);

	width = ilGetInteger(IL_IMAGE_WIDTH);
	height = ilGetInteger(IL_IMAGE_HEIGHT);
	format = ilGetInteger(IL_IMAGE_FORMAT);
	type = ilGetInteger(IL_IMAGE_TYPE);
	texels = ilGetData();

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, type, texels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);
	glGenerateMipmap(GL_TEXTURE_2D);

	ilDeleteImages(1, &imageIndex);
}

// Применить текстуру.
void Texture::apply(Shader *shader, const char *texUniform, int texBlock)
{
	glActiveTexture(texBlock);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _index);
	shader->setUniform1i(texUniform, (int)(texBlock - GL_TEXTURE0));
}

// Отключить текстурирование.
void Texture::disable()
{
	glActiveTexture(GL_TEXTURE0);
	glDisable(GL_TEXTURE_2D);
}