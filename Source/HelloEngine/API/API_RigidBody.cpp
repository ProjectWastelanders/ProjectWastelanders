#include "Headers.h"
#include "API_RigidBody.h"
#include "ModuleLayers.h"
#include "PhysicsComponent.h"
#include "API_GameObject.h"

API::API_RigidBody::API_RigidBody()
{
}

API::API_RigidBody::~API_RigidBody()
{
}

API::API_GameObject API::API_RigidBody::GetGameObject()
{
	if (!_rigidBody)
	{
		Engine::Console::S_Log("Trying to acces a NULLPTR Rigidbody");
		return *ModuleLayers::emptyAPIGameObject;
	}
	API_GameObject returnGO;
	returnGO.SetGameObject(_rigidBody->GetGameObject());
	return returnGO;
}

void API::API_RigidBody::SetVelocity(API_Vector3 vec)
{
	if (!_rigidBody)
	{
		Engine::Console::S_Log("Trying to acces a NULLPTR Rigidbody");
		return;
	}	

	_rigidBody->SetVelocity(vec);
}

API::API_Vector3 API::API_RigidBody::GetVelocity()
{
	if (!_rigidBody)
	{
		Engine::Console::S_Log("Trying to get a NULLPTR Rigidbody");
		return API_Vector3::S_Zero();
	}
	return _rigidBody->GetVelocity();
}

void API::API_RigidBody::SetGravity(API_Vector3 gravity)
{
	if (!_rigidBody)
	{
		Engine::Console::S_Log("Trying to acces a NULLPTR Rigidbody");
		return;
	}

	_rigidBody->SetGravity(gravity);
}

API::API_Vector3 API::API_RigidBody::GetGravity()
{
	if (!_rigidBody)
	{
		Engine::Console::S_Log("Trying to get a NULLPTR Rigidbody");
		return API_Vector3::S_Zero();
	}
	return _rigidBody->GetGravity();
}

void API::API_RigidBody::SetTrigger(bool isTrigger)
{
	if (!_rigidBody)
	{
		Engine::Console::S_Log("Trying to get a NULLPTR Rigidbody");
		return;
	}
	_rigidBody->_physBody->isTrigger = isTrigger;
	_rigidBody->CallUpdateAllPram();
}

void API::API_RigidBody::SetBoxScale(API_Vector3 scale)
{
	if (!_rigidBody)
	{
		Engine::Console::S_Log("Trying to get a NULLPTR Rigidbody");
		return;
	}

	if (_rigidBody->_shapeSelected == ColliderShape::BOX)
	{
		_rigidBody->_physBody->colScl = scale;
		ModulePhysics::UpdatePhysBodyScaleBox(_rigidBody->_physBody);
	}
}

API::API_Vector3 API::API_RigidBody::GetBoxScale()
{
	if (!_rigidBody)
	{
		Engine::Console::S_Log("Trying to get a NULLPTR Rigidbody");
		return API_Vector3();
	}

	API_Vector3 scale;

	if (_rigidBody->_shapeSelected == ColliderShape::BOX)
	{ 
		scale = _rigidBody->_physBody->colScl;
	}
	else
	{
		scale = API_Vector3();
	}

	return scale;
}

void API::API_RigidBody::SetRadius(float rad)
{
	if (!_rigidBody)
	{
		Engine::Console::S_Log("Trying to get a NULLPTR Rigidbody");
		return;
	}

	if (_rigidBody->_shapeSelected == ColliderShape::SPHERE)
	{
		_rigidBody->sphereRadius = rad;
		ModulePhysics::UpdatePhysBodyScaleSphere(_rigidBody->_physBody, rad);
	}
}

float API::API_RigidBody::GetRadius()
{
	if (!_rigidBody)
	{
		Engine::Console::S_Log("Trying to get a NULLPTR Rigidbody");
		return 0;
	}

	float ret = _rigidBody->GetRadius();

	if (ret <= 0) 
	{
		Engine::Console::S_Log("Trying to get a radius a non sphere rigidbody!!!");
	}

	return ret;
}

void API::API_RigidBody::SetCylinderScale(float radius, float height)
{
	if (!_rigidBody)
	{
		Engine::Console::S_Log("Trying to get a NULLPTR Rigidbody");
		return;
	}

	if (_rigidBody->_shapeSelected == ColliderShape::CYLINDER)
	{
		_rigidBody->cylRadiusHeight.x = radius;
		_rigidBody->cylRadiusHeight.y = height;
		ModulePhysics::UpdatePhysBodyScaleCylinder(_rigidBody->_physBody, radius, height);
	}
}

API::API_Vector2 API::API_RigidBody::GetCylinderScale()
{
	if (!_rigidBody)
	{
		Engine::Console::S_Log("Trying to get a NULLPTR Rigidbody");
		return API_Vector2(0,0);
	}

	API_Vector2 radiusHeight;

	radiusHeight.x = _rigidBody->cylRadiusHeight.x;
	radiusHeight.y = _rigidBody->cylRadiusHeight.y;

	return radiusHeight;
}

PhysicsComponent* API::API_RigidBody::GetComponent()
{
	if (!_rigidBody) 
		Engine::Console::S_Log("Trying to get a NULLPTR Rigidbody");
	
    return _rigidBody;
}

void API::API_RigidBody::SetComponent(PhysicsComponent* component)
{
	_rigidBody = component;
}
