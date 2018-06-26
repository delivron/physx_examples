#include "Model.h"

// Конструктор модели.
Model::Model()
{
	_arrayIndex = 0;
	_vertexArrayIndex = 0;
	_verticesCount = 0;
}

// Деструктор модели.
Model::~Model()
{
	glDeleteBuffers(1, &_arrayIndex);
	glDeleteVertexArrays(1, &_vertexArrayIndex);
}

// Создать плоскость.
void Model::createPlane(float size)
{
	float halfSize = 0.5f * size;
	Vertex vertices[6] =
	{
		Vertex(vec3(-halfSize, 0.0f, -halfSize)),
		Vertex(vec3(-halfSize, 0.0f, halfSize)),
		Vertex(vec3(halfSize, 0.0f, -halfSize)),
		Vertex(vec3(halfSize, 0.0f, -halfSize)),
		Vertex(vec3(-halfSize, 0.0f, halfSize)),
		Vertex(vec3(halfSize, 0.0f, halfSize))
	};

	_verticesCount = 6;

	if (_arrayIndex != 0)
		glDeleteBuffers(1, &_arrayIndex);
	if (_vertexArrayIndex != 0)
		glDeleteVertexArrays(1, &_vertexArrayIndex);

	glGenBuffers(1, &_arrayIndex);
	glBindBuffer(GL_ARRAY_BUFFER, _arrayIndex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

	glGenVertexArrays(1, &_vertexArrayIndex);
	glBindVertexArray(_vertexArrayIndex);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, coord));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
}

// Нарисовать модель.
void Model::draw() const
{
	glBindVertexArray(_vertexArrayIndex);
	glBindBuffer(GL_ARRAY_BUFFER, _arrayIndex);
	glDrawArrays(GL_TRIANGLES, 0, _verticesCount);
	glBindVertexArray(0);
}