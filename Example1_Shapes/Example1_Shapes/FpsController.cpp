#include "FpsController.h"

// Получить ссылку на экземпляр класса.
FpsController& FpsController::getInstance()
{
	static FpsController instance;
	return instance;
}

// Инициализация класса.
void FpsController::init()
{
	QueryPerformanceFrequency((LARGE_INTEGER*)&_frequency);
	QueryPerformanceCounter((LARGE_INTEGER*)&_lastTime);
	_fpsTime = 0.0;
	_updateCount = 0;
	_lastDelta = 0;
	_fps = 0;
}

// Обновить FPS.
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

// Получить количество тиков, прошедшее с момента с последнего обновления.
float FpsController::getLastDelta() const
{
	return _lastDelta;
}

// Получить текующее значение частоты кадров.
int FpsController::getFps() const
{
	return _fps;
}