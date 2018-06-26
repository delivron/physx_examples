#include "DisplayObject.h"

Shader *DisplayObject::_shader = nullptr;
Camera *DisplayObject::_camera = nullptr;
Light *DisplayObject::_light = nullptr;

// Конструктор объекта отображения.
DisplayObject::DisplayObject()
{
	_modelMatrix = nullptr;
	_model = nullptr;
	_material = nullptr;
}

// Задать матрицу модели.
void DisplayObject::setModelMatrix(const float *modelMatrix)
{
	_modelMatrix = modelMatrix;
}

// Задать модель.
void DisplayObject::setModel(Model *model)
{
	_model = model;
}

// Задать материал.
void DisplayObject::setMaterial(Material *material)
{
	_material = material;
}

// Отобразить объект.
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

// Задать шейдер, используемый для вывода.
void DisplayObject::setShader(Shader *shader)
{
	_shader = shader;
}

// Задать камеру.
void DisplayObject::setCamera(Camera *camera)
{
	_camera = camera;
}

// Задать источник света.
void DisplayObject::setLight(Light *light)
{
	_light = light;
}