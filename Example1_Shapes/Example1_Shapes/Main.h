#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include "Camera.h"
#include "Shader.h"
#include "Light.h"
#include "Model.h"
#include "Material.h"
#include "DisplayObject.h"
#include "PhysicalObject.h"
#include "FpsController.h"
#include "KeyboardController.h"
#include "ModelLoader.h"
#include "GL/freeglut.h"
#include "PxPhysicsAPI.h"

using namespace std;
using namespace physx;

// Параметры окна.
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const char *WINDOW_TITLE = "Creation of elementary shapes";

// Параметры камеры.
const double FOV_Y_ANGLE = 45.0;
const double NEAR_Z_COORD = 0.1;
const double FAR_Z_COORD = 250.0;

// Пути к шейдерам.
const char *PHONG_VERTEX_SHADER = "Shaders/Phong.vsh";
const char *PHONG_FRAGMENT_SHADER = "Shaders/Phong.fsh";

// Количество материалов.
const int RAND_MATERIALS_COUNT = 4;

// Размер плоскости.
const float PLANE_SIZE = 60.0f;

// Масса акторов.
const float ACTOR_MASS = 10.0f;

// Прототипы функций.
void initializeGlut(int argc, char **argv);
void initGlew();
void initializePhysX();
void initilaizeData();
void showProgramInfo();
void initKeys();
void initMaterials();
void initModels();
void initPyramid();
void initTriangleMeshArea();
void initHeightFieldArea();
void reshapeHandler(int w, int h);
void displayHandler();
void idleHandler();
void updateTitle();
void keyboardHandler();
void addPhysicalObject(PxGeometry *geometry, Model *model, PxVec3 massCenter);
void switchArea();