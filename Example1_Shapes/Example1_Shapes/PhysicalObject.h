#pragma once
#include "PxPhysicsAPI.h"
#include "DisplayObject.h"

using namespace physx;

class PhysicalObject
{
public:
	PhysicalObject();
	void setDisplayObject(DisplayObject *displayObject);
	void setActor(PxRigidActor *actor);
	void addToScene(PxScene *scene);
	void removeFromScene();
	void updateDisplayObject(PxMat44 *matrix = nullptr);
	PxVec3 getPosition() const;
	void setPosition(PxVec3 coord);
	void setOrientation(PxQuat &orient);

protected:
	DisplayObject *_displayObject;
	PxRigidActor *_actor;
};