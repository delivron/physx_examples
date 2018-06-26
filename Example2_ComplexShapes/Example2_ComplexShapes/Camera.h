#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

using namespace std;
using namespace glm;

class Camera
{
private:
	static const float MIN_DISTANCE;
	static const float MAX_DISTANCE;
	static const float MIN_ANGLE_TANGAGE;
	static const float MAX_ANGLE_TANGAGE;
	static const float FULL_CIRCLE_TIME;
	static const float FULL_ZOOM_TIME;

	float _angleTangage;
	float _angleYaw;
	float _distance;
	bool _leftKey;
	bool _rightKey;
	bool _upKey;
	bool _downKey;
	bool _forwardKey;
	bool _backKey;
	mat4 _projectionMatrix;
	mat4 _viewMatrix;

	void updateViewMatrix();

public:
	Camera();
	void move(float deltaTime);
	void setKeyStatus(bool left, bool right, bool up, bool down, bool forward, bool back);
	const float* getViewMatrix() const;
	const float* getProjectionMatrix() const;
	void setProjectionMatrix(double fovy, double aspect, double zNear, double zFar);
};