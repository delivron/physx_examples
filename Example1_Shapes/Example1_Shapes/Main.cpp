#include "Main.h"

PxDefaultAllocator pxAllocator;
PxDefaultErrorCallback pxErrorCallback;
PxFoundation *pxFoundation = nullptr;
PxPhysics *pxPhysics = nullptr;
PxCooking *pxCooking = nullptr;
PxDefaultCpuDispatcher *pxCpuDispatcher = nullptr;
PxScene *pxScene = nullptr;

Model *boxModel = nullptr;
Model *sphereModel = nullptr;
Model *capsuleModel = nullptr;
Model *capsuleModel2 = nullptr;
Model *pyramidModel = nullptr;
Model *triangleMeshModel = nullptr;
Model *heightFieldModel = nullptr;
PxGeometry *boxGeometry = nullptr;
PxGeometry *sphereGeometry = nullptr;
PxGeometry *capsuleGeometry = nullptr;
PxGeometry *capsuleGeometry2 = nullptr;
PxGeometry *pyramidGeometry = nullptr;

PxRigidStatic *planeActor = nullptr;
PxRigidStatic *triangleMeshActor = nullptr;
PxRigidStatic *heightFieldActor = nullptr;

Camera camera;
Shader shader;
Light light;
Model planeModel;
Material areaMaterial;
DisplayObject areaDisplayObject;
Material objectMaterials[RAND_MATERIALS_COUNT];
vector<PhysicalObject*> objects;
PhysicalObject area;
PxMaterial *areaPhysMaterial = nullptr;

bool pauseFlag = false;
bool polygonFillMode = true;
float timeAccumulator = 0.0f;
char areaId = 0;

int main(int argc, char **argv)
{
	initializeGlut(argc, argv);
	initGlew();
	initializePhysX();
	initilaizeData();
	showProgramInfo();
	glutMainLoop();
	return 0;
}

// Инициализировать библиотеку GLUT.
void initializeGlut(int argc, char **argv)
{
	int windowPositionX = (GetSystemMetrics(SM_CXSCREEN) - WINDOW_WIDTH) / 2;
	int windowPositionY = (GetSystemMetrics(SM_CYSCREEN) - WINDOW_HEIGHT) / 2;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutInitContextVersion(4, 0);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutInitWindowPosition(windowPositionX, windowPositionY);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow(WINDOW_TITLE);

	glutReshapeFunc(reshapeHandler);
	glutDisplayFunc(displayHandler);
	glutIdleFunc(idleHandler);
}

// Инициализировать библиотеку GLEW.
void initGlew()
{
	GLenum status = glewInit();

	if (status != GLEW_OK)
	{
		cout << "[GLEW]: error " << glewGetErrorString(status) << endl;
		exit(-1);
	}

	if (!GLEW_ARB_vertex_buffer_object)
		cout << "[GLEW]: VBO is not supported." << endl;
}

// Инициализировать библиотеку PhysX.
void initializePhysX()
{
	pxFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, pxAllocator, pxErrorCallback);
	if (pxFoundation == nullptr)
	{
		cout << "[PhysX]: error while creating object \"PxFoundation\"" << endl;
		exit(-1);
	};

	PxTolerancesScale scale;
	pxPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *pxFoundation, scale);
	if (pxPhysics == nullptr)
	{
		cout << "[PhysX]: error while creating object \"PxPhysics\"" << endl;
		exit(-1);
	};
	if (!PxInitExtensions(*pxPhysics))
	{
		cout << "[PhysX]: initialization error (Extensions)" << endl;
		exit(-1);
	};

	pxCooking = PxCreateCooking(PX_PHYSICS_VERSION, *pxFoundation, PxCookingParams(scale));
	if (pxCooking == nullptr)
	{
		cout << "[PhysX]: error while creating object \"PxCooking\"" << endl;
		exit(-1);
	};

	pxCpuDispatcher = PxDefaultCpuDispatcherCreate(0);
	if (pxCpuDispatcher == nullptr)
	{
		cout << "[PhysX]: error while creating object \"PxDefaultCpuDispatcher\"" << endl;
		exit(-1);
	};

	PxSceneDesc sceneDesc(scale);
	sceneDesc.cpuDispatcher = pxCpuDispatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
	
	pxScene = pxPhysics->createScene(sceneDesc);
	if (pxFoundation == nullptr)
	{
		cout << "[PhysX]: error while creating object \"PxFoundation\"" << endl;
		exit(-1);
	};
}

