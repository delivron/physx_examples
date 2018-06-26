#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include "Material.h"
#include "FileLoader.h"

using namespace std;

class MaterialLoader
{
public:
	static MaterialLoader &getInstance();
	Material::Materials* load(const char *modelName);

private:
	enum KeywordCode { CODE_NEWMTL, CODE_KA, CODE_KD, CODE_KS, CODE_NS, CODE_D, CODE_MAPKD };

	map<string, KeywordCode> _keywordCodes;

	MaterialLoader();
	~MaterialLoader() {};
	MaterialLoader(const MaterialLoader&) {};
	MaterialLoader& operator=(const MaterialLoader&) {};
};