#include "DisplayObject.h"

Shader *DisplayObject::_shader = nullptr;
Camera *DisplayObject::_camera = nullptr;
Light *DisplayObject::_light = nullptr;

// ����������� ������� �����������.
DisplayObject::DisplayObject()
{
	_modelMatrix = nullptr;
	_model = nullptr;
	_material = nullptr;
}

// ������ ������� ������.
void DisplayObject::setModelMatrix(const float *modelMatrix)
{
	_modelMatrix = modelMatrix;
}

// ������ ������.
void DisplayObject::setModel(Model *model)
{
	_model = model;
}

// ������ ��������.
void DisplayObject::setMaterial(Material *material)
{
	_material = material;
}

// ���������� ������.
void DisplayObject::show() const
{
	if (_shader == nullptr || _model == nullptr || _material == nullptr ||
		_camera == nullptr || _light == nullptr || _modelMatrix == nullptr)
		return;
	
	_shader->activate();
	_shader->setUniformMat4("uProjectionMatrix", _camera->getProjectionMatrix());

	mat4 viewMatrix = make_mat4(_camera->getViewMatrix());
	mat4 modelViewMatrix = viewMatrix * make_mat4(_modelMatrix);
	_shader->setUniformMat4("uModelViewMatrix", value_ptr(modelViewMatrix));

	_material->apply(_shader);
	_light->apply(_shader);
	_model->draw();
}

// ������ ������, ������������ ��� ������.
void DisplayObject::setShader(Shader *shader)
{
	_shader = shader;
}

// ������ ������.
void DisplayObject::setCamera(Camera *camera)
{
	_camera = camera;
}

// ������ �������� �����.
void DisplayObject::setLight(Light *light)
{
	_light = light;
}