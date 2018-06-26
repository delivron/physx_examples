#include "FpsController.h"

// �������� ������ �� ��������� ������.
FpsController& FpsController::getInstance()
{
	static FpsController instance;
	return instance;
}

// ������������� ������.
void FpsController::init()
{
	QueryPerformanceFrequency((LARGE_INTEGER*)&_frequency);
	QueryPerformanceCounter((LARGE_INTEGER*)&_lastTime);
	_fpsTime = 0.0;
	_updateCount = 0;
	_lastDelta = 0;
	_fps = 0;
}

// �������� FPS.
void FpsController::update()
{
	ULONGLONG currentTime;
	float delta;

	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
	delta = (float)(currentTime - _lastTime) / _frequency;

	_lastTime = currentTime;
	_fpsTime += delta;
	_lastDelta = delta;
	_updateCount++;

	if (_fpsTime >= 1.0f)
	{
		_fps = _updateCount;
		_fpsTime = 0.0f;
		_updateCount = 0;
	}
}

// �������� ���������� �����, ��������� � ������� � ���������� ����������.
float FpsController::getLastDelta() const
{
	return _lastDelta;
}

// �������� �������� �������� ������� ������.
int FpsController::getFps() const
{
	return _fps;
}