// Инициализировать пользовательские данные.
void initilaizeData() 
{
	srand((unsigned int)time(nullptr));

	FpsController::getInstance().init();

	bool success = shader.load(PHONG_VERTEX_SHADER, PHONG_FRAGMENT_SHADER);
	if (!success)
		exit(-1);

	// передача необходимых данных для объекта отображения
	DisplayObject::setShader(&shader);
	DisplayObject::setCamera(&camera);
	DisplayObject::setLight(&light);

	initKeys();
	initMaterials();
	initModels();

	// настройка объекта отображения плоскости
	areaDisplayObject.setMaterial(&areaMaterial);
	areaDisplayObject.setModel(&planeModel);

	boxGeometry = new PxBoxGeometry(0.5f, 0.5f, 0.5f);
	sphereGeometry = new PxSphereGeometry(0.5f);
	capsuleGeometry = new PxCapsuleGeometry(0.5f, 0.5f);
	capsuleGeometry2 = new PxCapsuleGeometry(0.5f, 0.75f);

	// создание и добавление actor'а плоскости
	areaPhysMaterial = pxPhysics->createMaterial(0.5f, 0.3f, 0.5f);
	planeActor = PxCreatePlane(*pxPhysics, PxPlane(0.0f, 1.0f, 0.0f, 0.0f), *areaPhysMaterial);
	
	area.setDisplayObject(&areaDisplayObject);
	area.setActor(planeActor);
	area.addToScene(pxScene);

	initPyramid();
	initTriangleMeshArea();
	initHeightFieldArea();
}

// Вывод информации о программе.
void showProgramInfo()
{
	ifstream file("Readme.txt");
	char str[255];

	if (file.is_open())
	{
		setlocale(LC_ALL, "rus");

		while (!file.eof())
		{
			file.getline(str, 255);
			cout << str << endl;
		}

		file.close();
	}
}

// Регистрация кнопок управления.
void initKeys()
{
	KeyboardController::getInstance().addKey(VK_ESCAPE);
	KeyboardController::getInstance().addKey(VK_LEFT);
	KeyboardController::getInstance().addKey(VK_RIGHT);
	KeyboardController::getInstance().addKey(VK_DOWN);
	KeyboardController::getInstance().addKey(VK_UP);
	KeyboardController::getInstance().addKey(VK_ADD);
	KeyboardController::getInstance().addKey(VK_SUBTRACT);
	KeyboardController::getInstance().addKey(VK_SPACE);
	KeyboardController::getInstance().addKey(VK_TAB);
	KeyboardController::getInstance().addKey('1');
	KeyboardController::getInstance().addKey('2');
	KeyboardController::getInstance().addKey('3');
	KeyboardController::getInstance().addKey('4');
	KeyboardController::getInstance().addKey('5');
	KeyboardController::getInstance().addKey('W');
}

// Инициализация материалов.
void initMaterials()
{
	// параметры источника света
	light.setAmbient(1.0f, 1.0f, 1.0f);
	light.setDiffuse(1.0f, 1.0f, 1.0f);
	light.setSpecular(1.0f, 1.f, 1.0f);
	light.setPosition(50.0f, 50.0f, 0.0f);
	light.setCamera(&camera);

	// материал плоскости
	areaMaterial.setAmbient(0.0f, 0.15f, 0.3f);
	areaMaterial.setDiffuse(0.0f, 0.5f, 1.0f);

	// материалы объектов
	objectMaterials[0].setAmbient(0.3f, 0.0f, 0.0f);
	objectMaterials[0].setDiffuse(0.9f, 0.0f, 0.0f);
	objectMaterials[0].setSpecular(0.3f, 0.0f, 0.0f);
	objectMaterials[1].setAmbient(0.0f, 0.3f, 0.0f);
	objectMaterials[1].setDiffuse(0.0f, 0.9f, 0.0f);
	objectMaterials[1].setSpecular(0.0f, 0.3f, 0.0f);
	objectMaterials[2].setAmbient(0.3f, 0.0f, 0.3f);
	objectMaterials[2].setDiffuse(0.9f, 0.0f, 0.9f);
	objectMaterials[2].setSpecular(0.3f, 0.0f, 0.3f);
	objectMaterials[3].setAmbient(0.3f, 0.3f, 0.0f);
	objectMaterials[3].setDiffuse(0.9f, 0.9f, 0.0f);
	objectMaterials[3].setSpecular(0.3f, 0.3f, 0.0f);
}

