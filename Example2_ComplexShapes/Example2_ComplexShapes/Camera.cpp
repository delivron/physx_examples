#include "Camera.h"

#define PI 3.14159f

const float Camera::MIN_DISTANCE = 8.0f;
const float Camera::MAX_DISTANCE = 2000.0f;
const float Camera::MIN_ANGLE_TANGAGE = 0.35f;
const float Camera::MAX_ANGLE_TANGAGE = 1.57f;
const float Camera::FULL_CIRCLE_TIME = 3.0f;
const float Camera::FULL_ZOOM_TIME = 1.5f;

// Конструктор камеры.
Camera::Camera()
{
	_angleTangage = MIN_ANGLE_TANGAGE + 0.5f * (MAX_ANGLE_TANGAGE - MIN_ANGLE_TANGAGE);
	_angleYaw = 0.0f;
	_distance = MIN_DISTANCE + 0.5f * (MAX_DISTANCE - MIN_DISTANCE);

	updateViewMatrix();

	_leftKey = false;
	_rightKey = false;
	_upKey = false;
	_downKey = false;
	_forwardKey = false;
	_backKey = false;
}

// Движение камеры.
void Camera::move(float deltaTime)
{
	float deltaAngle = 2.0f * PI / (FULL_CIRCLE_TIME / deltaTime);
	float deltaDistance = (MAX_DISTANCE - MIN_DISTANCE) / (FULL_ZOOM_TIME / deltaTime);

	if (_leftKey || _rightKey || _upKey || _downKey || _backKey || _forwardKey)
	{
		if (_leftKey)
		{
			_angleYaw += deltaAngle;
			if (_angleYaw >= 2 * PI)
				_angleYaw -= 2 * PI;
		}

		if (_rightKey)
		{
			_angleYaw -= deltaAngle;
			if (_angleYaw < 0)
				_angleYaw += 2 * PI;
		}

		if (_upKey)
		{
			_angleTangage += deltaAngle;
			if (_angleTangage > MAX_ANGLE_TANGAGE)
				_angleTangage = MAX_ANGLE_TANGAGE;
		}

		if (_downKey)
		{
			_angleTangage -= deltaAngle;
			if (_angleTangage < MIN_ANGLE_TANGAGE)
				_angleTangage = MIN_ANGLE_TANGAGE;
		}

		if (_forwardKey)
		{
			_distance -= deltaDistance;
			if (_distance < MIN_DISTANCE)
				_distance = MIN_DISTANCE;
		}

		if (_backKey)
		{
			_distance += deltaDistance;
			if (_distance > MAX_DISTANCE)
				_distance = MAX_DISTANCE;
		}

		updateViewMatrix();
	}
}

// Установить статус кнопок.
void Camera::setKeyStatus(bool left, bool right, bool up, bool down, bool forward, bool back)
{
	_leftKey = left;
	_rightKey = right;
	_upKey = up;
	_downKey = down;
	_forwardKey = forward;
	_backKey = back;
}

// Получить матрицу вида.
const float* Camera::getViewMatrix() const
{
	return value_ptr(_viewMatrix);
}

// Получить матрицу проекции.
const float* Camera::getProjectionMatrix() const
{
	return value_ptr(_projectionMatrix);
}

// Установить матрицу проекции.
void Camera::setProjectionMatrix(double fovY, double aspect, double zNear, double zFar)
{
	_projectionMatrix = perspective(fovY, aspect, zNear, zFar);
}

// Обновить матрицу вида.
void Camera::updateViewMatrix()
{
	vec3 eye;
	vec3 up;

	eye.x = _distance * cos(_angleYaw) * cos(_angleTangage);
	eye.y = _distance * sin(_angleTangage);
	eye.z = _distance * sin(_angleYaw) * cos(_angleTangage);
	
	_viewMatrix = lookAt(eye, vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
}