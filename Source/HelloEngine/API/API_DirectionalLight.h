#pragma once
#include "Globals.h"
#include "API_GameObject.h"

class DirectionalLightComponent;

namespace API
{
	class TO_API API_DirectionalLight
	{
	public:
		API_DirectionalLight();
		~API_DirectionalLight();

		API_GameObject GetGameObject();

		void SetActive(bool active);

		void SetFrustumOrthographicSize(int width = 70, int height = 70);

	private:
		DirectionalLightComponent* GetComponent();
		void SetComponent(DirectionalLightComponent* component);

	private:
		DirectionalLightComponent* _directionalLight = nullptr;

		friend class API_GameObject;
	};
}
