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

// Нарисовать модель.
void Model::draw(Shader *shader) const
{
	glBindVertexArray(_vertexArrayIndex);
	glBindBuffer(GL_ARRAY_BUFFER, _arrayIndex);

	if (_usedMaterials.size() == 0)
	{
		Texture::disable();
		glDrawArrays(GL_TRIANGLES, 0, _verticesCount);
	}
	else
	{
		string currentMaterial;
		int count;
		
		for (size_t i = 0, len = _usedMaterials.size(); i < len; i++)
		{
			if (i < len - 1)
				count = _usedMaterials[i + 1].beginIndex - _usedMaterials[i].beginIndex;
			else
				count = _verticesCount - _usedMaterials[i].beginIndex;

			currentMaterial = _usedMaterials[i].materialName;
			(*_materials)[currentMaterial]->apply(shader);

			glDrawArrays(GL_TRIANGLES, _usedMaterials[i].beginIndex, count);
		}
	}

	glBindVertexArray(0);
}

// Применить материалы.
void Model::setMaterials(Material::Materials *materials)
{
	_materials = materials;
}

// Указать применяемый материал.
void Model::useMaterial(const string &materialName, int beginIndex)
{
	if (_materials != nullptr)
	{
		if (_materials->find(materialName) == _materials->end())
			cout << "[Model]: attempt to apply unknown material (" << materialName << ")" << endl;
		else
			_usedMaterials.push_back(UsedMaterial(beginIndex, materialName));
	}
}