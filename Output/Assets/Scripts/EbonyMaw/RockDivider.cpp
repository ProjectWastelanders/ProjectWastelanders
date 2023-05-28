#include "RockDivider.h"
#include "../Player/PlayerStats.h"
#include "BossLoop.h"
#include "BossAttacks.h"
#include <cmath>
//Pau Olmos

HELLO_ENGINE_API_C RockDivider* CreateRockDivider(ScriptToInspectorInterface* script)
{
	RockDivider* classInstance = new RockDivider();
	script->AddDragBoxGameObject("Boss", &classInstance->boss);
	script->AddDragInt("Num Rock", &classInstance->whichRockAmI);
	script->AddDragBoxMeshResource("Stone Mesh", &classInstance->stoneMesh);

	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void RockDivider::Start()
{
	bAttacks = (BossAttacks*)boss.GetScript("BossAttacks");
	//gameObject.CreateRigidBodyBox((0, 0, 0), (0, 0, 0), (0.6f, 0.6f, 0.6f), false); 
	for (size_t i = 0; i < 8; i++)
	{
		API_GameObject stone = Game::CreateGameObject("Stone", "Stone");
		stone.AddMeshRenderer().ChangeMesh(stoneMesh);
		stone.AddMaterial();
		stone.CreateRigidBodyBox((0, 0, 0), (0, 0, 0), (0.3f, 0.3f, 0.3f), false);
		stone.AddScript("Stone");
		stone.GetRigidBody().SetVelocity({ 0,0,0 });
		if(whichRockAmI < 5) stone.SetActive(true);
		else  stone.SetActive(false);
		
		stones.push_back(stone);
	}
}
void RockDivider::Update()
{

	if (rockDivided == true && whichRockAmI < 5 && bAttacks->bossState != BossAttacks::BOSS_STATE::RESIZINGROCK) {
		dt = Time::GetDeltaTime();
		stoneTime += dt;
		bAttacks->ReturnRock(&gameObject, whichRockAmI, false);
		gameObject.GetTransform().SetRotation(0, 0, 0);
		stones[0].GetTransform().Translate(0, 0, 0.1f * dt * 100);
		stones[1].GetTransform().Translate(0.1f * dt * 100, 0, 0.1f * dt * 100);
		stones[2].GetTransform().Translate(0.1f * dt * 100, 0, 0);
		stones[3].GetTransform().Translate(0.1f * dt * 100, 0, -0.1f * dt * 100);
		stones[4].GetTransform().Translate(0, 0, -0.1f * dt * 100);
		stones[5].GetTransform().Translate(-0.1f * dt * 100, 0, -0.1f * dt * 100);
		stones[6].GetTransform().Translate(-0.1f * dt * 100, 0, 0);
		stones[7].GetTransform().Translate(-0.1f * dt * 100, 0, 0.1f * dt * 100);
		if (stoneTime > bAttacks->stoneLifeTime) {
			stoneTime = 0;
			rockDivided = false;
		}
		
	}
	else {
		//gameObject.SetActive(true);
		for (int i = 0; i < 8; i++) {
			stones[i].GetTransform().SetPosition(gameObject.GetTransform().GetLocalPosition());
		}
	}
}

void RockDivider::OnCollisionEnter(API::API_RigidBody other)
{
	std::string detectionName = other.GetGameObject().GetName();
	std::string detectionTag = other.GetGameObject().GetTag();

	if (detectionName == "Player") {
		PlayerStats* pStats = (PlayerStats*)other.GetGameObject().GetScript("PlayerStats");
		if (whichRockAmI < 16 && bAttacks->bossState != BossAttacks::BOSS_STATE::SEEKING) {	
			pStats->TakeDamage(bAttacks->rockDmg, 0);
			bAttacks->ReturnRock(&gameObject, whichRockAmI, false);
		}
		else if (whichRockAmI > 15){
			pStats->TakeDamage(bAttacks->orbitingRockDmg, 0);
		}
	}
	else if (detectionTag == "Wall" && whichRockAmI < 5 && bAttacks->bossState != BossAttacks::BOSS_STATE::SEEKING && bAttacks->bossState != BossAttacks::BOSS_STATE::FIREROCKATTACK) {
		rockDivided = true;
	}

}