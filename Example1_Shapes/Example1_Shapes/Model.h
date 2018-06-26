#pragma once
#include "glm/glm.hpp"
#include "GL/glew.h"
#include "GL/freeglut.h"

using namespace glm;

class Model
{
public:
	friend class ModelLoader;
	
	Model();
	~Model();
	void createPlane(float size);
	void draw() const;

private:
	struct Vertex
	{
		vec3 coord;
		vec3 normal;

		Vertex(vec3 c, vec3 n) :
			coord(c), normal(n) {};

		Vertex(vec3 c) :
			coord(c), normal(vec3(0.0f, 1.0f, 0.0f)) {};
	};

	GLuint _arrayIndex;
	GLuint _vertexArrayIndex;
	GLuint _verticesCount;
};