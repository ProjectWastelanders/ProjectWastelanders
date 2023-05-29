#include "TpBossZone.h"
#include "../EbonyMaw/BossLoop.h"

HELLO_ENGINE_API_C TpBossZone* CreateTpBossZone(ScriptToInspectorInterface* script)
{
	TpBossZone* classInstance = new TpBossZone();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragBoxGameObject("Boss GO", &classInstance->bossGO);
	script->AddDragBoxGameObject("Collider1",&classInstance->colliderEntry);
	script->AddDragBoxGameObject("Collider2", &classInstance->colliderExit);
	return classInstance;
}

void TpBossZone::Start()
{

	bossloopref = (BossLoop*)bossGO.GetScript("BossLoop");
	if (bossloopref == nullptr) Console::Log("Boss GO missing in TPBOSSZONE Script.");
	colliderExit.SetActive(true);
}
void TpBossZone::Update()
{

	if (bossloopref->battle)
	{
		Console::Log("COLLIDER TRUE");
		colliderEntry.SetActive(true);
	}

	if (bossloopref->endBattle)
	{
		colliderEntry.SetActive(false);
		colliderExit.SetActive(false);
	}
	
	

	
}