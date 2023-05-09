#pragma once

#include "Globals.h"
#include "API/API_Vector3.h"
#include "API/API_Vector2.h"
#include <vector>
#include "API/API_GameObject.h"
class RayCast;

using namespace std;

namespace API
{
	class TO_API API_RayCast
	{
	public:
		API_RayCast();
		API_RayCast(RayCast raycast);
		~API_RayCast();

		void GetObjectsInRaycast(API_GameObject* buffer);
		uint GetObjectsSize();

	private:
		vector<API_GameObject> gameObjectsInRaycast;
	};

	
}
