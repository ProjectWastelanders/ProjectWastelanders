#pragma once
#include "Globals.h"
#include "API_GameObject.h"

class ComponentAgent;

namespace API
{
	class TO_API API_Agent
	{
	public:
		API_Agent();
		~API_Agent();

		API_GameObject GetGameObject();

		// Move to the position that recived	
		bool SetDestination(API_Vector3 des);

		// Move to the last position which recived
		void Move();

		// Stop moving
		void Stop();

		// Change properties
		
		void SetSpeed(float speed);

		float GetSpeed();
		
	private:
		ComponentAgent* GetComponent();
		void SetComponent(ComponentAgent* component);

	private:
		ComponentAgent* _componentAgent = nullptr;

		friend class API_GameObject;
		friend class DragBoxNavMeshAgent;
	};
}