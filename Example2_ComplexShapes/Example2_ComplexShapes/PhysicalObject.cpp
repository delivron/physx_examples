#include "PhysicalObject.h"

// Конструктор физического объекта.
PhysicalObject::PhysicalObject()
{
	_displayObject = nullptr;
	_actor = nullptr;
}

// Задать графическое представление.
void PhysicalObject::setDisplayObject(DisplayObject *displayObject)
{
	_displayObject = displayObject;
}

// Задать указатель на объект актора.
void PhysicalObject::setActor(PxRigidActor *actor)
{
	_actor = actor;
}

// Добавить на сцену.
void PhysicalObject::addToScene(PxScene *scene)
{
	if (scene != nullptr && _actor != nullptr)
		scene->addActor(*_actor);
}

// Удалить со сцены.
void PhysicalObject::removeFromScene()
{
	if (_actor != nullptr)
	{
		PxScene *scene = _actor->getScene();

		if (scene != nullptr)
			scene->removeActor(*_actor);
	}
}

// Обновить графическое представление.
void PhysicalObject::updateDisplayObject(PxMat44 *matrix)
{
	if (_actor != nullptr)
	{
		if (matrix == nullptr)
		{
			PxTransform transform = _actor->getGlobalPose();
			PxMat44	m = PxMat44(transform);

			_displayObject->setModelMatrix(m.front());
		}
		else
			_displayObject->setModelMatrix(matrix->front());

		_displayObject->show();
	}
}

// Задать позицию.
void PhysicalObject::setPosition(PxVec3 coord)
{
	if (_actor != nullptr)
		_actor->setGlobalPose(PxTransform(coord));
}

// Задать ориентацию.
void PhysicalObject::setOrientation(PxQuat &orient)
{
	if (_actor != nullptr)
		_actor->setGlobalPose(
			PxTransform(_actor->getGlobalPose().p, orient)
		);
}