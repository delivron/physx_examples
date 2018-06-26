#pragma once
#include <fstream>
#include <string>

using namespace std;

class FileLoader
{
public:
	static bool isCorrectFile(const char *modelName, const char *fmt);
	static void skipLine(ifstream &file);
	static string getPath(const string &fileName);
};