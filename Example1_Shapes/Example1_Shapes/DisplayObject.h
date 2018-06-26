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
	DisplayObject();
	void setModelMatrix(const float *modelMatrix);
	void setModel(Model *model);
	void setMaterial(Material *material);
	void show() const;
	static void setShader(Shader *shader);
	static void setCamera(Camera *camera);
	static void setLight(Light *light);

private:
	const float *_modelMatrix;
	Model *_model;
	Material *_material;
	static Shader *_shader;
	static Camera *_camera;
	static Light *_light;
};