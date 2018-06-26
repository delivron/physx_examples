#include "PhysicalObject.h"

// ����������� ����������� �������.
PhysicalObject::PhysicalObject()
{
	_displayObject = nullptr;
	_actor = nullptr;
}

// ������ ����������� �������������.
void PhysicalObject::setDisplayObject(DisplayObject *displayObject)
{
	_displayObject = displayObject;
}

// ������ ��������� �� ������ ������.
void PhysicalObject::setActor(PxRigidActor *actor)
{
	_actor = actor;
}

// �������� �� �����.
void PhysicalObject::addToScene(PxScene *scene)
{
	if (scene != nullptr && _actor != nullptr)
		scene->addActor(*_actor);
}

// ������� �� �����.
void PhysicalObject::removeFromScene()
{
	if (_actor != nullptr)
	{
		PxScene *scene = _actor->getScene();

		if (scene != nullptr)
			scene->removeActor(*_actor);
	}
}

// �������� ����������� �������������.
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

// ������ �������.
void PhysicalObject::setPosition(PxVec3 coord)
{
	if (_actor != nullptr)
		_actor->setGlobalPose(PxTransform(coord));
}

// ������ ����������.
void PhysicalObject::setOrientation(PxQuat &orient)
{
	if (_actor != nullptr)
		_actor->setGlobalPose(
			PxTransform(_actor->getGlobalPose().p, orient)
		);
}