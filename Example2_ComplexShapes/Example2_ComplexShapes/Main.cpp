#include "Main.h"

PxDefaultAllocator pxAllocator;
PxDefaultErrorCallback pxErrorCallback;
PxFoundation *pxFoundation = nullptr;
PxPhysics *pxPhysics = nullptr;
PxCooking *pxCooking = nullptr;
PxDefaultCpuDispatcher *pxCpuDispatcher = nullptr;
PxScene *pxScene = nullptr;

Camera camera;
Light light;
Shader phongShader;
Shader texPhongShader;

PxMaterial *ballPhysMaterial = nullptr;
PxMaterial *furniturePhysMaterial = nullptr;
Model *ballModel = nullptr;
Model *ballShapeModel = nullptr;
Model *tableModel = nullptr;
Model *tableShapeModel = nullptr;
Model *chairModel = nullptr;
Model *chairShapeModel = nullptr;
PhysicalObject *area = nullptr;
vector<PhysicalObject*> objects;

bool pauseFlag = false;
bool polygonFillMode = true;
float timeAccumulator = 0.0f;
int furnitureCount = 0;

int main(int argc, char **argv)
{
	initializeGlut(argc, argv);
	initGlew();
	initializePhysX();
	ilInit();
	iluInit();
	ilutInit();
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

	bool success = phongShader.load("Shaders/Phong.vsh", "Shaders/Phong.fsh");
	success &= texPhongShader.load("Shaders/TexPhong.vsh", "Shaders/TexPhong.fsh");
	if (!success)
		exit(-1);

	DisplayObject::setCamera(&camera);
	DisplayObject::setLight(&light);
	DisplayObject::setShapeShader(&phongShader);

	ballPhysMaterial = pxPhysics->createMaterial(0.5f, 0.4f, 0.6f);
	furniturePhysMaterial = pxPhysics->createMaterial(0.1f, 0.05f, 0.05f);

	ballModel = ModelLoader::getInstance().load("Models/SoccerBall/SoccerBall.obj");
	ballShapeModel = ModelLoader::getInstance().load("Models/SoccerBall/SoccerBallShape.obj");
	tableModel = ModelLoader::getInstance().load("Models/Pizza/Building_Pizza2.obj");
	tableShapeModel = ModelLoader::getInstance().load("Models/Pizza/Building_Pizza2.obj");
    chairModel = ModelLoader::getInstance().load("Models/Chair/Chair.obj");
    chairShapeModel = ModelLoader::getInstance().load("Models/Chair/ChairShape.obj");

	initKeys();
	initLight();
	initArea();
	addTable(PxTransform(0.0f, 1.5f, 0.0f));
	addChair(PxTransform(-3.5f, 1.5f, 0.0f, PxQuat(PxHalfPi, PxVec3(0.0f, 1.0f, 0.0f))));
	addChair(PxTransform(3.5f, 1.5f, 0.0f, PxQuat(-PxHalfPi, PxVec3(0.0f, 1.0f, 0.0f))));
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
	KeyboardController::getInstance().addKey('Z');
}

// Инициализация параметров источника света.
void initLight()
{
	light.setAmbient(1.0f, 1.0f, 1.0f);
	light.setDiffuse(1.0f, 1.0f, 1.0f);
	light.setSpecular(1.0f, 1.f, 1.0f);
	light.setPosition(1000.0f, 1000.0f, 1000.0f);
	light.setCamera(&camera);
}

