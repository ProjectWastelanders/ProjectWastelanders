#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"
#include "../EnemyGun.h"
#include "API/API.h"
#include "../EnemyRanger.h"
#include "../../Missions/LVL3/ActiveShotsDiviner.h"
class EnemyBurst : public EnemyGun
{
public:
	void Start() override; 
	void Update() override;
	void Shoot() override;

	float fullShotCooldown;
	float shotCooldown;
	bool canShoot = true;
	float fullBurstDelay;
	float burstDelay;
	int burstLenght;
	int shotCount;
	
	ActiveShotsDiviner* activeShotsScript = nullptr;
	EnemyRanger* rangerScript = nullptr;
};

