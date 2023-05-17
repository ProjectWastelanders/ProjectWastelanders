#include "Headers.h"
#include "API_RayCast.h"
#include "ModulePhysics.h"
#include "PhysBody3D.h"
#include "ModuleLayers.h"

API::API_RayCast::API_RayCast()
{
}

API::API_RayCast::API_RayCast(RayCast raycast)
{
	collisionsInRaycast.clear();
	btCollisionWorld::AllHitsRayResultCallback btRaycast = raycast.btRaycast;
	if (btRaycast.hasHit())
	{
		//get gameObject
		btAlignedObjectArray<const btCollisionObject*> objectsArray = btRaycast.m_collisionObjects;
		for (int i = 0; i < objectsArray.size(); i++)
		{
			API_RayCastCollision raycastCol;

			const btCollisionObject* btObj = objectsArray.at(i);

			PhysBody3D* pBody = (PhysBody3D*)btObj->getUserPointer();
		
			if (pBody)
			{
				if (ModuleLayers::gameObjects.count(pBody->gameObjectUID) != 0 && ModuleLayers::gameObjects[pBody->gameObjectUID] != nullptr)
				{
					GameObject* object = ModuleLayers::gameObjects[pBody->gameObjectUID];
					API_GameObject apiGameObject;
					apiGameObject.SetGameObject(object);
					raycastCol.colGameObject = apiGameObject;
				}
			}
			collisionsInRaycast.push_back(raycastCol);
		}
		//get fraction and distance
		btAlignedObjectArray<btScalar> fractionArray = btRaycast.m_hitFractions;
		for (int i = 0; i < fractionArray.size(); i++)
		{
			if (i < collisionsInRaycast.size())
			{
				float fraction = (float)fractionArray.at(i);
				collisionsInRaycast.at(i).colFraction = fraction;
				collisionsInRaycast.at(i).colDistance = fraction * raycast.btRaycast.m_rayFromWorld.distance(raycast.btRaycast.m_rayToWorld);
			}
		}
		//get point
		btAlignedObjectArray<btVector3> pointArray = btRaycast.m_hitPointWorld;
		for (int i = 0; i < pointArray.size(); i++)
		{
			if (i < collisionsInRaycast.size())
			{
				collisionsInRaycast.at(i).colPoint = (float3)pointArray.at(i);
			}
		}
		//get normal
		btAlignedObjectArray<btVector3> normalArray = btRaycast.m_hitNormalWorld;
		for (int i = 0; i < normalArray.size(); i++)
		{
			if (i < collisionsInRaycast.size())
			{
				collisionsInRaycast.at(i).colNormal = (float3)normalArray.at(i);
			}
		}
	}
	else
	{
		return;
	}
}

API::API_RayCast::~API_RayCast()
{
	collisionsInRaycast.clear();
}

void API::API_RayCast::GetCollisionsInRaycast(API_RayCastCollision* buffer)
{
	for (int i = 0; i < collisionsInRaycast.size(); i++)
	{
		*buffer = collisionsInRaycast[i];
		if (i < collisionsInRaycast.size() - 1)
		{
			++buffer;
		}
	}

	return;
}

API::API_RayCastCollision API::API_RayCast::GetClosestCollision()
{
	if (collisionsInRaycast.size() > 0) 
	{
		API_RayCastCollision closest = collisionsInRaycast.at(0);
		for (int i = 1; i < collisionsInRaycast.size(); i++)
		{
			if (collisionsInRaycast.at(i).colFraction < collisionsInRaycast.at(i - 1).colFraction)
			{
				closest = collisionsInRaycast.at(i);
			}
		}
		return closest;
	}
	else 
	{
		return API_RayCastCollision();
	}
	
}

uint API::API_RayCast::GetCollisionsSize()
{
	return collisionsInRaycast.size();
}

