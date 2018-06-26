#include "ModelLoader.h"

// Конструктор загрузчика.
ModelLoader::ModelLoader()
{
	_keywordCodes["v"] = CODE_V;
	_keywordCodes["vn"] = CODE_VN;
	_keywordCodes["f"] = CODE_F;
}

// Получить ссылку на экземпляр класса.
ModelLoader& ModelLoader::getInstance()
{
	static ModelLoader instance;
	return instance;
}

// Загрузить файл 3d модели с файла формата "obj".
Model* ModelLoader::load(const char *modelName)
{
	ifstream objFile(modelName, ios::in);
	Model *model = nullptr;
	vector<vec3> coords;
	vector<vec3> normals;
	vector<Model::Vertex> vertices;
	string dataType;
	Face face;
	vec3 coord, normal;
	float x, y, z;

	if (isCorrectFile(modelName) && objFile.is_open())
	{
		model = new Model();

		while (!objFile.eof())
		{
			objFile >> dataType;

			if (_keywordCodes.find(dataType) == _keywordCodes.end())
			{
				skipLine(objFile);
				continue;
			}

			switch (_keywordCodes[dataType])
			{
			case CODE_V:
				objFile >> x >> y >> z;
				coords.push_back(vec3(x, y, z));
				break;

			case CODE_VN:
				objFile >> x >> y >> z;
				normals.push_back(vec3(x, y, z));
				break;

			case CODE_F:
				for (int i = 0; i < 3; i++)
				{
					objFile >> face;
					coord = coords[face.coordIndex - 1];
					normal = normals[face.normalIndex - 1];
					vertices.push_back(Model::Vertex(coord, normal));
				}
			}
		}

		objFile.close();

		model->_verticesCount = (GLuint)vertices.size();

		glGenBuffers(1, &(model->_arrayIndex));
		glBindBuffer(GL_ARRAY_BUFFER, model->_arrayIndex);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Model::Vertex), &vertices[0], GL_DYNAMIC_DRAW);

		glGenVertexArrays(1, &(model->_vertexArrayIndex));
		glBindVertexArray(model->_vertexArrayIndex);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Model::Vertex), (const void*)offsetof(Model::Vertex, coord));
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Model::Vertex), (const void*)offsetof(Model::Vertex, normal));
		glEnableVertexAttribArray(1);
		glBindVertexArray(0);
	}
	else
		cout << "[ObjFileLoader]: error opening the file (" << modelName << ")" << endl;
	
	return model;
}

// Проверка корректности пути к файлу.
bool ModelLoader::isCorrectFile(const char *modelName)
{
	size_t len = strlen(modelName);

	return modelName[len - 4] == '.' &&
		modelName[len - 3] == 'o' &&
		modelName[len - 2] == 'b' &&
		modelName[len - 1] == 'j';
}

// Пропустить строку файла.
void ModelLoader::skipLine(ifstream &file)
{
	char str[255];
	file.getline(str, 255);
}