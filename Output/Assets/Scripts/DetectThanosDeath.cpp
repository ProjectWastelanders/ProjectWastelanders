#include "DetectThanosDeath.h"
#include "Thanos/ThanosAttacks.h"
HELLO_ENGINE_API_C DetectThanosDeath* CreateDetectThanosDeath(ScriptToInspectorInterface* script)
{
	DetectThanosDeath* classInstance = new DetectThanosDeath();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void DetectThanosDeath::Start()
{
	thanos = (ThanosAttacks*)Game::FindGameObject("Thanos").GetScript("ThanosAttacks");
}
void DetectThanosDeath::Update()
{
	if (thanos && thanos->isDead && thanos->deathTimer >= 2.0f)
	{
		Scene::LoadScene("FinalCinematics.HScene");
	}
}