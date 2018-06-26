#pragma once
#include <map>
#include <iostream>
#include <Windows.h>

using namespace std;

class KeyboardController
{
private:
	// 0-й бит Ц текущее состо€ние клавиши
	// 1-й бит Ц прошлое состо€ние клавиши
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