#pragma once
#include "Shader.h"
#include "Model.h"
#include "Material.h"
#include "Camera.h"
#include "Light.h"
#include "glm/glm.hpp"

class DisplayObject
{
public:
	enum ShowMode { SHOW_MODEL = 0, SHOW_SHAPE = 1 };

	DisplayObject();
	void setModelMatrix(const float *modelMatrix);
	void setModel(Model *model, Model *shapeModel);
	void show() const;
	void setShader(Shader *shader);
	static void setShapeShader(Shader *shader);
	static void setCamera(Camera *camera);
	static void setLight(Light *light);
	static void setShowMode(ShowMode mode);
	static ShowMode getShowMode();

private:
	const float *_modelMatrix;
	Model *_model;
	Model *_shapeModel;
	Shader *_shader;
	static Shader *_shapeShader;
	static Camera *_camera;
	static Light *_light;
	static ShowMode _showMode;

	void showModel() const;
	void showShapeModel() const;
};