// Инициализация моделей.
void initModels()
{
	// создание модели плоскости
	planeModel.createPlane(PLANE_SIZE);

	// загрузка моделей
	boxModel = ModelLoader::getInstance().load("Models/Box.obj");
	sphereModel = ModelLoader::getInstance().load("Models/Sphere.obj");
	capsuleModel = ModelLoader::getInstance().load("Models/Capsule.obj");
	capsuleModel2 = ModelLoader::getInstance().load("Models/Capsule2.obj");
	pyramidModel = ModelLoader::getInstance().load("Models/Pyramid.obj");
	triangleMeshModel = ModelLoader::getInstance().load("Models/TriangleMesh.obj");
	heightFieldModel = ModelLoader::getInstance().load("Models/HeightField.obj");
}

// Инициализация пирамиды.
void initPyramid()
{
	const PxVec3 pyramidVerts[5] =
	{
		PxVec3(-0.5f, -0.5f, -0.5f),
		PxVec3(-0.5f, -0.5f, 0.5f),
		PxVec3(0.5f, -0.5f, 0.5f),
		PxVec3(0.5f, -0.5f, -0.5f),
		PxVec3(0.0f, 0.5f, 0.0f)
	};
	PxConvexMeshDesc pyramidDesc;
	pyramidDesc.points.count = 5;
	pyramidDesc.points.stride = sizeof(PxVec3);
	pyramidDesc.points.data = pyramidVerts;
	pyramidDesc.flags = PxConvexFlag::eCOMPUTE_CONVEX;
	PxDefaultMemoryOutputStream pyramidBuf;
	PxConvexMeshCookingResult::Enum pyramidResult;

	pxCooking->cookConvexMesh(pyramidDesc, pyramidBuf, &pyramidResult);
	PxDefaultMemoryInputData pyramidInput(pyramidBuf.getData(), pyramidBuf.getSize());
	PxConvexMesh *pyramidMesh = pxPhysics->createConvexMesh(pyramidInput);
	pyramidGeometry = new PxConvexMeshGeometry(pyramidMesh);
}

// Инициализация поверхности из Triangle Mesh.
void initTriangleMeshArea()
{
	const PxVec3 triangleMeshVerts[12] =
	{
		PxVec3(-30.0f, 0.0f, -30.0f),
		PxVec3(30.0f, 0.0f, -30.0f),
		PxVec3(30.0f, 0.0f, 30.0f),
		PxVec3(-30.0f, 0.0f, 30.0f),
		PxVec3(-18.0f, 0.0f, -18.0f),
		PxVec3(18.0f, 0.0f, -18.0f),
		PxVec3(18.0f, 0.0f, 18.0f),
		PxVec3(-18.0f, 0.0f, 18.0f),
		PxVec3(-18.0f, -5.0f, -18.0f),
		PxVec3(18.0f, -5.0f, -18.0f),
		PxVec3(18.0f, -5.0f, 18.0f),
		PxVec3(-18.0f, -5.0f, 18.0f)
	};
	const PxU32 triangleMeshIndices[54] =
	{
		0, 4, 1,
		1, 4, 5,
		1, 5, 2,
		2, 5, 6,
		2, 6, 3,
		3, 6, 7,
		3, 7, 4,
		3, 4, 0,
		8, 11, 9,
		9, 11, 10,
		4, 8, 5,
		5, 8, 9,
		5, 9, 6,
		6, 9, 10,
		6, 10, 7,
		7, 10, 11,
		7, 11, 4,
		4, 11, 8
	};

	PxTriangleMeshDesc triangleMeshDesc;
	triangleMeshDesc.points.count = 12;
	triangleMeshDesc.points.stride = sizeof(PxVec3);
	triangleMeshDesc.points.data = triangleMeshVerts;
	triangleMeshDesc.triangles.count = 18;
	triangleMeshDesc.triangles.stride = 3 * sizeof(PxU32);
	triangleMeshDesc.triangles.data = triangleMeshIndices;
	
	PxDefaultMemoryOutputStream triangleMeshBuffer;
	pxCooking->cookTriangleMesh(triangleMeshDesc, triangleMeshBuffer);
	PxDefaultMemoryInputData triangleMeshInput(triangleMeshBuffer.getData(), triangleMeshBuffer.getSize());
	PxTriangleMesh *triangleMesh = pxPhysics->createTriangleMesh(triangleMeshInput);
	PxGeometry *triangleMeshGeometry = new PxTriangleMeshGeometry(triangleMesh);

	triangleMeshActor = pxPhysics->createRigidStatic(PxTransform(PxVec3(0.0f)));
	PxShape *triangleMeshShape = pxPhysics->createShape(*triangleMeshGeometry, *areaPhysMaterial);
	triangleMeshActor->attachShape(*triangleMeshShape);
}

