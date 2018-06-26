#include "DisplayObject.h"

Shader *DisplayObject::_shapeShader = nullptr;
Camera *DisplayObject::_camera = nullptr;
Light *DisplayObject::_light = nullptr;
DisplayObject::ShowMode DisplayObject::_showMode = SHOW_MODEL;

// Конструктор объекта отображения.
DisplayObject::DisplayObject()
{
	_modelMatrix = nullptr;
	_model = nullptr;
	_shapeModel = nullptr;
	_shader = nullptr;
}

// Задать матрицу модели.
void DisplayObject::setModelMatrix(const float *modelMatrix)
{
	_modelMatrix = modelMatrix;
}

// Задать модель.
void DisplayObject::setModel(Model *model, Model *shapeModel)
{
	_model = model;
	_shapeModel = shapeModel;
}

// Отобразить объект.
void DisplayObject::show() const
{
	if (_camera == nullptr || _light == nullptr || _modelMatrix == nullptr)
		return;
	
	switch (_showMode)
	{
	case SHOW_MODEL:
		showModel();
		break;

	case SHOW_SHAPE:
		showShapeModel();
	}
}

// Вывод модели.
void DisplayObject::showModel() const
{
	if (_shader == nullptr || _model == nullptr)
		return;

	_shader->activate();
	_shader->setUniformMat4("uProjectionMatrix", _camera->getProjectionMatrix());

	mat4 viewMatrix = make_mat4(_camera->getViewMatrix());
	mat4 modelViewMatrix = viewMatrix * make_mat4(_modelMatrix);
	_shader->setUniformMat4("uModelViewMatrix", value_ptr(modelViewMatrix));

	_light->apply(_shader);
	_model->draw(_shader);
}

// Вывод модели формы.
void DisplayObject::showShapeModel() const
{
	if (_shapeShader == nullptr || _shapeModel == nullptr)
		return;

	_shapeShader->activate();
	_shapeShader->setUniformMat4("uProjectionMatrix", _camera->getProjectionMatrix());

	mat4 viewMatrix = make_mat4(_camera->getViewMatrix());
	mat4 modelViewMatrix = viewMatrix * make_mat4(_modelMatrix);
	_shapeShader->setUniformMat4("uModelViewMatrix", value_ptr(modelViewMatrix));

	_light->apply(_shapeShader);
	_shapeModel->draw(_shapeShader);
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

// Задать шейдер для модели формы.
void DisplayObject::setShapeShader(Shader *shader)
{
	_shapeShader = shader;
}

// Задать источник света.
void DisplayObject::setLight(Light *light)
{
	_light = light;
}

// Задать режим вывода.
void DisplayObject::setShowMode(ShowMode mode)
{
	_showMode = mode;
}

// Получить режим вывода.
DisplayObject::ShowMode DisplayObject::getShowMode()
{
	return _showMode;
}