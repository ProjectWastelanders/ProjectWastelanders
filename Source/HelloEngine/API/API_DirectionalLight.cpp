#include "Headers.h"
#include "ModuleLayers.h"
#include "API_DirectionalLight.h"
#include "DirectionalLightComponent.h"


API::API_DirectionalLight::API_DirectionalLight()
{

}

API::API_DirectionalLight::~API_DirectionalLight()
{

}

API::API_GameObject API::API_DirectionalLight::GetGameObject()
{
	if (!_directionalLight)
	{
		Engine::Console::S_Log("Trying to acces a NULLPTR DirectionalLight. GetGameObject()");
		return *ModuleLayers::emptyAPIGameObject;
	}
}

void API::API_DirectionalLight::SetFrustumOrthographicSize(int width, int height)
{
	if (!_directionalLight)
	{
		Engine::Console::S_Log("Trying to acces a NULLPTR DirectionalLight. SetFrustumOrthographicSize()");
		return;
	}

	_directionalLight->data.lightFrustum.orthographicWidth = width;
	_directionalLight->data.lightFrustum.orthographicHeight = height;
	_directionalLight->UpdateToLightMap();
}

void API::API_DirectionalLight::SetActive(bool active)
{
	if (!_directionalLight)
	{
		Engine::Console::S_Log("Trying to access a NULLPTR DirectionalLight. SetActive()");
		return;
	}

	if (active)
	{
		_directionalLight->Enable();
	}
	else
	{
		_directionalLight->Disable();
	}
}

DirectionalLightComponent* API::API_DirectionalLight::GetComponent()
{
	return _directionalLight;
}

void API::API_DirectionalLight::SetComponent(DirectionalLightComponent* component)
{
	_directionalLight = component;
}