// Инициализация ландшафта.
void initHeightFieldArea()
{
	const size_t width = 6;
	const size_t pointCount = width * width;
	const PxI16 maxHeight = 5;
	PxHeightFieldSample *heightFieldArray = new PxHeightFieldSample[pointCount]();
	for (size_t i = 0; i < pointCount; i++)
	{
		if (i != 14 && i != 15 && i != 20 && i != 21)
			heightFieldArray[i].height = maxHeight;
	}
	heightFieldArray[7].setTessFlag();
	heightFieldArray[21].setTessFlag();

	PxHeightFieldDesc heightFieldDesc;
	heightFieldDesc.format = PxHeightFieldFormat::eS16_TM;
	heightFieldDesc.nbRows = 6;
	heightFieldDesc.nbColumns = 6;
	heightFieldDesc.samples.data = heightFieldArray;
	heightFieldDesc.samples.stride = sizeof(PxHeightFieldSample);

	PxHeightField *heightField = pxCooking->createHeightField(
		heightFieldDesc,
		pxPhysics->getPhysicsInsertionCallback()
	);
	PxGeometry *gridGeometry = new PxHeightFieldGeometry(
		heightField,
		PxMeshGeometryFlags(),
		1.0f,
		PLANE_SIZE / (heightFieldDesc.nbRows - 1),
		PLANE_SIZE / (heightFieldDesc.nbColumns - 1)
	);
	heightFieldActor = pxPhysics->createRigidStatic(
		PxTransform(PxVec3(-0.5f * PLANE_SIZE, -(PxReal)maxHeight, -0.5f * PLANE_SIZE))
	);
	PxShape *heightFieldShape = pxPhysics->createShape(*gridGeometry, *areaPhysMaterial);
	heightFieldActor->attachShape(*heightFieldShape);
}

