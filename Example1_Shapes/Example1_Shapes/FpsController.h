#pragma once
#include <Windows.h>

class FpsController
{
public:
	static FpsController& getInstance();
	void init();
	void update();
	float getLastDelta() const;
	int getFps() const;

private:
	ULONGLONG _frequency;
	ULONGLONG _lastTime;
	float _fpsTime;
	float _lastDelta;
	int _updateCount;
	int _fps;

	FpsController() {};
	~FpsController() {};
	FpsController(const FpsController&) {};
	FpsController& operator=(const FpsController&) {};
};