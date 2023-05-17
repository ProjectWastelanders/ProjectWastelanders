#include "BossAttacks.h"
#include "../EbonyMaw/RockDivider.h"
#include "../Player/PlayerStats.h"
#include "../EbonyMaw/BossLoop.h"
#include "../Player/PlayerMove.h"
#include <time.h>
#include <random>
#include <math.h>
//Pau Olmos

HELLO_ENGINE_API_C BossAttacks* CreateBossAttacks(ScriptToInspectorInterface* script)
{
	BossAttacks* classInstance = new BossAttacks();
	script->AddDragInt("AttackType", &classInstance->attackType);
	script->AddDragInt("Random", &classInstance->difficultySetter);
	script->AddDragFloat("specialAttackCooldown", &classInstance->specialAttackCooldown);

	script->AddDragBoxParticleSystem("Fire From Rock", &classInstance->groundFire);

	script->AddCheckBox("Attacking", &classInstance->attacking);
	script->AddDragBoxGameObject("Boss", &classInstance->boss);
	script->AddDragBoxGameObject("Player", &classInstance->player);
	script->AddDragBoxGameObject("OrbitingRocks", &classInstance->orbitingRocks);
	script->AddDragBoxGameObject("explosionWave1", &classInstance->explosionWave1);
	script->AddDragFloat("Distance", &classInstance->distSA);
	script->AddDragFloat("Cooldown", &classInstance->cooldownAttack);
	script->AddDragFloat("RotationSpeed", &classInstance->rotationSpeed);
	script->AddDragFloat("Radius", &classInstance->radius);

	script->AddDragBoxGameObject("Rock1", &classInstance->rocks[0]);
	script->AddDragBoxGameObject("Rock2", &classInstance->rocks[1]);
	script->AddDragBoxGameObject("Rock3", &classInstance->rocks[2]);
	script->AddDragBoxGameObject("Rock4", &classInstance->rocks[3]);
	script->AddDragBoxGameObject("Rock5", &classInstance->rocks[4]);
	script->AddDragBoxGameObject("Rock6", &classInstance->rocks[5]);
	script->AddDragBoxGameObject("Rock7", &classInstance->rocks[6]);
	script->AddDragBoxGameObject("Rock8", &classInstance->rocks[7]);
	script->AddDragBoxGameObject("Rock9", &classInstance->rocks[8]);
	script->AddDragBoxGameObject("Rock10", &classInstance->rocks[9]);
	script->AddDragBoxGameObject("Rock11", &classInstance->rocks[10]);
	script->AddDragBoxGameObject("Rock12", &classInstance->rocks[11]);
	script->AddDragBoxGameObject("Rock13", &classInstance->rocks[12]);
	script->AddDragBoxGameObject("Rock14", &classInstance->rocks[13]);
	script->AddDragBoxGameObject("Rock15", &classInstance->rocks[14]);
	script->AddDragBoxGameObject("Rock16", &classInstance->rocks[15]);
	script->AddDragBoxGameObject("Rock17", &classInstance->rocks[16]);
	script->AddDragBoxGameObject("Rock18", &classInstance->rocks[17]);
	script->AddDragBoxGameObject("Rock19", &classInstance->rocks[18]);
	script->AddDragBoxGameObject("Rock20", &classInstance->rocks[19]);

	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void BossAttacks::Start()
{
	for (int i = 0; i < 15; i++) {
		rockPositions[i] = rocks[i].GetTransform().GetGlobalPosition();
		rocks[i].GetRigidBody().SetVelocity({ 0, 0, 0 });
	}
	
	srand(time(NULL));
	bLoop = (BossLoop*)boss.GetScript("BossLoop");
	pStats = (PlayerStats*)player.GetScript("PlayerStats");
	pMove = (PlayerMove*)player.GetScript("PlayerMove");
	if (bLoop == nullptr) {
		Console::Log("BossLoop is nullptr in BOSSATTACK script");
	}
	takeobjTimer = 0;
	explosionWave1.GetTransform().Translate(0, -10, 0);
	orbitingRocks.SetActive(false);
}

void BossAttacks::Update()
{
	distSA = player.GetTransform().GetGlobalPosition().Distance(gameObject.GetTransform().GetGlobalPosition());
	dt = Time::GetDeltaTime();
	if (isFireOn == true) {
		fireTime += Time::GetDeltaTime();
		if (fireTime > 10.0f) {
			isFireOn = false;
			fireTime = 0.0f;
			groundFire.StopEmitting();
			groundFire.GetGameObject().SetActive(false);
		}
	}

	if (distSA < 80.0f && bLoop->battle) {
		if (bLoop->phase == 2) {
			specialAttackCooldown += Time::GetDeltaTime();
		}

		if (bLoop && bLoop->weakTime > 0) {
			bossState = BOSS_STATE::KO;
			for (int i = 0; i < numRocks[attackType]; i++) {
				ReturnRock(&rocks[currentRock[i]], i, true);
			}
		}
		else if (bossState == BOSS_STATE::KO) {
			bossState = BOSS_STATE::IDLE;
			attacking = false;
		}
		if (bLoop->canTakeDamage == false) {
			switch (bLoop->phase)
			{
			case 0:
				rocks[16].SetActive(false);
				rocks[17].SetActive(false);
				rocks[18].SetActive(false);
				rocks[19].SetActive(false);
				break;
			case 1:
				rocks[16].SetActive(true);
				rocks[17].SetActive(true);
				rocks[18].SetActive(true);
				rocks[19].SetActive(true);
				if (bossState != BOSS_STATE::KO) OrbitingRocks(&rocks[19], &rocks[18], &rocks[17], &rocks[16], rotationSpeed, radius);
				//rotationBoss = boss.GetTransform().GetGlobalRotation().y;

				break;
			case 2:
				rocks[16].SetActive(true);
				rocks[17].SetActive(true);
				rocks[18].SetActive(true);
				rocks[19].SetActive(true);
				if (bossState != BOSS_STATE::KO) OrbitingRocks(&rocks[19], &rocks[18], &rocks[17], &rocks[16], rotationSpeed * 1.5, radius * 1.25);
			default:
				break;
			}

		}

		if (attacking) {
			switch (attackType)
			{
			case 0:
			case 1:
			case 2:
				speed = 1.0f;
				switch (bossState)
				{
				case BossAttacks::BOSS_STATE::ROCKSELECT: SelectRock();
					break;
				case BossAttacks::BOSS_STATE::SEEKING:
					bossPosition[0] = { gameObject.GetTransform().GetGlobalPosition().x, gameObject.GetTransform().GetGlobalPosition().y + 10.0f,gameObject.GetTransform().GetGlobalPosition().z - 4.0f };
					bossPosition[1] = { gameObject.GetTransform().GetGlobalPosition().x + 4.0f,gameObject.GetTransform().GetGlobalPosition().y + 8.0f,gameObject.GetTransform().GetGlobalPosition().z + 4.0f };
					bossPosition[2] = { gameObject.GetTransform().GetGlobalPosition().x - 4.0f,gameObject.GetTransform().GetGlobalPosition().y + 8.0f,gameObject.GetTransform().GetGlobalPosition().z + 4.0f };
					bossPosition[3] = { gameObject.GetTransform().GetGlobalPosition().x + 6.0f,gameObject.GetTransform().GetGlobalPosition().y + 13.0f,gameObject.GetTransform().GetGlobalPosition().z };
					bossPosition[4] = { gameObject.GetTransform().GetGlobalPosition().x - 6.0f,gameObject.GetTransform().GetGlobalPosition().y + 13.0f,gameObject.GetTransform().GetGlobalPosition().z };

					for (int i = 0; i < numRocks[attackType]; i++) {
						Seek(&rocks[currentRock[i]], bossPosition[i], speed / 3, i, false, 60.0f);
					}			

					//if (takeobjTimer <= 6) {
						if (bLoop->animState != BossLoop::AnimationState::TAKEOBJ && bLoop->animState != BossLoop::AnimationState::THROWOBJ)
						{
							bLoop->animState = BossLoop::AnimationState::TAKEOBJ;
							bLoop->animationPlayer.ChangeAnimation(bLoop->recoverAnim);
							bLoop->animationPlayer.SetLoop(true);
							bLoop->animationPlayer.Play();
						}/*
						takeobjTimer = 0;
					}
					else {
						takeobjTimer += dt;
					}*/

					break;
				case BossAttacks::BOSS_STATE::HOLDING:

					if (bLoop->animState != BossLoop::AnimationState::IDLE2 && bLoop->animState != BossLoop::AnimationState::TAKEOBJ && bLoop->animState != BossLoop::AnimationState::THROWOBJ && bLoop->animState != BossLoop::AnimationState::KNOCKUP && bLoop->animState != BossLoop::AnimationState::RECOVER)
					{
						bLoop->animState = BossLoop::AnimationState::IDLE2;
						bLoop->animationPlayer.ChangeAnimation(bLoop->idleAnim2);
						bLoop->animationPlayer.SetLoop(true);
						bLoop->animationPlayer.Play();
					}

					HoldRock();

					break;
				case BossAttacks::BOSS_STATE::THROWING:
					currentTimeAttack += Time::GetDeltaTime();

					if (!animAttack)
					{
						animAttack = true;
						bLoop->animState = BossLoop::AnimationState::THROWOBJ;
						bLoop->animationPlayer.ChangeAnimation(bLoop->throwObjAnim);
						bLoop->animationPlayer.SetLoop(true);
						bLoop->animationPlayer.Play();
					}

					for (int i = 0; i < numRocks[attackType]; i++) {
						if (currentTimeAttack <= timeAttack[i]) {
							playerPosition[i] = player.GetTransform().GetGlobalPosition();
							dir[i] = player.GetTransform().GetGlobalPosition() - rocks[currentRock[i]].GetTransform().GetGlobalPosition();
						}
						if (currentTimeAttack > timeAttack[i] && hasReachedTarget[i] == false) Seek(&rocks[currentRock[i]], playerPosition[i], speed / 2, i, false, 60.0f);
					}

					if (currentTimeAttack >= timeAttack[numRocks[attackType]]) {
						bossState = BOSS_STATE::IDLE;
						attacking = false;
						animAttack = false;
					}

					break;
				
				default:
					break;
				}
				break;
			case 3:
				currentTimeAttack += Time::GetDeltaTime();
				throwing = false;
				SpecialAttack();
				if (currentTimeAttack >= specialTimeAttack[numRocks[attackType]] && bossState == BOSS_STATE::SPECIALATTACK) {
					bossState = BOSS_STATE::IDLE;
					specialAttackCooldown = 0.0f;
					attacking = false;
				}
				break;
			case 4:
				explosionTime += Time::GetDeltaTime();
				if (explosionTime < 0.5 && distSA < 15.0 && explosionWave1HasArrived == false) {
					pStats->TakeDamage(50,0);
					explosionWave1HasArrived = true;
				}
				if (explosionTime >= 0.5 && distSA > 15.0 && distSA < 30.0 && explosionWave2HasArrived == false) {

					pStats->TakeDamage(0,0);

					API_Vector3 normalizedvector = boss.GetTransform().GetGlobalPosition() - player.GetTransform().GetGlobalPosition();
					float x = normalizedvector.x * normalizedvector.x;
					float y = 0;
					float z = normalizedvector.z * normalizedvector.z;
					float sum = x + y + z;
					API_Vector3 direction = { normalizedvector.x / sum, 0, normalizedvector.z / sum };
					pMove->RecieveImpulse(-direction, 0.25f, 50);

					//KnockBack
					explosionWave2HasArrived = true;
				}
				if (explosionTime > 0.6) {
					bLoop->exploting = false;
					explosionWave1.SetActive(false);
					explosionWave1.GetTransform().Translate(0, -10, 0);
					explosionWave1.GetParticleSystem().Stop();
					explosionWave1.GetParticleSystem().StopEmitting();
					bossState = BOSS_STATE::IDLE;
					attacking = false;
				}
				break;
			case 5: 
				switch (bossState)
				{
				case BossAttacks::BOSS_STATE::FIREROCKATTACK:

					Seek(&rocks[currentRock[1]], { boss.GetTransform().GetGlobalPosition().x,  boss.GetTransform().GetGlobalPosition().y + 12.0f, boss.GetTransform().GetGlobalPosition().z}, speed / 3, 1, false, 60.0f);
					dir[1] = player.GetTransform().GetGlobalPosition() - rocks[1].GetTransform().GetGlobalPosition();

					if (bLoop->animState != BossLoop::AnimationState::TAKEOBJ)
					{
						bLoop->animState = BossLoop::AnimationState::TAKEOBJ;
						bLoop->animationPlayer.ChangeAnimation(bLoop->takeObjAnim);
						bLoop->animationPlayer.SetLoop(true);
						bLoop->animationPlayer.Play();
					}

					break;

				case BossAttacks::BOSS_STATE::RESIZINGROCK:

					if (rocks[1].GetTransform().GetGlobalScale().x >= 6.0f) {


						Seek(&rocks[currentRock[1]], lastPlayerPosition, speed / 5, 1, false, 60.0f);

						returnFireRockTime += Time::GetDeltaTime();

						if (returnFireRockTime > 0.6f) {
							bossState = BOSS_STATE::IDLE;
							attacking = false;
							groundFire.GetGameObject().GetTransform().SetPosition(lastPlayerPosition);
							groundFire.GetGameObject().SetActive(true);
							groundFire.Play();
							isFireOn = true;
						}

					}
					else {
						rocks[1].GetTransform().Scale(10 * Time::GetDeltaTime());
						lastPlayerPosition = player.GetTransform().GetGlobalPosition();
					}

					if (bLoop->animState != BossLoop::AnimationState::SPECIAL)
					{
						bLoop->animState = BossLoop::AnimationState::SPECIAL;
						bLoop->animationPlayer.ChangeAnimation(bLoop->specialAnim);
						bLoop->animationPlayer.SetLoop(false);
						bLoop->animationPlayer.Play();
					}

					break;
				default:
					break;
				}
			default:
				break;
			}
		}
		else {
			switch (bossState)
			{
			case BOSS_STATE::IDLE:
				attackType = 0.0f;
				returnFireRockTime = 0.0f;
				explosionTime = 0.0f;
				currentTimeAttack = 0.0f;
				rocks[1].GetTransform().SetScale(1.0f, 1.0f, 1.0f);
				for (int i = 0; i < 15; i++) {
					hasReachedTarget[i] = false;
					playerPosition[i] = { 0,0,0 };
					dir[i] = { 0,0,0 };
				}
				for (int i = 0; i < 15; i++) {
					rocks[i].GetTransform().SetPosition(rockPositions[i]);
				}
				cooldownAttack += Time::GetDeltaTime();
				if (cooldownAttack > 3.0f) {
					bossState = BOSS_STATE::SELECTATTACK;
					cooldownAttack = 0;
				}

				if (bLoop->animState != BossLoop::AnimationState::IDLE && bLoop->animState != BossLoop::AnimationState::KNOCKUP && bLoop->animState != BossLoop::AnimationState::RECOVER)
				{
					bLoop->animState = BossLoop::AnimationState::IDLE;
					bLoop->animationPlayer.ChangeAnimation(bLoop->idleAnim);
					bLoop->animationPlayer.SetLoop(true);
					bLoop->animationPlayer.Play();
				}

				break;
			case BOSS_STATE::SELECTATTACK:
				// Generate a random integer between 0 and 0
				difficultySetter = rand() % 120 + 1;

				switch (bLoop->phase)
				{
				case 0:
					speed = 1.0f;
					if (difficultySetter <= 60) attackType = 0;
					else if (difficultySetter > 60 && difficultySetter <= 90) attackType = 1;
					else if (difficultySetter > 90 && difficultySetter < 101) attackType = 2;
					else if (difficultySetter > 100) attackType = 5;

					break;

				case 1:
					speed = 1.25f;
					if (difficultySetter <= 40) attackType = 0;
					else if (difficultySetter > 40 && difficultySetter <= 85) attackType = 1;
					else if (difficultySetter > 85 && difficultySetter < 101) attackType = 2;
					else if (difficultySetter > 100) attackType = 5;

					break;

				case 2:
					speed = 1.5f;
					if (difficultySetter <= 20) attackType = 0;
					else if (difficultySetter > 20 && difficultySetter <= 65) attackType = 1;
					else if (difficultySetter > 65 && difficultySetter < 101) attackType = 2;
					else if (difficultySetter > 100) attackType = 5;

					if (specialAttackCooldown >= 60.0f) {
						difficultySetter = rand() % 10 + 1;
						if (difficultySetter <= 5) {
							attackType = 3;
							specialAttackCooldown = 0.0f;
						}
					}
					break;

				default:
					break;
				}
				if (attackType < 3) {
					bossState = BOSS_STATE::ROCKSELECT;
				}
				else if (attackType == 3) {
					bossState = BOSS_STATE::SPECIALATTACK;
				}

				if (bLoop->exploting == true) {

					attackType = 4;

				}

				if (attackType == 4) {
					explosionWave1HasArrived = false;
					explosionWave2HasArrived = false;
					bossState = BOSS_STATE::EXPLOSIONWAVE;
					explosionWave1.SetActive(true);
					explosionWave1.GetTransform().Translate(0, 10, 0);
					explosionWave1.GetParticleSystem().Play();
				}

				if (attackType == 5) {
					bossState = BOSS_STATE::FIREROCKATTACK;

				}

				hasBossCoords = false;
				attacking = true;
				break;
			default:
				break;
			}
		}
	}
}

void BossAttacks::SelectRock()
{
	throwing = false;

	for (int i = 0; i < 15; i++) {
		currentRock[i] = i;
	}

	bossState = BOSS_STATE::SEEKING;

}

void BossAttacks::Seek(API_GameObject* seeker, API_Vector3 target, float speed, int rock, float endedAttacking, float time)
{
	dt = Time::GetDeltaTime();
	if (bossState == BOSS_STATE::SEEKING || bossState == BOSS_STATE::FIREROCKATTACK) {
	dir[rock] = target - seeker->GetTransform().GetGlobalPosition();
	seeker->GetTransform().Rotate(150 * dt, 300 * dt, 250 * dt);
	} 
	API_Vector3 dist = target - seeker->GetTransform().GetGlobalPosition();
	seeker->GetTransform().Translate((dir[rock] / time) * speed * dt * 400);
	if ((dist.x < 2 && dist.x > -2 && dist.y < 2 && dist.y && dist.z < 2 && dist.z)) {
		if (bossState == BOSS_STATE::FIREROCKATTACK) bossState = BOSS_STATE::RESIZINGROCK;
		else if (bossState == BOSS_STATE::RESIZINGROCK) {
			bossState = BOSS_STATE::IDLE;
			attacking = false;
			groundFire.GetGameObject().GetTransform().SetPosition(target);
			groundFire.GetGameObject().SetActive(true);
			groundFire.Play();
			isFireOn = true;
		}
	}
	if ( dist.x < 0.3 && dist.x > -0.3 && dist.y < 0.3 && dist.y && dist.z < 0.3 && dist.z) {
		if (bossState == BOSS_STATE::SEEKING) {
			bossState = BOSS_STATE::HOLDING;
		}
		else if (bossState == BOSS_STATE::THROWING || bossState == BOSS_STATE::SPECIALATTACK) {
			//if(rock == numRocks[attackType]) ReturnRock(seeker, rock, true);
			ReturnRock(seeker, rock, false);
		}
		if (currentTimeAttack >= timeAttack[numRocks[attackType]] && bossState == BOSS_STATE::THROWING) {
			bossState = BOSS_STATE::IDLE;
			attacking = false;
		}
		else if (currentTimeAttack >= specialTimeAttack[10] && bossState == BOSS_STATE::SPECIALATTACK) {
			bossState = BOSS_STATE::IDLE;
			attacking = false;
		}
		
	}
}

void BossAttacks::HoldRock()
{
	dt = Time::GetDeltaTime();
	preAttack += dt;
	for (int i = 0; i < numRocks[attackType]; i++) {
		rocks[currentRock[i]].GetTransform().Rotate(300 * dt, 600 * dt, 500 * dt);
	}
	throwing = true;
	bossState = BOSS_STATE::THROWING;
}

void BossAttacks::ReturnRock(API_GameObject* rock, int numRock, float endedAttacking)
{
	hasReachedTarget[numRock] = true;
	rock->GetTransform().SetPosition(rockPositions[currentRock[numRock]]);
	if (bossState != BOSS_STATE::KO && endedAttacking == true) {
		bossState = BOSS_STATE::IDLE;
		attacking = false;
	}
}

void BossAttacks::SpecialAttack()
{

	for (int i = 0; i < 10; i++) {
		if (currentTimeAttack <= specialTimeAttack[i]) {
			playerPosition[i+5] = player.GetTransform().GetGlobalPosition();
			dir[i+5] = player.GetTransform().GetGlobalPosition() - rocks[currentRock[i+5]].GetTransform().GetGlobalPosition();
		}
		if (currentTimeAttack > specialTimeAttack[i] && hasReachedTarget[i + 5] == false) 
		{
			Seek(&rocks[currentRock[i + 5]], playerPosition[i + 5], speed / 2, i + 5, false, 80.0f);

			if (bLoop->animState != BossLoop::AnimationState::SPECIAL2)
			{
				bLoop->animState = BossLoop::AnimationState::SPECIAL2;
				bLoop->animationPlayer.ChangeAnimation(bLoop->specialAnim2);
				bLoop->animationPlayer.Play();
			}
		}
	}
}

void BossAttacks::OrbitingRocks(API_GameObject* orbitingRock1, API_GameObject* orbitingRock2, API_GameObject* orbitingRock3, API_GameObject* orbitingRock4, float rotationSpeed, float radius)
{
	//float finalRotationBoss = boss.GetTransform().GetGlobalRotation().y - rotationBoss;

	orbitingRock1->GetTransform().SetPosition(radius, 0, 0);
	orbitingRock2->GetTransform().SetPosition(-radius, 0, 0);
	orbitingRock3->GetTransform().SetPosition(0, 0, radius);
	orbitingRock4->GetTransform().SetPosition(0, 0, -radius);
	orbitingRocks.GetTransform().Rotate(0, rotationSpeed, 0);
}
