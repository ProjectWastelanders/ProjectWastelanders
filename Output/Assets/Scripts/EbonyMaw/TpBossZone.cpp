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
	colliderEntry.GetRigidBody().SetTrigger(true);
	colliderExit.SetActive(true);

	cont = 1;
}
void TpBossZone::Update()
{
	
	if (bossloopref->battle && cont == 1)
	{
		colliderEntry.GetTransform().SetPosition(101.362f, 94.791f, -36.731f);
		colliderEntry.GetRigidBody().SetTrigger(false);
		cont = 0;
	}
	 
	if (bossloopref->endBattle && cont == 0)
	{
		colliderEntry.GetTransform().SetPosition(101.362f, 94.791f, -36.731f);
		colliderEntry.GetRigidBody().SetTrigger(true);
		colliderExit.SetActive(false);
		cont = -1;
	}
	
	

	
}