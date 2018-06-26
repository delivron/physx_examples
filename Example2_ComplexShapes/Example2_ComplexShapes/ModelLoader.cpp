#include "ModelLoader.h"

// Конструктор загрузчика.
ModelLoader::ModelLoader()
{
	_keywordCodes["v"] = CODE_V;
	_keywordCodes["vn"] = CODE_VN;
	_keywordCodes["vt"] = CODE_VT;
	_keywordCodes["f"] = CODE_F;
	_keywordCodes["mtllib"] = CODE_MTLLIB;
	_keywordCodes["usemtl"] = CODE_USEMTL;
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
	Material::Materials* materials = nullptr;
	Model *model = nullptr;
	vector<vec3> coords;
	vector<vec2> texCoords;
	vector<vec3> normals;
	vector<Model::Vertex> vertices;
	string dataType;
	string materialName;
	string mtlFileName;
	Face face;
	vec3 coord, normal;
	vec2 texCoord;
	float x, y, z;

	if (FileLoader::isCorrectFile(modelName, "obj") && objFile.is_open())
	{
		model = new Model();

		while (!objFile.eof())
		{
			objFile >> dataType;

			if (_keywordCodes.find(dataType) == _keywordCodes.end())
			{
				FileLoader::skipLine(objFile);
				continue;
			}

			switch (_keywordCodes[dataType])
			{
			case CODE_MTLLIB:
				objFile >> mtlFileName;
				mtlFileName = FileLoader::getPath(modelName) + mtlFileName;
				materials = MaterialLoader::getInstance().load(mtlFileName.c_str());
				model->setMaterials(materials);
				break;

			case CODE_USEMTL:
				objFile >> materialName;
				model->useMaterial(materialName, (int)vertices.size());
				break;

			case CODE_V:
				objFile >> x >> y >> z;
				coords.push_back(vec3(x, y, z));
				break;

			case CODE_VT:
				objFile >> x >> y;
				texCoords.push_back(vec2(x, 1.0f - y));
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

					if (face.texCoordIndex > 0)
						texCoord = texCoords[face.texCoordIndex - 1];
					else
						texCoord = vec2(0.0f, 0.0f);

					vertices.push_back(Model::Vertex(coord, texCoord, normal));
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
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Model::Vertex), (const void*)offsetof(Model::Vertex, texCoord));
		glEnableVertexAttribArray(2);
		glBindVertexArray(0);
	}
	else
		cout << "[ObjFileLoader]: error opening the file (" << modelName << ")" << endl;
	
	return model;
}