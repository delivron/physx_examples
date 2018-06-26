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
#include "IL/il.h"
#include "IL/ilu.h"
#include "IL/ilut.h"

using namespace std;
using namespace physx;

// Параметры окна.
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const char *WINDOW_TITLE = "Creation of complex shapes";

// Параметры камеры.
const double FOV_Y_ANGLE = 45.0;
const double NEAR_Z_COORD = 0.1;
const double FAR_Z_COORD = 250.0;

// Размер поверхности.
const float AREA_SIZE = 40.0f;

// Масса мяча.
const float BALL_MASS = 1.0f;

// Радиус появления мяча.
const float BALL_SPAWN_RADIUS = 17.5f;
const float BALL_SPAWN_SHORT_RADIUS = 0.2f;

// Высота появления мяча.
const float BALL_SPAWN_HEIGHT = 10.0f;

// Масса стола.
const float TABLE_MASS = 5.0f;

// Масса стула.
const float CHAIR_MASS = 2.5f;

// Прототипы функций.
void initializeGlut(int argc, char **argv);
void initGlew();
void initializePhysX();
void initilaizeData();
void showProgramInfo();
void initKeys();
void initLight();
void initArea();
void reshapeHandler(int w, int h);
void displayHandler();
void idleHandler();
void updateTitle();
void keyboardHandler();
void addBall(PxTransform &transform);
void addTable(PxTransform &transform);
void addChair(PxTransform &transform);