// Обработчик события изменения размера окна.
void reshapeHandler(int w, int h)
{
	camera.setProjectionMatrix(FOV_Y_ANGLE, (double)w / h, NEAR_Z_COORD, FAR_Z_COORD);
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

// Рендеринг кадра.
void displayHandler()
{
	updateTitle();

	glClearColor(0.99f, 0.99f, 0.99f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	PxMat44 identityMatrix = PxMat44(1.0f);
	area.updateDisplayObject(&identityMatrix);
	for (size_t i = 0; i < objects.size(); i++)
		objects[i]->updateDisplayObject();

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glutSwapBuffers();
}

// Функция простоя – выполнение расчётов.
void idleHandler()
{
	const float STEP_TIME = 1.0f / 60.0f;

	FpsController::getInstance().update();
	KeyboardController::getInstance().handle();
	keyboardHandler();

	float deltaTime = FpsController::getInstance().getLastDelta();

	if (!pauseFlag)
	{
		timeAccumulator += deltaTime;

		if (timeAccumulator > STEP_TIME)
		{
			timeAccumulator -= STEP_TIME;
			pxScene->simulate(STEP_TIME);
			pxScene->fetchResults(true);
		}
	}
	
	camera.move(deltaTime);

	glutPostRedisplay();
}

// Обновить заголовок окна.
void updateTitle()
{
	string areaName;
	string newWindowTitle = WINDOW_TITLE;
	int fps = FpsController::getInstance().getFps();

	switch (areaId)
	{
	case 0:
		areaName = "plane";
		break;

	case 1:
		areaName = "triangle mesh";
		break;

	case 2:
		areaName = "height field";
		break;
	}

	newWindowTitle += " [FPS: " + to_string(fps) + "] " +
		"[Pause: " + ((pauseFlag) ? "yes" : "no") + "] " +
		"[PolygonMode: " + ((polygonFillMode) ? "fill" : "line") + "] "
		"[Area: " + areaName + "]";

	glutSetWindowTitle(newWindowTitle.c_str());
}

// Обработчик клавиатуры.
void keyboardHandler()
{
	bool leftKey = KeyboardController::getInstance().keyIsDown(VK_LEFT);
	bool rightKey = KeyboardController::getInstance().keyIsDown(VK_RIGHT);
	bool upKey = KeyboardController::getInstance().keyIsDown(VK_UP);
	bool downKey = KeyboardController::getInstance().keyIsDown(VK_DOWN);
	bool forwardKey = KeyboardController::getInstance().keyIsDown(VK_ADD);
	bool backKey = KeyboardController::getInstance().keyIsDown(VK_SUBTRACT);

	camera.setKeyStatus(leftKey, rightKey, upKey, downKey, forwardKey, backKey);

	if (KeyboardController::getInstance().keyIsFirstDown(VK_ESCAPE))
		exit(0);

	if (KeyboardController::getInstance().keyIsFirstDown(VK_SPACE))
		pauseFlag = !pauseFlag;

	if (KeyboardController::getInstance().keyIsFirstDown(VK_TAB))
		switchArea();

	if (KeyboardController::getInstance().keyIsFirstDown('1'))
		addPhysicalObject(boxGeometry, boxModel, PxVec3(0.0f));

	if (KeyboardController::getInstance().keyIsFirstDown('2'))
		addPhysicalObject(sphereGeometry, sphereModel, PxVec3(0.0f));

	if (KeyboardController::getInstance().keyIsFirstDown('3'))
		addPhysicalObject(pyramidGeometry, pyramidModel, PxVec3(0.0f));

	if (KeyboardController::getInstance().keyIsFirstDown('4'))
	{
		addPhysicalObject(capsuleGeometry, capsuleModel, PxVec3(0.0f));

		int randVal = rand() % 2;
		if (randVal == 0)
			objects.back()->setOrientation(PxQuat(PxHalfPi, PxVec3(0.0f, 0.0f, 1.0f)));
	}
		
	if (KeyboardController::getInstance().keyIsFirstDown('5'))
	{
		addPhysicalObject(capsuleGeometry2, capsuleModel2, PxVec3(-1.2f, 0.0f, 0.0f));
		objects.back()->setOrientation(PxQuat(PxHalfPi, PxVec3(0.0f, 0.0f, 1.0f)));
	}

	if (KeyboardController::getInstance().keyIsFirstDown('W'))
	{
		polygonFillMode = !polygonFillMode;

		GLenum mode = (polygonFillMode) ? GL_FILL : GL_LINE;
		glPolygonMode(GL_FRONT_AND_BACK, mode);
	}
}

// Добавить физический объект на сцену.
void addPhysicalObject(PxGeometry *geometry, Model *model, PxVec3 massCenter)
{
	float delta = -0.1f + (float)(rand() % 2000) / 10000.0f;
	PhysicalObject *object = new PhysicalObject();
	DisplayObject *displayObject = new DisplayObject();
	PxRigidDynamic *actor = pxPhysics->createRigidDynamic(PxTransform(delta, 15.0f, delta));
	PxMaterial *physMaterial = pxPhysics->createMaterial(0.5f, 0.3f, 0.4f);
	PxShape *shape = pxPhysics->createShape(*geometry, *physMaterial);
	Material *material = &objectMaterials[rand() % RAND_MATERIALS_COUNT];

	actor->attachShape(*shape);
	actor->setMass(ACTOR_MASS);
	actor->setCMassLocalPose(PxTransform(massCenter));
	actor->isSleeping();

	displayObject->setMaterial(material);
	displayObject->setModel(model);

	object->setDisplayObject(displayObject);
	object->setActor(actor);

	objects.push_back(object);
	object->addToScene(pxScene);
	
	actor->addForce(PxVec3(0.0f, 100.0f, 0.0f));
}

// Переключить поверхность с удалением всех объектов.
void switchArea()
{
	areaId = (areaId + 1) % 3;
	area.removeFromScene();

	while (objects.size() != 0)
	{
		objects.back()->removeFromScene();
		objects.pop_back();
	}

	switch (areaId)
	{
	case 0:
		areaDisplayObject.setModel(&planeModel);
		area.setActor(planeActor);
		break;

	case 1:
		areaDisplayObject.setModel(triangleMeshModel);
		area.setActor(triangleMeshActor);
		break;

	case 2:
		areaDisplayObject.setModel(heightFieldModel);
		area.setActor(heightFieldActor);
	};

	area.addToScene(pxScene);
}