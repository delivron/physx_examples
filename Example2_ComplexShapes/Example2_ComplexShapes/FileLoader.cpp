#include "FileLoader.h"

// Проверка корректности формата файла.
bool FileLoader::isCorrectFile(const char *modelName, const char *fmt)
{
	size_t len = strlen(modelName);

	return strlen(fmt) == 3 &&
		modelName[len - 4] == '.' &&
		modelName[len - 3] == fmt[0] &&
		modelName[len - 2] == fmt[1] &&
		modelName[len - 1] == fmt[2];
}

// Пропустить строку файла.
void FileLoader::skipLine(ifstream &file)
{
	char str[255];
	file.getline(str, 255);
}

// Получить путь к папке модели.
string FileLoader::getPath(const string &fileName)
{
	int i = fileName.length() - 1;

	while (fileName[i] != '/' && fileName[i] != '\\' && i > 0)
		i--;

	return fileName.substr(0, i + 1);
}