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

API::API_RayCast API::API_RigidBody::RayCastLineGlobal(float length, API_Vector3 origin, API_Vector3 dir)
{
	if (!_rigidBody)
	{
		Engine::Console::S_Log("Trying to get a NULLPTR Rigidbody");
		return API_RayCast();
	}

	float dirNorm = sqrt(pow(dir.x, 2) + pow(dir.y, 2) + pow(dir.z, 2));
	API_Vector3 normDir = { dir.x / dirNorm, dir.y / dirNorm, dir.z / dirNorm };

	API_Vector3 lengthDir = normDir * length;
	API_Vector3 finalPoint = origin + lengthDir;

	_rigidBody->_physBody->raycastPos1 = origin;
	_rigidBody->_physBody->raycastPos2 = finalPoint;
	
	//Do the raycast
	btCollisionWorld::AllHitsRayResultCallback btRaycast = ModulePhysics::RayCastLine((float3)origin, (float3)finalPoint);
	RayCast newRaycast = RayCast(btRaycast);
	API_RayCast raycast(btRaycast);

	return raycast;
}

API::API_RayCast API::API_RigidBody::RayCastLineLocal(float length, API_Vector3 localOrigin, API_Vector3 localDir)
{
	if (!_rigidBody)
	{
		Engine::Console::S_Log("Trying to get a NULLPTR Rigidbody");
		return API_RayCast();
	}

	//Get global body transform
	float globalMatFloat[16] = { 0.0f };
	_rigidBody->_physBody->body->getWorldTransform().getOpenGLMatrix(globalMatFloat);
	
	float4x4 globalMat4x4 = float4x4::identity;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			globalMat4x4[i][j] = globalMatFloat[i*4 + j];
		}
	}
	globalMat4x4.Transpose();

	//Calculate the global origin

	float4 localOri4x1 = { localOrigin.x, localOrigin.y, localOrigin.z, 1 };
	float4 globalOri4x1 = globalMat4x4 * localOri4x1;
	API_Vector3 globalOrigin = { globalOri4x1.x, globalOri4x1.y, globalOri4x1.z };

	//Calculate the global direction

	float dirNorm = sqrt(pow(localDir.x, 2) + pow(localDir.y, 2) + pow(localDir.z, 2));
	API_Vector3 normDir = { localDir.x / dirNorm, localDir.y / dirNorm, localDir.z / dirNorm };
	API_Vector3 lengthDir = normDir * length;

	float4 localDir4x1 = { localOrigin.x + lengthDir.x, localOrigin.y + lengthDir.y, localOrigin.z + lengthDir.z, 1 };
	float4 globalDir4x1 = globalMat4x4 * localDir4x1;
	API_Vector3 globalDir = { globalDir4x1.x, globalDir4x1.y, globalDir4x1.z };

	_rigidBody->_physBody->raycastPos1 = globalOrigin;
	_rigidBody->_physBody->raycastPos2 = globalDir;

	//Do the raycast
	btCollisionWorld::AllHitsRayResultCallback btRaycast = ModulePhysics::RayCastLine((float3)globalOrigin, (float3)globalDir);
	RayCast newRaycast = RayCast(btRaycast);

	API_RayCast raycast(newRaycast);

	return raycast;
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
