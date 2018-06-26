#include "Shader.h"

// Конструктор шейдера.
Shader::Shader()
{
	_programIndex = 0;
	_vertexShaderIndex = 0;
	_fragmentShaderIndex = 0;
}

// Деструктор шейдера.
Shader::~Shader()
{
	glDeleteShader(_vertexShaderIndex);
	glDeleteShader(_fragmentShaderIndex);
	glDeleteProgram(_programIndex);
}

// Отключить шейдер.
void Shader::deactivate()
{
	glUseProgram(0);
}

// Активировать шейдер.
void Shader::activate()
{
	glUseProgram(_programIndex);
}

// Загрузить вершинный и врагментный шейдеры.
bool Shader::load(const char *vertexFileName, const char *fragmentFileName)
{
	bool success[3];

	success[0] = compileShader(vertexFileName, GL_VERTEX_SHADER, _vertexShaderIndex);
	success[1] = compileShader(fragmentFileName, GL_FRAGMENT_SHADER, _fragmentShaderIndex);
	success[2] = linkProgram();

	return success[0] && success[1] && success[2];
}

// Создать и скомпилировать шейдер.
bool Shader::compileShader(const char *fileName, GLenum type, GLuint &index)
{
	ifstream inputFile(fileName, ios_base::in | ios_base::binary);
	char *data = nullptr;
	char *info = nullptr;
	bool succesFlag = true;
	GLint infoLength;
	GLint status;
	int bytesCount;

	if (inputFile.is_open())
	{
		inputFile.seekg(0, ios_base::end);
		bytesCount = (int)inputFile.tellg();
		inputFile.seekg(0, ios_base::beg);

		data = new char[bytesCount + 1]();
		inputFile.read(data, bytesCount);
		inputFile.close();

		index = glCreateShader(type);
		glShaderSource(index, 1, &data, nullptr);
		glCompileShader(index);

		glGetShaderiv(index, GL_COMPILE_STATUS, &status);
		if (status == GL_FALSE)
		{
			glGetShaderiv(index, GL_INFO_LOG_LENGTH, &infoLength);
			info = new char[infoLength];
			glGetShaderInfoLog(index, infoLength, nullptr, info);
			succesFlag = false;

			cout << "[Shader]: error while compiling the shader " << "\"" << fileName << "\"" << endl;
			cout << "[Shader]: " << info << endl;
		}

		delete[] data;
		delete[] info;
	}
	else
	{
		cout << "[Shader]: error opening file \"" << fileName << "\"" << endl;
		succesFlag = false;
	}

	return succesFlag;
}

// Линковка программы.
bool Shader::linkProgram()
{
	bool succesFlag = true;
	char *info = nullptr;
	GLint infoLength;
	GLint status;

	_programIndex = glCreateProgram();
	glAttachShader(_programIndex, _vertexShaderIndex);
	glAttachShader(_programIndex, _fragmentShaderIndex);
	glLinkProgram(_programIndex);

	glGetProgramiv(_programIndex, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		glGetProgramiv(_programIndex, GL_INFO_LOG_LENGTH, &infoLength);
		info = new char[infoLength];
		glGetProgramInfoLog(_programIndex, infoLength, nullptr, info);
		succesFlag = false;

		cout << "[Shader]: error while linking the program" << endl;
		cout << "[Shader]: " << info << endl;
	}

	delete[] info;

	return succesFlag;
}

// Присвоить uniform-переменной матрицу 4x4.
void Shader::setUniformMat4(const char *name, const float *matrix)
{
	GLint uinformIndex = glGetUniformLocation(_programIndex, name);
	glUniformMatrix4fv(uinformIndex, 1, GL_FALSE, matrix);
}

// Присвоить uniform-переменной вектор.
void Shader::setUniform4fv(const char *name, const float *vector)
{
	GLint uinformIndex = glGetUniformLocation(_programIndex, name);
	glUniform4fv(uinformIndex, 1, vector);
}

// Присвоить uniform-переменной float значение.
void Shader::setUniform1f(const char *name, float value)
{
	GLint uinformIndex = glGetUniformLocation(_programIndex, name);
	glUniform1f(uinformIndex, value);
}