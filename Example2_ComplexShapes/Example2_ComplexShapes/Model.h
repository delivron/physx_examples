#pragma once
#include <map>
#include <vector>
#include <string>
#include "Material.h"
#include "Shader.h"
#include "glm/glm.hpp"
#include "GL/glew.h"
#include "GL/freeglut.h"

using namespace glm;
using namespace std;

class Model
{
public:
	friend class ModelLoader;
	
	~Model();
	void draw(Shader *shader) const;

private:
	Model();
	void setMaterials(Material::Materials *materials);
	void useMaterial(const string& materialName, int beginIndex);

	struct Vertex
	{
		vec3 coord;
		vec2 texCoord;
		vec3 normal;

		Vertex(vec3 c, vec2 tc, vec3 n) :
			coord(c), texCoord(tc), normal(n) {};

		Vertex(vec3 c, vec2 tc) :
			coord(c), texCoord(tc), normal(vec3(0.0f, 1.0f, 0.0f)) {};
	};
	struct UsedMaterial
	{
		int beginIndex;
		string materialName;

		UsedMaterial(int partBeginIndex, string partMaterialName) :
			beginIndex(partBeginIndex), materialName(partMaterialName) {};
	};

	Material::Materials* _materials;
	vector<UsedMaterial> _usedMaterials;
	GLuint _arrayIndex;
	GLuint _vertexArrayIndex;
	GLuint _verticesCount;
};