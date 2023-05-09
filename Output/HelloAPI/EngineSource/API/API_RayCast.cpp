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
	gameObjectsInRaycast.clear();
	btCollisionWorld::AllHitsRayResultCallback btRaycast = raycast.btRaycast;
	if (btRaycast.hasHit())
	{

		btAlignedObjectArray<const btCollisionObject*> objectsArray = btRaycast.m_collisionObjects;
		for (int i = 0; i < objectsArray.size(); i++)
		{
			
			const btCollisionObject* btObj = objectsArray.at(i);

			PhysBody3D* pBody = (PhysBody3D*)btObj->getUserPointer();
		
			if (pBody)
			{
				if (ModuleLayers::gameObjects.count(pBody->gameObjectUID) != 0 && ModuleLayers::gameObjects[pBody->gameObjectUID] != nullptr)
				{
					GameObject* object = ModuleLayers::gameObjects[pBody->gameObjectUID];
					API_GameObject apiGameObject;
					apiGameObject.SetGameObject(object);
					gameObjectsInRaycast.push_back(apiGameObject);
				}
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
	gameObjectsInRaycast.clear();
}

void API::API_RayCast::GetObjectsInRaycast(API_GameObject* buffer)
{
	for (int i = 0; i < gameObjectsInRaycast.size(); i++) 
	{
		*buffer = gameObjectsInRaycast[i];
		if (i < gameObjectsInRaycast.size() - 1)
		{
			++buffer;
		}
	}

	return;
}

uint API::API_RayCast::GetObjectsSize()
{
	return gameObjectsInRaycast.size();
}

