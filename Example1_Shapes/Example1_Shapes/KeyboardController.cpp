#include "KeyboardController.h"

// �������� ������ �� ��������� ������.
KeyboardController& KeyboardController::getInstance()
{
	static KeyboardController instance;
	return instance;
}

// �������� ������ ��� ������������ ���������.
void KeyboardController::addKey(int keyCode)
{
	keyStates[keyCode] = 0;
}

// ������� ���������� ������� ������� �������.
void KeyboardController::removeKey(int keyCode)
{
	keyStates.erase(keyCode);
}

// ��������� ������.
void KeyboardController::handle()
{
	for (auto i = keyStates.begin(); i != keyStates.end(); i++)
	{
		i->second <<= 1;
		i->second &= 3;
		i->second |= (char)((GetKeyState(i->first) >> 15) & 1);
	}
}

// ��������� ��������� ������� �������.
bool KeyboardController::keyIsDown(int keyCode)
{
	if (keyStates.find(keyCode) != keyStates.end())
		return keyStates[keyCode] & 1;
	else
		cout << "Key " << keyCode << " is not handle" << endl;

	return false;
}

// ��������� ��������� ������� ������� �������.
bool KeyboardController::keyIsFirstDown(int keyCode)
{
	if (keyStates.find(keyCode) != keyStates.end())
		return ((keyStates[keyCode] & 2) == 0) && (keyStates[keyCode] & 1);
	else
		cout << "Key " << keyCode << " is not handle" << endl;

	return false;
}