// Инициализация поверхности.
void initArea()
{
	DisplayObject *dispObject = new DisplayObject();
	Model *model = ModelLoader::getInstance().load("Models/HeightField/HeightField.obj");
	Model *shapeModel = ModelLoader::getInstance().load("Models/HeightField/HeightFieldShape.obj");

	dispObject->setModel(model, shapeModel);
	dispObject->setShader(&texPhongShader);

	PxHeightFieldSample *heightFieldArray = new PxHeightFieldSample[25]();
	heightFieldArray[0].setTessFlag();
	heightFieldArray[18].setTessFlag();
	heightFieldArray[0].height = heightFieldArray[1].height = 5;
	heightFieldArray[2].height = heightFieldArray[3].height = 5;
	heightFieldArray[4].height = heightFieldArray[9].height = 5;
	heightFieldArray[14].height = heightFieldArray[19].height = 5;
	heightFieldArray[24].height = heightFieldArray[23].height = 5;
	heightFieldArray[22].height = heightFieldArray[21].height = 5;
	heightFieldArray[20].height = heightFieldArray[15].height = 5;
	heightFieldArray[2].height = heightFieldArray[3].height = 5;
	heightFieldArray[10].height = heightFieldArray[5].height = 5;

	PxHeightFieldDesc heightFieldDesc;
	heightFieldDesc.format = PxHeightFieldFormat::eS16_TM;
	heightFieldDesc.nbRows = 5;
	heightFieldDesc.nbColumns = 5;
	heightFieldDesc.samples.data = heightFieldArray;
	heightFieldDesc.samples.stride = sizeof(PxHeightFieldSample);

	PxHeightField *heightField = pxCooking->createHeightField(
		heightFieldDesc,
		pxPhysics->getPhysicsInsertionCallback()
	);
	PxGeometry *areaGeometry = new PxHeightFieldGeometry(
		heightField,
		PxMeshGeometryFlags(),
		1.0f,
		AREA_SIZE / (heightFieldDesc.nbRows - 1),
		AREA_SIZE / (heightFieldDesc.nbColumns - 1)
	);
	PxRigidStatic *areaActor = pxPhysics->createRigidStatic(
		PxTransform(PxVec3(-0.5f * AREA_SIZE, 0.0f, -0.5f * AREA_SIZE))
	);
	PxMaterial *areaPhysMaterial = pxPhysics->createMaterial(0.5f, 0.3f, 0.3f);
	PxShape *areaShape = pxPhysics->createShape(*areaGeometry, *areaPhysMaterial);
	areaActor->attachShape(*areaShape);

	area = new PhysicalObject();
	area->setDisplayObject(dispObject);
	area->setActor(areaActor);
	area->addToScene(pxScene);
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
	area->updateDisplayObject(&identityMatrix);

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
	string showModeStr;
	string newWindowTitle = WINDOW_TITLE;
	int fps = FpsController::getInstance().getFps();

	switch (DisplayObject::getShowMode())
	{
	case DisplayObject::SHOW_MODEL:
		showModeStr = "model";
		break;

	case DisplayObject::SHOW_SHAPE:
		showModeStr = "shape";
	}

	newWindowTitle += " [FPS: " + to_string(fps) + "] " +
		"[Pause: " + ((pauseFlag) ? "yes" : "no") + "] " +
		"[ShowMode: " + showModeStr + "]";

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
	{
		DisplayObject::ShowMode mode = DisplayObject::getShowMode();
		mode = (DisplayObject::ShowMode)((mode + 1) % 2);
		DisplayObject::setShowMode(mode);
	}

	bool key1Flag = KeyboardController::getInstance().keyIsFirstDown('1');
	bool key2Flag = KeyboardController::getInstance().keyIsFirstDown('2');
	if (key1Flag || key2Flag)
	{
		float radius = (key1Flag) ? BALL_SPAWN_RADIUS : BALL_SPAWN_SHORT_RADIUS;
		float angle = (float)(rand() % 360);
		float x = radius * cos(angle);
		float z = radius * sin(angle);
		addBall(PxTransform(x, BALL_SPAWN_HEIGHT, z));
	}

	if (KeyboardController::getInstance().keyIsFirstDown('Z'))
	{
		while (objects.size() > (size_t)furnitureCount)
		{
			objects.back()->removeFromScene();
			objects.pop_back();
		}
	}
}

// Добавить мяч на сцену.
void addBall(PxTransform &transform)
{
	PhysicalObject *object = new PhysicalObject();
	DisplayObject *displayObject = new DisplayObject();
	PxRigidDynamic *actor = pxPhysics->createRigidDynamic(transform);
	PxGeometry *geometry = new PxSphereGeometry(0.5f);
	PxShape *shape = pxPhysics->createShape(*geometry, *ballPhysMaterial);

	actor->attachShape(*shape);
	actor->setMass(BALL_MASS);

	displayObject->setModel(ballModel, ballShapeModel);
	displayObject->setShader(&phongShader);

	object->setDisplayObject(displayObject);
	object->setActor(actor);

	objects.push_back(object);
	object->addToScene(pxScene);
}

