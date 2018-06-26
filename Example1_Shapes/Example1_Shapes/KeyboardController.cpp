#include "KeyboardController.h"

// Получить ссылку на экземпляр класса.
KeyboardController& KeyboardController::getInstance()
{
	static KeyboardController instance;
	return instance;
}

// Добавить кнопку для отслеживания состояния.
void KeyboardController::addKey(int keyCode)
{
	keyStates[keyCode] = 0;
}

// Удалить обработчик события нажатия клавиши.
void KeyboardController::removeKey(int keyCode)
{
	keyStates.erase(keyCode);
}

// Обработка клавиш.
void KeyboardController::handle()
{
	for (auto i = keyStates.begin(); i != keyStates.end(); i++)
	{
		i->second <<= 1;
		i->second &= 3;
		i->second |= (char)((GetKeyState(i->first) >> 15) & 1);
	}
}

// Проверить состояние нажатия клавиши.
bool KeyboardController::keyIsDown(int keyCode)
{
	if (keyStates.find(keyCode) != keyStates.end())
		return keyStates[keyCode] & 1;
	else
		cout << "Key " << keyCode << " is not handle" << endl;

	return false;
}

// Проверить состояние первого нажатия клавиши.
bool KeyboardController::keyIsFirstDown(int keyCode)
{
	if (keyStates.find(keyCode) != keyStates.end())
		return ((keyStates[keyCode] & 2) == 0) && (keyStates[keyCode] & 1);
	else
		cout << "Key " << keyCode << " is not handle" << endl;

	return false;
}