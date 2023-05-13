#pragma once

#include "Globals.h"
#include "API/API_Vector3.h"
#include "API/API_Vector2.h"
#include <vector>
#include "API/API_GameObject.h"
#include "API/API_RayCastCollision.h"
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

		void GetCollisionsInRaycast(API_RayCastCollision* buffer);
		API_RayCastCollision GetClosestCollision();
		uint GetCollisionsSize();

	private:
	
		vector<API_RayCastCollision> collisionsInRaycast;

	};
}