// Добавить стол на сцену.
void addTable(PxTransform &transform)
{
	PhysicalObject *table = new PhysicalObject();
	DisplayObject *tableDisplayObject = new DisplayObject();
	PxRigidDynamic *tableActor = pxPhysics->createRigidDynamic(transform);
	PxGeometry *rectGeometry = new PxBoxGeometry(2.67f, 0.16f, 1.4f);
	PxShape *rectShape = pxPhysics->createShape(*rectGeometry, *furniturePhysMaterial);
	PxGeometry *legGeometries[4];
	PxShape *legShapes[4];

	for (size_t i = 0; i < 4; i++)
	{
		legGeometries[i] = new PxCapsuleGeometry(0.085f, 0.845f);
		legShapes[i] = pxPhysics->createShape(*legGeometries[i], *furniturePhysMaterial);
		tableActor->attachShape(*legShapes[i]);
	}

	// зададим координаты "ножек" стола
	PxQuat orientation(PxHalfPi, PxVec3(0.0f, 0.0f, 1.0f));
	legShapes[0]->setLocalPose(PxTransform(2.5466f, -0.26f, 1.25457f, orientation));
	legShapes[1]->setLocalPose(PxTransform(2.5466f, -0.26f, -1.25457f, orientation));
	legShapes[2]->setLocalPose(PxTransform(-2.5466f, -0.26f, -1.25457f, orientation));
	legShapes[3]->setLocalPose(PxTransform(-2.5466f, -0.26f, 1.25457f, orientation));
	
	// добавим "плоскость" стола
	PxTransform massCenter(0.0f, 0.81516f, 0.0f);
	rectShape->setLocalPose(massCenter);
	tableActor->attachShape(*rectShape);
	
	tableActor->setMass(TABLE_MASS);
	tableActor->setCMassLocalPose(massCenter);

	tableDisplayObject->setModel(tableModel, tableShapeModel);
	tableDisplayObject->setShader(&texPhongShader);

	table->setDisplayObject(tableDisplayObject);
	table->setActor(tableActor);

	objects.push_back(table);
	table->addToScene(pxScene);
	furnitureCount++;
}

// Добавить стул на сцену.
void addChair(PxTransform &transform)
{
	PhysicalObject *chair = new PhysicalObject();
	DisplayObject *chairDisplayObject = new DisplayObject();
	PxRigidDynamic *chairActor = pxPhysics->createRigidDynamic(transform);
	PxGeometry *seatGeometry = new PxBoxGeometry(0.58f, 0.09f, 0.55f);
	PxShape *seatShape = pxPhysics->createShape(*seatGeometry, *furniturePhysMaterial);
	PxGeometry *backGeometry = new PxBoxGeometry(0.57f, 0.455f, 0.075f);
	PxShape *backShape = pxPhysics->createShape(*backGeometry, *furniturePhysMaterial);
	PxGeometry *legGeometry = new PxBoxGeometry(0.03f, 0.55f, 0.03f);
	PxShape *frontLeftLegShape = pxPhysics->createShape(*legGeometry, *furniturePhysMaterial);
	PxShape *frontRightLegShape = pxPhysics->createShape(*legGeometry, *furniturePhysMaterial);
	PxShape *backLeftLegShape = pxPhysics->createShape(*legGeometry, *furniturePhysMaterial);
	PxShape *backRightLegShape = pxPhysics->createShape(*legGeometry, *furniturePhysMaterial);

	seatShape->setLocalPose(PxTransform(0.0f, -0.06f, 0.168f));
	chairActor->attachShape(*seatShape);

	PxQuat backOrientation(-0.2112f, PxVec3(1.0f, 0.0f, 0.0f));
	backShape->setLocalPose(PxTransform(0.0f, 0.46027f, -0.42877f, backOrientation));
	chairActor->attachShape(*backShape);

	PxQuat frontLegOrientation(-0.0785f, PxVec3(1.0f, 0.0f, 0.0f));
	frontLeftLegShape->setLocalPose(PxTransform(0.5751f, -0.6587f, 0.7268f, frontLegOrientation));
	chairActor->attachShape(*frontLeftLegShape);
	frontRightLegShape->setLocalPose(PxTransform(-0.5751f, -0.6587f, 0.7268f, frontLegOrientation));
	chairActor->attachShape(*frontRightLegShape);
	
	PxQuat backLegOrientation(0.2199f, PxVec3(1.0f, 0.0f, 0.0f));
	backLeftLegShape->setLocalPose(PxTransform(0.5441f, -0.6666f, -0.4404f, backLegOrientation));
	chairActor->attachShape(*backLeftLegShape);
	backRightLegShape->setLocalPose(PxTransform(-0.5441f, -0.6666f, -0.4404f, backLegOrientation));
	chairActor->attachShape(*backRightLegShape);

	chairActor->setMass(CHAIR_MASS);

	chairDisplayObject->setModel(chairModel, chairShapeModel);
	chairDisplayObject->setShader(&texPhongShader);

	chair->setDisplayObject(chairDisplayObject);
	chair->setActor(chairActor);

	objects.push_back(chair);
	chair->addToScene(pxScene);
	furnitureCount++;
}