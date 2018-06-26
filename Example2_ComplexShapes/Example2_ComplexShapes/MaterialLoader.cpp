#include "MaterialLoader.h"

// Конструктор загрузчика.
MaterialLoader::MaterialLoader()
{
	_keywordCodes["newmtl"] = CODE_NEWMTL;
	_keywordCodes["Ka"] = CODE_KA;
	_keywordCodes["Kd"] = CODE_KD;
	_keywordCodes["Ks"] = CODE_KS;
	_keywordCodes["Ns"] = CODE_NS;
	_keywordCodes["map_Kd"] = CODE_MAPKD;
}

// Получить ссылку на экземпляр класса.
MaterialLoader& MaterialLoader::getInstance()
{
	static MaterialLoader instance;
	return instance;
}

// Загрузить материалы с файла формата "mtl".
Material::Materials* MaterialLoader::load(const char *fileName)
{
	ifstream mtlFile(fileName, ios::in);
	Material::Materials *materials = new Material::Materials();
	Material *newMaterial = nullptr;
	Texture *newTexture = nullptr;
	string materialName;
	string texturePath;
	string dataType;
	char textureFile[255];
	GLfloat shininess;
	GLfloat r;
	GLfloat g;
	GLfloat b;

	if (FileLoader::isCorrectFile(fileName, "mtl") && mtlFile.is_open())
	{
		while (!mtlFile.eof())
		{
			mtlFile >> dataType;

			if (_keywordCodes.find(dataType) == _keywordCodes.end())
			{
				FileLoader::skipLine(mtlFile);
				continue;
			}

			switch (_keywordCodes[dataType])
			{
			case CODE_NEWMTL:
				mtlFile >> materialName;
				newMaterial = new Material();
				materials->insert(
					pair<string, Material*>(materialName, newMaterial)
				);
				break;

			case CODE_KA:
				mtlFile >> r >> g >> b;
				newMaterial->setAmbient(r, g, b);
				break;

			case CODE_KD:
				mtlFile >> r >> g >> b;
				newMaterial->setDiffuse(r, g, b);
				break;

			case CODE_KS:
				mtlFile >> r >> g >> b;
				newMaterial->setSpecular(r, g, b);
				break;

			case CODE_NS:
				mtlFile >> shininess;
				newMaterial->setShininess(shininess);
				break;

			case CODE_MAPKD:
				mtlFile.getline(textureFile, 255);
				texturePath = FileLoader::getPath(fileName);
				texturePath += string(textureFile + 1);

				newTexture = new Texture();
				newTexture->load(texturePath.c_str());
				newMaterial->setTexture(newTexture);
			}
		}

		mtlFile.close();
	}
	else
		cout << "[MaterialLoader]: error opening the file (" << fileName << ")" << endl;

	return materials;
}