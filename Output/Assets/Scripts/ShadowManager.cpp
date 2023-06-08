#include "ShadowManager.h"
HELLO_ENGINE_API_C ShadowManager* CreateShadowManager(ScriptToInspectorInterface* script)
{
	ShadowManager* classInstance = new ShadowManager();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void ShadowManager::Start()
{
	directionalLight = gameObject.GetDirectionalLight();
}

void ShadowManager::Update()
{

}

void ShadowManager::SetShadowsHigh()
{
	directionalLight.SetFrustumOrthographicSize(139, 113);
}

void ShadowManager::SetShadowsLow()
{
	directionalLight.SetFrustumOrthographicSize(70, 70);
}
