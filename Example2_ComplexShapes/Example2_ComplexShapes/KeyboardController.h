#pragma once
#include <map>
#include <iostream>
#include <Windows.h>

using namespace std;

class KeyboardController
{
private:
	// 0-� ��� � ������� ��������� �������
	// 1-� ��� � ������� ��������� �������
	map<int, char> keyStates;

	KeyboardController() {};
	~KeyboardController() {};
	KeyboardController(const KeyboardController&) {};
	KeyboardController& operator=(const KeyboardController&) {};

public:
	static KeyboardController& getInstance();
	void addKey(int keyCode);
	void removeKey(int keyCode);
	void handle();
	bool keyIsDown(int keyCode);
	bool keyIsFirstDown(int keyCode);
};