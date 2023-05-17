#pragma once

#include "Globals.h"
#include "API/API_GameObject.h"
#include "API/API_Vector3.h"
#include "API/API_Vector2.h"
#include "API/API_RayCast.h"

class PhysicsComponent;
class ScriptComponent;

namespace API
{
	class TO_API API_RigidBody
	{
	public:
		API_RigidBody();
		~API_RigidBody();

		API_GameObject GetGameObject();

		void SetVelocity(API_Vector3 vec);
		API_Vector3 GetVelocity();

		void SetGravity(API_Vector3 gravity);
		API_Vector3 GetGravity();

		void SetTrigger(bool isTrigger);

		void SetBoxScale(API_Vector3 scale);
		API_Vector3 GetBoxScale();

		/// <summary>
		/// If the rigidbody is not a Sphere, there will return 0
		/// </summary>
		/// <returns></returns>
		void SetRadius(float radius);
		float GetRadius();

		void SetCylinderScale(float radius, float height);
		API_Vector2 GetCylinderScale();

		//Put global parameters
		API_RayCast RayCastLineGlobal(float length, API_Vector3 origin, API_Vector3 direction);

		//Put local parameters
		API_RayCast RayCastLineLocal(float length, API_Vector3 origin, API_Vector3 direction);

	private:
		PhysicsComponent* GetComponent();
		void SetComponent(PhysicsComponent* component);
	private:
		PhysicsComponent* _rigidBody = nullptr;

		friend class API_GameObject;
		friend class ScriptComponent;
		friend class DragBoxRigidBody;
	};
}
