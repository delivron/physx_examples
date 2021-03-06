#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include "Model.h"
#include "FileLoader.h"
#include "MaterialLoader.h"
#include "glm/glm.hpp"

using namespace std;
using namespace glm;

class ModelLoader
{
public:
	static ModelLoader& getInstance();
	Model* load(const char *modelName);

private:
	struct Face
	{
		GLuint coordIndex, texCoordIndex, normalIndex;

		friend istream& operator>>(istream &is, Face &face)
		{
			char ch;
			return is >> face.coordIndex >> ch >> face.texCoordIndex >> ch >> face.normalIndex;
		};
	};

	enum KeywordCode { CODE_V, CODE_VN, CODE_VT, CODE_F, CODE_MTLLIB, CODE_USEMTL };

	map<string, KeywordCode> _keywordCodes;

	ModelLoader();
	~ModelLoader() {};
	ModelLoader(const ModelLoader&) {};
	ModelLoader& operator=(const ModelLoader&) {};
};