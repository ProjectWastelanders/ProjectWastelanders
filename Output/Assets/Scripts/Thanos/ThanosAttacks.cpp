#include "ThanosAttacks.h"
#include "ThanosMeleeDmg.h"
#include "ThanosLoop.h"
#include "../Player/PlayerStats.h"
#include "../Player/PlayerMove.h"

HELLO_ENGINE_API_C ThanosAttacks* CreateThanosAttacks(ScriptToInspectorInterface* script)
{
	ThanosAttacks* classInstance = new ThanosAttacks();

	script->AddCheckBox("IsAttacking?", &classInstance->isAttacking);

	script->AddDragBoxGameObject("Boss", &classInstance->boss);
	script->AddDragBoxGameObject("Player", &classInstance->player);

	script->AddDragBoxGameObject("Melee Attack 1", &classInstance->melee1); 
	script->AddDragBoxGameObject("SWORD", &classInstance->sword);
	script->AddDragBoxGameObject("DeflectProjectiles", &classInstance->defenseSword);
	script->AddDragBoxGameObject("BoomerangTarget", &classInstance->bTarget);

	script->AddDragBoxGameObject("ExplosionWave", &classInstance->explosionWave);

	script->AddDragBoxGameObject("Bullet1", &classInstance->bullet1);
	script->AddDragBoxGameObject("Bullet2", &classInstance->bullet2);
	script->AddDragBoxGameObject("Bullet3", &classInstance->bullet3);

	script->AddDragBoxGameObject("Beam1", &classInstance->beam1);
	script->AddDragBoxGameObject("Beam2", &classInstance->beam2);
	script->AddDragBoxGameObject("Beam3", &classInstance->beam3);
	script->AddDragBoxGameObject("Beam4", &classInstance->beam4);

	script->AddDragBoxGameObject("BeamTarget1", &classInstance->beamTarget1);
	script->AddDragBoxGameObject("BeamTarget2", &classInstance->beamTarget2);
	script->AddDragBoxGameObject("BeamTarget3", &classInstance->beamTarget3);
	script->AddDragBoxGameObject("BeamTarget4", &classInstance->beamTarget4);

	script->AddDragBoxGameObject("Meteor Rain", &classInstance->meteorRain);

	script->AddDragBoxGameObject("Meteor Rain", &classInstance->meteorRain);

	script->AddDragBoxGameObject("Laser Aim Position", &classInstance->laserPosition);
	script->AddDragBoxParticleSystem("Laser Particle System", &classInstance->laserPS);
	script->AddDragBoxGameObject("Laser GameObject", &classInstance->laserGO);

	for(int i = 0; i < 30; i++){
		script->AddDragBoxGameObject("Meteor", &classInstance->meteors[i]);

	}

	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void ThanosAttacks::Start()
{
	srand(time(NULL));
	tMeleeDmg = (ThanosMeleeDmg*)melee1.GetScript("ThanosMeleeDmg");
	tLoop = (ThanosLoop*)boss.GetScript("ThanosLoop");
	pStats = (PlayerStats*)player.GetScript("PlayerStats");
	pMove = (PlayerMove*)player.GetScript("PlayerMove");
	thanosState = THANOS_STATE::SEEKING;
	sword.SetActive(false);
	melee1.SetActive(false);
	defenseSword.SetActive(false);
	explosionWave.SetActive(false);
	bullet1.SetActive(false);
	bullet2.SetActive(false);
	bullet3.SetActive(false);
	area.SetActive(false);
	laserGO.SetActive(false);

	bullets[0] = bullet1;
	bullets[1] = bullet2;
	bullets[2] = bullet3;

	beams[0] = beam1;
	beams[1] = beam2;
	beams[2] = beam3;
	beams[3] = beam4;

	beamTargets[0] = beamTarget1;
	beamTargets[1] = beamTarget2;
	beamTargets[2] = beamTarget3;
	beamTargets[3] = beamTarget4;

	for (int i = 0; i < 30; i++) {
		meteorsPosition[i] = meteors[i].GetTransform().GetGlobalPosition();
	}

}
void ThanosAttacks::Update()
{
	//defenseSword.GetTransform().SetPosition(boss.GetTransform().GetGlobalPosition());
	if (tLoop->phase == 2) {
		//2ND phase!!!
		if (pulse == false) {
			pulse = true;
			sword.SetActive(false);
			melee1.SetActive(false);
			thanosState = THANOS_STATE::PULSE;
		}

		

		if (meteorRainCooldown > 30.0f) {
			isRainingMeteors = true;
		}
		else {
			meteorRainCooldown += Time::GetDeltaTime();
		}

		if (isRainingMeteors == true) {
			MeteorAttack();
		}

		switch (thanosState)
		{
		case THANOS_STATE::PULSE:

			Console::Log("leleleleleleleelelelelele");

			isAttacking = true;
			distSA = player.GetTransform().GetGlobalPosition().Distance(gameObject.GetTransform().GetGlobalPosition());

			explosionTime += Time::GetDeltaTime();
			explosionWave.GetTransform().Scale(20.0f * Time::GetDeltaTime());
			explosionWave.SetActive(true);
			if (explosionTime < 0.5 && distSA < 30.0 && explosionWave1HasArrived == false) {
				pStats->TakeDamage(50, 0);
				explosionWave1HasArrived = true;
			}
			if (explosionTime >= 0.5 && distSA > 30.0 && distSA < 60.0 && explosionWave2HasArrived == false) {

				//pStats->TakeDamage(1, 0);

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
				explosionWave.SetActive(false);
				isAttacking = false;
				thanosState = THANOS_STATE::IDLE;
			}
			break;
		case THANOS_STATE::IDLE:

			for (int i = 0; i < 4; i++) {
				beams[i].SetActive(false);
			}

			attackType = rand() % 100 + 1;

			isAttacking = true;

			charge += Time::GetDeltaTime();
			if (charge > 2.0f) {
				charge = 0.0f;
				if (attackType > 75) thanosState = THANOS_STATE::BURST;
				if (attackType <= 75 && attackType > 50) thanosState = THANOS_STATE::BEAM;
				if (attackType < 50 && attackType > 25) {
					playerPosition = player.GetTransform().GetGlobalPosition();
					thanosPosition = gameObject.GetTransform().GetGlobalPosition();
					thanosState = THANOS_STATE::DASH2;
				}
				if (attackType <= 25) {
					playerPosition = player.GetTransform().GetGlobalPosition();
					//laserCilinder.SetActive(true);
					thanosState = THANOS_STATE::LASER;
				}
			}

			break;

		case THANOS_STATE::BURST:
			
			busrstTime += Time::GetDeltaTime();

			for (int i = 0; i < 3; i++) {
				if (busrstTime > burstTimes[i]) {
					if (bulletThrown[i] == false) {
						bulletThrown[i] = true;
						playerPositions[i] = player.GetTransform().GetGlobalPosition();
						bullets[i].SetActive(true);
					}
					BulletSeek(&bullets[i], playerPositions[i], bulletSpeed, i);
				}
				else bullets[i].GetTransform().SetPosition(gameObject.GetTransform().GetGlobalPosition());
			}
			
			if (busrstTime > burstTimes[3]) {
				thanosState = THANOS_STATE::IDLE;
				isAttacking = false;
				busrstTime = 0.0f;
				for (int i = 0; i < 3; i++) {
					bulletThrown[i] = false;
				}

			}

			break;

		case THANOS_STATE::BEAM:
			
			beamTime += Time::GetDeltaTime();

			for (int i = 0; i < 4; i++) {
				if (beamTime > beamTimes[i]) {
					if (beamThrown[i] == false) {
						beamThrown[i] = true;
						beams[i].SetActive(true);
						beamPositions[i] = beamTargets[i].GetTransform().GetGlobalPosition();
						angle = Rotate(beamPositions[i], angle, &beams[i]);
					}
					if (i == 1 || i == 2) {
						BulletSeek(&beams[i], beamPositions[i], beamSpeed * 1.2f , i);
					}
					else BulletSeek(&beams[i],beamPositions[i], beamSpeed, i);
				}
				else beams[i].GetTransform().SetPosition(gameObject.GetTransform().GetGlobalPosition());
			}

			if (beamTime > beamTimes[4]) {
				thanosState = THANOS_STATE::IDLE;
				isAttacking = false;
				beamTime = 0.0f;
				for (int i = 0; i < 4; i++) {
					beamThrown[i] = false;
				}

			}
			break;

		case THANOS_STATE::DASH2:

			if (areaDmg == false) {
				Seek2(&gameObject, playerPosition, dashSpeed);
			}
			else {
				charge += Time::GetDeltaTime();

				if (charge > 0.25f) {
					area.SetActive(false);
					Seek2(&gameObject, thanosPosition, dashSpeed);
				}
			}

			break;
		case THANOS_STATE::LASER:
			
			charge += Time::GetDeltaTime();

			Seek2(&laserPosition, player.GetTransform().GetGlobalPosition(), laserSpeed / 6);

			if (charge > 0.5f) {
				
				Rotate2(laserPosition.GetTransform().GetGlobalPosition(), 0, &laserGO);
				laserPS.Play();
				laserPS.SetInitialSpeed(boss.GetTransform().GetForward() * 40.0f);
				laserGO.SetActive(true);
			}

			if (charge > 10.5f) {
				thanosState = THANOS_STATE::IDLE;
				laserPS.StopEmitting();
				charge = 0.0f;
				laserGO.SetActive(false);
				isAttacking = false;
			}

			break;
		default:
			break;
		}

	}
	else {
		if (isAttacking) {
			//defenseSword.GetTransform().SetPosition(0, 100000000, 0);
			switch (thanosState)
			{
			case THANOS_STATE::IDLE:
			{
				float selectAttack = rand() % 10 + 1;

				charge += Time::GetDeltaTime();

				if (charge > 0.25f) {
					charge = 0.0f;
					if (selectAttack < 6 || tLoop->phase == 2) {
						playerPosition = player.GetTransform().GetGlobalPosition();

						thanosState = THANOS_STATE::THROWINGATTACK;
						sword.SetActive(true);
					}
					else {
						playerPosition = player.GetTransform().GetGlobalPosition();

						thanosState = THANOS_STATE::DASHATTACK;
					}
				}

			}break;
			case THANOS_STATE::MELEEATTACK:

				if (dashing == false) {
					tMeleeDmg->meleeDmg = 30;
				}
				MeleeAttack();

				break;

			case THANOS_STATE::DASHATTACK:
					tMeleeDmg->meleeDmg = 50;
					dashing = true;
					DashAttack();

				break;
			case THANOS_STATE::THROWINGATTACK:

				isAttacking = true;
				if (swordThrown == false) {

					aimPosition = bTarget.GetTransform().GetGlobalPosition();

					swordThrown = true;
				}

				Seek(&sword, aimPosition, swordSpeed);
				swordTime += Time::GetDeltaTime();

				if (swordTime > 1.75f) {
					swordThrown = false;
					isAttacking = false;
					thanosState = THANOS_STATE::SEEKING;
					sword.SetActive(false);
					swordTime = 0.0f;
				}


				break;
			default:
				break;
			}
		}
		else {
			sword.GetTransform().SetPosition(boss.GetTransform().GetGlobalPosition());
			//defenseSword.GetTransform().SetPosition({ 0,1,1 });
			//defenseSword.SetActive(true);
		}
	}
	
}

void ThanosAttacks::MeleeAttack() {

	meleeAttackTime += Time::GetDeltaTime();
	if (meleeAttackTime >= 0.5f) {
		melee1.SetActive(false);
		meleeAttackTime = 0.0f;
		isAttacking = false;
		thanosState = THANOS_STATE::SEEKING;
		dashing = false;
	}
	else if (meleeAttackTime > 0.05f) {
		melee1.SetActive(true);
	}
}

void ThanosAttacks::DashAttack() {
	Seek(&boss, playerPosition, 1.5f);
}

void ThanosAttacks::Seek2(API_GameObject* seeker, API_Vector3 target, float speed) {

	API_Vector3 direction = target - seeker->GetTransform().GetGlobalPosition();
	seeker->GetTransform().Translate(direction * speed * Time::GetDeltaTime() * 10);

	if (direction.x < 0.15 && direction.x > -0.15 && direction.y < 0.15 && direction.y && direction.z < 0.15 && direction.z) {
	
		if (thanosState == THANOS_STATE::DASH2) {
			if (areaDmg == false) {
				areaDmg = true;
				area.SetActive(true);
			}
			else if(thanosState != THANOS_STATE::LASER){
				thanosState = THANOS_STATE::IDLE;
				isAttacking = false;
				areaDmg = false;
				charge = 0.0f;
			}
		}
	
	}

}
void ThanosAttacks::Seek(API_GameObject* seeker, API_Vector3 target, float speed)
{
	API_Vector3 direction = target - seeker->GetTransform().GetGlobalPosition();
	seeker->GetTransform().Translate(direction * speed * Time::GetDeltaTime() * 10);

	if (direction.x < 0.15 && direction.x > -0.15 && direction.y < 0.15 && direction.y && direction.z < 0.15 && direction.z) {
		if (tLoop->phase == 2) {
			Console::Log("telodoyyyy");
			thanosState = THANOS_STATE::PULSE;
			isAttacking = false;
			finalSword = true;
			//defenseSword.SetActive(false);
		}
		if (thanosState == THANOS_STATE::THROWINGATTACK) {
			if (aimPosition == boss.GetTransform().GetGlobalPosition()) {
				swordThrown = false;
				isAttacking = false;
				thanosState = THANOS_STATE::SEEKING;
				sword.SetActive(false);
			}
			aimPosition = boss.GetTransform().GetGlobalPosition();
		}
		else if(thanosState != THANOS_STATE::PULSE) thanosState = THANOS_STATE::MELEEATTACK;
	}
}

void ThanosAttacks::BulletSeek(API_GameObject* seeker, API_Vector3 target, float speed, int numBullet)
{
	API_Vector3 direction = target - seeker->GetTransform().GetGlobalPosition();
	seeker->GetTransform().Translate(direction * speed * Time::GetDeltaTime());

	if (direction.x < 0.3 && direction.x > -0.3 && direction.y < 0.3 && direction.y && direction.z < 0.3 && direction.z) {
		seeker->SetActive(false);
	}

}

float ThanosAttacks::Rotate(API_Vector3 target, float _angle, API_GameObject* rotator)
{
	API_Vector2 lookDir;
	lookDir.x = (target.x - rotator->GetTransform().GetGlobalPosition().x);
	lookDir.y = (target.z - rotator->GetTransform().GetGlobalPosition().z);

	API_Vector2 normLookDir;
	normLookDir.x = lookDir.x / sqrt(pow(lookDir.x, 2) + pow(lookDir.y, 2));
	normLookDir.y = lookDir.y / sqrt(pow(lookDir.x, 2) + pow(lookDir.y, 2));
	_angle = 0;
	_angle = atan2(normLookDir.y, normLookDir.x) * RADTODEG - 90.0f;
	rotator->GetTransform().SetRotation(0, -_angle - 90, 0);

	return _angle;
}

float ThanosAttacks::Rotate2(API_Vector3 target, float _angle, API_GameObject* rotator)
{
	API_Vector2 lookDir;
	lookDir.x = (target.z - rotator->GetTransform().GetGlobalPosition().z);
	lookDir.y = (target.y - rotator->GetTransform().GetGlobalPosition().y);

	API_Vector2 normLookDir;
	normLookDir.x = lookDir.x / sqrt(pow(lookDir.x, 2) + pow(lookDir.y, 2));
	normLookDir.y = lookDir.y / sqrt(pow(lookDir.x, 2) + pow(lookDir.y, 2));
	_angle = 0;
	_angle = atan2(normLookDir.y, normLookDir.x) * RADTODEG - 90.0f;
	rotator->GetTransform().SetRotation(_angle - 90, 0, 0);

	return _angle;
}

void ThanosAttacks::MeteorAttack() {
	meteorRainTime += Time::GetDeltaTime();

	if (meteorRainTime < 30.0f) {
		for(int i = 0; i < 30; i++){
			meteors[i].GetTransform().Translate(0, -meteorRainSpeed * Time::GetDeltaTime(),0);
		}
		//meteorRain.GetTransform().Translate(0, -meteorRainSpeed * Time::GetDeltaTime(), 0);
	}
	else {
		meteorRainTime = 0.0f;
		for (int i = 0; i < 30; i++) {
			meteors[i].GetTransform().SetPosition(meteorsPosition[i]);
			meteors[i].GetMeshRenderer().SetActive(true);
		}
		thanosState = THANOS_STATE::IDLE;
		isRainingMeteors = false;
		meteorRainCooldown = 0.0f;
	}
}

void ThanosAttacks::LookAt(API_Vector3 tarPos, API_GameObject* go)
{
	float norm = sqrt(tarPos.x * tarPos.x + tarPos.y * tarPos.y);
	API_Vector3 movDir;
	movDir.x = tarPos.x / norm;
	movDir.y = 0.0f;
	movDir.z = tarPos.y / norm;

	go->GetTransform().SetRotation(0.0f, atan2(movDir.x, movDir.z), 0.0f);
}
