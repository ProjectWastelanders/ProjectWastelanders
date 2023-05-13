#pragma once

#include "Globals.h"
#include "API/API_Vector3.h"
#include "API/API_Vector2.h"
#include "API/API_GameObject.h"

namespace API
{
	class TO_API API_RayCastCollision
	{
	public:
		API_RayCastCollision();
		~API_RayCastCollision();

		API::API_GameObject GetGameObject();
		float GetFraction();
		float GetDistance();
		API::API_Vector3 GetPoint();
		API::API_Vector3 GetNormal();

	private:
		API_GameObject colGameObject;
		float colFraction;
		float colDistance;
		API_Vector3 colPoint;
		API_Vector3 colNormal;

		friend class API_RayCast;
	};
}
