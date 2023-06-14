#include "ThanosAttacks.h"
#include "ThanosMeleeDmg.h"
#include "ThanosLoop.h"
#include "../Player/PlayerStats.h"
#include "../Player/PlayerMove.h"
#include "../../Assets/Game/Characters/Enemies/enemyThanos_Animations/ThanosAssets/Columna.h"

HELLO_ENGINE_API_C ThanosAttacks* CreateThanosAttacks(ScriptToInspectorInterface* script)
{
	ThanosAttacks* classInstance = new ThanosAttacks();

	script->AddCheckBox("IsAttacking?", &classInstance->isAttacking);

	script->AddDragBoxGameObject("Boss", &classInstance->boss);
	script->AddDragBoxGameObject("Player", &classInstance->player);

	script->AddDragBoxGameObject("Melee Attack 1", &classInstance->melee1); 
	script->AddDragBoxGameObject("Area", &classInstance->area); 
	script->AddDragBoxGameObject("Area2", &classInstance->area2); 
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

	script->AddDragBoxGameObject("Laser Aim Position", &classInstance->laserPosition);
	script->AddDragBoxParticleSystem("Laser Particle System 1", &classInstance->laserPS);
	script->AddDragBoxParticleSystem("Laser Particle System 2", &classInstance->laserPS2);
	script->AddDragBoxParticleSystem("Laser Particle System 3", &classInstance->laserPS3);
	script->AddDragBoxGameObject("Laser GameObject 1", &classInstance->laserGO);
	script->AddDragBoxGameObject("Laser GameObject 2", &classInstance->laserGO2);
	script->AddDragBoxGameObject("Laser GameObject 3", &classInstance->laserGO3);

	script->AddDragBoxGameObject("Meteor 0", &classInstance->meteors[0]);
	script->AddDragBoxGameObject("Meteor 1", &classInstance->meteors[1]);
	script->AddDragBoxGameObject("Meteor 2", &classInstance->meteors[2]);
	script->AddDragBoxGameObject("Meteor 3", &classInstance->meteors[3]);
	script->AddDragBoxGameObject("Meteor 4", &classInstance->meteors[4]);
	script->AddDragBoxGameObject("Meteor 5", &classInstance->meteors[5]);
	script->AddDragBoxGameObject("Meteor 6", &classInstance->meteors[6]);
	script->AddDragBoxGameObject("Meteor 7", &classInstance->meteors[7]);
	script->AddDragBoxGameObject("Meteor 8", &classInstance->meteors[8]);
	script->AddDragBoxGameObject("Meteor 9", &classInstance->meteors[9]);
	script->AddDragBoxGameObject("Meteor 10", &classInstance->meteors[10]);
	script->AddDragBoxGameObject("Meteor 11", &classInstance->meteors[11]);
	script->AddDragBoxGameObject("Meteor 12", &classInstance->meteors[12]);
	script->AddDragBoxGameObject("Meteor 13", &classInstance->meteors[13]);
	script->AddDragBoxGameObject("Meteor 14", &classInstance->meteors[14]);
	script->AddDragBoxGameObject("Meteor 15", &classInstance->meteors[15]);
	script->AddDragBoxGameObject("Meteor 16", &classInstance->meteors[16]);
	script->AddDragBoxGameObject("Meteor 17", &classInstance->meteors[17]);
	script->AddDragBoxGameObject("Meteor 18", &classInstance->meteors[18]);
	script->AddDragBoxGameObject("Meteor 19", &classInstance->meteors[19]);
	script->AddDragBoxGameObject("Meteor 20", &classInstance->meteors[20]);
	script->AddDragBoxGameObject("Meteor 21", &classInstance->meteors[21]);
	script->AddDragBoxGameObject("Meteor 22", &classInstance->meteors[22]);
	script->AddDragBoxGameObject("Meteor 23", &classInstance->meteors[23]);
	script->AddDragBoxGameObject("Meteor 24", &classInstance->meteors[24]);
	script->AddDragBoxGameObject("Meteor 25", &classInstance->meteors[25]);
	script->AddDragBoxGameObject("Meteor 26", &classInstance->meteors[26]);
	script->AddDragBoxGameObject("Meteor 27", &classInstance->meteors[27]);
	script->AddDragBoxGameObject("Meteor 28", &classInstance->meteors[28]);
	script->AddDragBoxGameObject("Meteor 29", &classInstance->meteors[29]);

	script->AddDragBoxAnimationPlayer("Thanos Animation Player", &classInstance->thanosAnimationPlayer);
	script->AddDragBoxAnimationResource("Thanos Aim Animation", &classInstance->thanosAimAnimation);
	script->AddDragBoxAnimationResource("Thanos Melee Animation", &classInstance->thanosMeleeAnimation);
	script->AddDragBoxAnimationResource("Thanos Charge Attack Animation", &classInstance->thanosChargeAttackAnimation);
	script->AddDragBoxAnimationResource("Thanos Charge Only Animation", &classInstance->thanosChargeOnlyAnimation);
	script->AddDragBoxAnimationResource("Thanos Gauntlet Recoil Animation", &classInstance->thanosGuantletRecoilAnimation);
	script->AddDragBoxAnimationResource("Thanos Idle Animation (With Sword)", &classInstance->thanosIdleAnimation);
	script->AddDragBoxAnimationResource("Thanos Idle Animation (With No Sword)", &classInstance->thanosIdle2Animation);
	script->AddDragBoxAnimationResource("Thanos Meteor Animation", &classInstance->thanosMeteorAnimation);
	script->AddDragBoxAnimationResource("Thanos Pulse Animation", &classInstance->thanosPulseAnimation);
	script->AddDragBoxAnimationResource("Thanos Run Animation", &classInstance->thanosRunAnimation);
	script->AddDragBoxAnimationResource("Thanos Shoot Animation", &classInstance->thanosShootAnimation);
	script->AddDragBoxAnimationResource("Thanos Spin Animation", &classInstance->thanosSpinAnimation);
	script->AddDragBoxAnimationResource("Thanos Start Spin Animation", &classInstance->thanosStartSpinAnimation);
	script->AddDragBoxAnimationResource("Thanos Stop Spin Animation", &classInstance->thanosStopSpinAnimation);
	script->AddDragBoxAnimationResource("Thanos Start Sword Thrown Animation", &classInstance->thanosStartSwordThrownAnimation);
	script->AddDragBoxAnimationResource("Thanos Stop Sword Thrown Animation", &classInstance->thanosStopSwordThrownAnimation);
	script->AddDragBoxAnimationResource("Thanos Sword Thrown Animation", &classInstance->thanosSwordThrownAnimation);
	script->AddDragBoxAnimationResource("Thanos Walk Animation (No sword)", &classInstance->thanosWalk2Animation);
	script->AddDragBoxAnimationResource("Thanos Walk Animation (With sword)", &classInstance->thanosWalkAnimation);
	script->AddDragBoxAnimationResource("Thanos Walk Backwards Animation (Without sword)", &classInstance->thanosWalkBackwardsAnimation);
	script->AddDragBoxAnimationResource("Thanos Getting On Combat", &classInstance->thanosWakeUp);
	script->AddDragBoxAnimationResource("Thanos Out Of Combat", &classInstance->thanosOutOfCombat);
	script->AddDragBoxAnimationResource("Thanos Punch Attack", &classInstance->thanosPunchAttack);
	script->AddDragBoxAnimationResource("Thanos Punch Attack 2", &classInstance->thanosPunchAttack2);

	script->AddDragBoxGameObject("Column 0", &classInstance->columns[0]);
	script->AddDragBoxGameObject("Column 1", &classInstance->columns[1]);
	script->AddDragBoxGameObject("Column 2", &classInstance->columns[2]);
	script->AddDragBoxGameObject("Column 3", &classInstance->columns[3]);
	script->AddDragBoxGameObject("Column 4", &classInstance->columns[4]);
	script->AddDragBoxGameObject("Column 5", &classInstance->columns[5]);
	script->AddDragBoxGameObject("Column 6", &classInstance->columns[6]);
	script->AddDragBoxGameObject("Column 7", &classInstance->columns[7]);
	script->AddDragBoxGameObject("Column 8", &classInstance->columns[8]);
	script->AddDragBoxGameObject("Column 9", &classInstance->columns[9]);
	script->AddDragBoxGameObject("Column 10", &classInstance->columns[10]);
	script->AddDragBoxGameObject("Column 11", &classInstance->columns[11]);


	script->AddDragBoxGameObject("Explosion3D", &classInstance->areaImpact);
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
	area2.SetActive(false);
	laserGO.SetActive(false);
	laserGO2.SetActive(false);
	laserGO3.SetActive(false);

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
	explosionWave.GetTransform().Translate(0, 0, 0);
	areaImpact.GetTransform().SetPosition(0, -1000, 0);

}
void ThanosAttacks::Update()
{

	if (area.IsActive() == true) {
		brokenFloorTimer += Time::GetDeltaTime();
		if (brokenFloorTimer > 5.0f) {
			brokenFloorTimer = 0.0f;
			area.SetActive(false);
		}
	}

	if (pStats->currentHp < 0 && playerDeath == false) {
		Audio::Event("thanos_laugh");
		playerDeath = true;
	}

	if (whichColumn > -1) {

		for (int i = 0; i < 12; i++) {
			if (columnsStates[i] == true) {
				columns[i].GetTransform().Translate(0, -1 * Time::GetDeltaTime(), 0);
			}
		}

		switch (whichColumn)
		{
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
		case 10:
		case 11:
			columns[whichColumn].GetTransform().Translate(0, -2 * Time::GetDeltaTime(), 0);

			break;
		default:
			break;
		}

	}

	//defenseSword.GetTransform().SetPosition(boss.GetTransform().GetGlobalPosition());
	if (tLoop->phase == 2) {
		//2ND phase!!!
		if (pulse == false) {
			pulse = true;
			sword.SetActive(false);
			melee1.SetActive(false);
			thanosState = THANOS_STATE::PULSE;
			explosionWave.GetTransform().Translate(0, 10, 0);

			areaImpact.SetActive(true);
			areaImpact.GetTransform().SetPosition(0, -1, 0);
			thanosAnimationPlayer.ChangeAnimation(thanosPulseAnimation);
			thanosAnimationPlayer.SetLoop(false);
			thanosAnimationPlayer.Play();
		}

		

		if (meteorRainCooldown > 30.0f && isRainingMeteors == false) {
			meteorQueue = true;
			//thanosState = THANOS_STATE::IDLE;
			//isAttacking = false;
			//thanosAnimationPlayer.ChangeAnimation(thanosMeteorAnimation);
			//thanosAnimationPlayer.SetLoop(false);
			//thanosAnimationPlayer.Play();
			isMeteorAnimationReady = true;
			isRainingMeteors = true;
			
		}
		else {
			meteorRainCooldown += Time::GetDeltaTime();
		}

		if (isRainingMeteors == true) {
			meteorAnimationCooldown += Time::GetDeltaTime();
			if(meteorAnimationCooldown > 2.75f && isMeteorAnimationReady == true){
				isMeteorAnimationReady = false;
				thanosAnimationPlayer.ChangeAnimation(thanosIdle2Animation);
				thanosAnimationPlayer.SetLoop(true);
				thanosAnimationPlayer.Play();
				Audio::Event("thanos_meteor_spawn");
			}
			MeteorAttack();
		}

		switch (thanosState)
		{
		case THANOS_STATE::PULSE:

			isAttacking = true;
			distSA = player.GetTransform().GetGlobalPosition().Distance(gameObject.GetTransform().GetGlobalPosition());

			explosionTime += Time::GetDeltaTime();				
			areaImpact.GetTransform().Scale(3.5f * Time::GetDeltaTime());
			areaImpact.GetTransform().Rotate(4000 * Time::GetDeltaTime(), 2600 * Time::GetDeltaTime(), 5800 * Time::GetDeltaTime());


			//explosionWave.GetTransform().Scale(20.0f * Time::GetDeltaTime());
			explosionWave.SetActive(true);
			explosionWave.GetParticleSystem().Play();
			if (explosionTime > 2.8f && distSA < 15.0 && explosionWave1HasArrived == false) {
				pStats->TakeDamage(25, 0);
				explosionWave1HasArrived = true;
			}
			else if (explosionTime >= 2.9f && distSA > 15.0 && distSA < 30.0 && explosionWave2HasArrived == false) {

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
			if (explosionTime > 4.0f) {
				explosionWave.GetParticleSystem().Stop();
				explosionWave.GetParticleSystem().StopEmitting();
				explosionWave.SetActive(false);
				explosionWave.GetTransform().Translate(0, -10, 0);

				hasPulsed = true;
				isAttacking = false;

				thanosAnimationPlayer.ChangeAnimation(thanosWalk2Animation);
				thanosAnimationPlayer.SetLoop(false);
				thanosAnimationPlayer.Play();
				thanosState = THANOS_STATE::IDLE;
				areaImpact.GetTransform().SetScale(1, 1, 1);
				areaImpact.GetTransform().SetPosition(0, -1000, 0);
				areaImpact.SetActive(false);
			}
			break;
		case THANOS_STATE::IDLE:

			laserPS.Stop();
			laserPS2.Stop();
			laserPS3.Stop();

			for (int i = 0; i < 4; i++) {
				beams[i].SetActive(false);
			}

			if (meteorQueue == true) {
				meteorQueue = false;
				thanosAnimationPlayer.ChangeAnimation(thanosMeteorAnimation);
				thanosAnimationPlayer.SetLoop(false);
				thanosAnimationPlayer.Play();
			}


			attackType = rand() % 100 + 1;

			isAttacking = true;
			charge += Time::GetDeltaTime();
			if (charge > 2.0f) {
				charge = 0.0f;
				if (attackType > 75) {
					thanosState = THANOS_STATE::BURST;
				
					thanosAnimationPlayer.ChangeAnimation(thanosShootAnimation);
					thanosAnimationPlayer.SetLoop(true);
					thanosAnimationPlayer.Play();

					
				
				}
				if (attackType <= 75 && attackType > 50) {

					thanosState = THANOS_STATE::BEAM;
					thanosAnimationPlayer.ChangeAnimation(thanosGuantletRecoilAnimation);
					thanosAnimationPlayer.SetLoop(true);
					thanosAnimationPlayer.Play();
				}
				if (attackType < 50 && attackType > 25) {
					playerPosition = player.GetTransform().GetGlobalPosition();
					thanosPosition = gameObject.GetTransform().GetGlobalPosition();
					thanosState = THANOS_STATE::DASH2;
					Audio::Event("thanos_power_charging");
					thanosAnimationPlayer.ChangeAnimation(thanosPunchAttack);
					thanosAnimationPlayer.SetLoop(false);
					thanosAnimationPlayer.Play();
					brokenFloorTimer = 0.0f;

				}
				if (attackType <= 25) {
					playerPosition = player.GetTransform().GetGlobalPosition();

					thanosAnimationPlayer.ChangeAnimation(thanosAimAnimation);
					thanosAnimationPlayer.SetLoop(false);
					thanosAnimationPlayer.Play();
					//laserCilinder.SetActive(true);
					thanosState = THANOS_STATE::LASER;
					Audio::Event("thanos_lasers");
				}
			}

			break;

		case THANOS_STATE::BURST:
			
			busrstTime += Time::GetDeltaTime();

			for (int i = 0; i < 3; i++) {
				if (busrstTime > burstTimes[i]) {
					if (bulletThrown[i] == false) {
						bulletThrown[i] = true;
						API_Vector3 normalizedvector = boss.GetTransform().GetGlobalPosition() - player.GetTransform().GetGlobalPosition();
						float x = normalizedvector.x * normalizedvector.x;
						float y = 0;
						float z = normalizedvector.z * normalizedvector.z;
						float sum = x + y + z;
						API_Vector3 direction = { normalizedvector.x / sum, 0, normalizedvector.z / sum };
						playerPositions[i] = player.GetTransform().GetGlobalPosition() - direction * 100;
						bullets[i].SetActive(true);
						bullets[i].GetTransform().SetRotation(0, gameObject.GetTransform().GetLocalRotation().y - 90, 0);
					}
					BulletSeek(&bullets[i], playerPositions[i], bulletSpeed, i);
				}
				else bullets[i].GetTransform().SetPosition(gameObject.GetTransform().GetGlobalPosition());
			}
			
			if (busrstTime > burstTimes[3]) {
				thanosAnimationPlayer.ChangeAnimation(thanosWalk2Animation);
				thanosAnimationPlayer.SetLoop(true);
				thanosAnimationPlayer.Play();
				thanosState = THANOS_STATE::IDLE;
				isAttacking = false;
				busrstTime = 0.0f;
				for (int i = 0; i < 3; i++) {
					bulletThrown[i] = false;
					bullets[i].SetActive(false);
					bullets[i].GetTransform().SetPosition(0,0,0);
				}

			}

			break;

		case THANOS_STATE::BEAM:
			
			beamTime += Time::GetDeltaTime();

			for (int i = 0; i < 4; i++) {
				if (beamTime > beamTimes[i]) {
					if (beamThrown[i] == false) {
						Audio::Event("thanos_wall_throw");
						beamThrown[i] = true;
						beams[i].SetActive(true);
						beams[i].GetParticleSystem().Play();
						beamPositions[i] = beamTargets[i].GetTransform().GetGlobalPosition();
						angle = Rotate(beamPositions[i], angle+180, &beams[i]);
					}
					if (i == 1 || i == 2) {
						BulletSeek(&beams[i], beamPositions[i], beamSpeed * 1.2f , i);
					}
					else BulletSeek(&beams[i],beamPositions[i], beamSpeed, i);
				}
				else beams[i].GetTransform().SetPosition(gameObject.GetTransform().GetGlobalPosition());
			}

			if (beamTime > beamTimes[4]) {

				thanosAnimationPlayer.ChangeAnimation(thanosWalk2Animation);
				thanosAnimationPlayer.SetLoop(true);
				thanosAnimationPlayer.Play();
				thanosState = THANOS_STATE::IDLE;
				isAttacking = false;
				beamTime = 0.0f;
				for (int i = 0; i < 4; i++) {
					beamThrown[i] = false;

					beams[i].GetParticleSystem().Stop();
					beams[i].GetParticleSystem().StopEmitting();
				}

			}
			break;

		case THANOS_STATE::DASH2:
			anotherTimer += Time::GetDeltaTime();
			if (anotherTimer > 2.0f) {
				anotherTimer = 0.0f;
				thanosState = THANOS_STATE::IDLE;
				isAttacking = false;
				areaDmg = false;
				charge = 0.0f;
			}
			if (areaDmg == false) {
				
				charge += Time::GetDeltaTime();

				if (charge > 1.0f) {
					Seek2(&gameObject, playerPosition, dashSpeed / 4);
					if (punchAnimation == false) {

						thanosAnimationPlayer.ChangeAnimation(thanosPunchAttack2);
						thanosAnimationPlayer.SetLoop(false);
						thanosAnimationPlayer.Play();
						punchAnimation = true;

					}
				}

			}
			else {
				Seek2(&gameObject, thanosPosition, dashSpeed);
				

			}

			break;
		case THANOS_STATE::LASER:
			
			charge += Time::GetDeltaTime();

			Seek2(&laserPosition, player.GetTransform().GetGlobalPosition(), laserSpeed / 4);

			if (charge > 0.5f) {

				//Rotate2(laserPosition.GetTransform().GetGlobalPosition(), 0, &laserGO);
				laserPS.Play();
				laserPS.SetInitialSpeed(boss.GetTransform().GetForward() * 40.0f);
				laserPS2.Play();
				laserPS2.SetInitialSpeed(laserGO2.GetTransform().GetForward() * 40.0f);
				laserPS3.Play();
				laserPS3.SetInitialSpeed(laserGO3.GetTransform().GetForward() * 40.0f);
				laserGO.SetActive(true);
				laserGO2.SetActive(true);
				laserGO3.SetActive(true);
			}
			if (charge > 0.75f) {
				thanosAnimationPlayer.Pause();

			}

			if (charge > 8.5f && laserExpanded == false) {
				laserGO2.GetTransform().Rotate(0, -Time::GetDeltaTime() * 100, 0);
				laserGO3.GetTransform().Rotate(0, Time::GetDeltaTime() * 100 , 0);
			}

			if (charge > 9.0f) {
				laserExpanded = true;
				laserGO2.GetTransform().Rotate(0, Time::GetDeltaTime() * 100, 0);
				laserGO3.GetTransform().Rotate(0, -Time::GetDeltaTime() * 100, 0);
			}

			if (charge > 10.5f) {
				laserExpanded = false;
				thanosAnimationPlayer.Resume();
				thanosAnimationPlayer.ChangeAnimation(thanosWalk2Animation);
				thanosAnimationPlayer.SetLoop(true);
				thanosAnimationPlayer.Play();
				thanosState = THANOS_STATE::IDLE;
				laserPS.StopEmitting();
				laserPS.Stop();
				laserPS2.StopEmitting();
				laserPS2.Stop();
				laserPS3.StopEmitting();
				laserPS3.Stop();
				charge = 0.0f;
				laserGO.SetActive(false);
				laserGO2.GetTransform().SetRotation(0.0f, 53.4f, 0.0f);
				laserGO2.SetActive(false);
				laserGO3.GetTransform().SetRotation(0.0f, -53.4f, 0.0f);
				laserGO3.SetActive(false);
				isAttacking = false;
			}

			break;
		default:
			break;
		}

	}
	else if(tLoop->phase == 1){
		if (isAttacking) {
			//defenseSword.GetTransform().SetPosition(0, 100000000, 0);
			switch (thanosState)
			{
			case THANOS_STATE::IDLE:
			{
				if (justRandOnce == false) {
					selectAttack = rand() % 10 + 1;
					justRandOnce = true; 
					if (selectAttack < 6 || tLoop->phase == 2) {
						thanosAnimationPlayer.ChangeAnimation(thanosStopSwordThrownAnimation);
						thanosAnimationPlayer.SetLoop(false);
						thanosAnimationPlayer.Play();
					}
					else {
						thanosAnimationPlayer.ChangeAnimation(thanosChargeOnlyAnimation);
						thanosAnimationPlayer.SetLoop(false);
						thanosAnimationPlayer.Play();
					}
				}
				

				charge += Time::GetDeltaTime();

				
				if (charge > 0.25f) {
					justRandOnce = false;

					charge = 0.0f;
					if (selectAttack < 6 || tLoop->phase == 2) {
						playerPosition = player.GetTransform().GetGlobalPosition();
						thanosAnimationPlayer.Pause();
						thanosState = THANOS_STATE::THROWINGATTACK;
						sword.SetActive(true);
					}
					else {
						playerPosition = player.GetTransform().GetGlobalPosition();
						thanosAnimationPlayer.ChangeAnimation(thanosChargeAttackAnimation);
						thanosAnimationPlayer.SetLoop(false);
						thanosAnimationPlayer.Play();
						thanosState = THANOS_STATE::DASHATTACK;
						Audio::Event("thanos_power_charging");

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

					dashTimer += Time::GetDeltaTime();

					if (dashTimer > 1.5f) {
						thanosAnimationPlayer.ChangeAnimation(thanosMeleeAnimation);
						thanosAnimationPlayer.SetLoop(false);
						thanosAnimationPlayer.Play();
						thanosState = THANOS_STATE::MELEEATTACK;
						dashTimer = 0.0f;
					}

				break;
			case THANOS_STATE::THROWINGATTACK:

				isAttacking = true;
				if (swordThrown == false) {

					aimPosition = bTarget.GetTransform().GetGlobalPosition();
					Audio::Event("thanos_sword_throw");
					swordThrown = true;
				}

				Seek(&sword, aimPosition, swordSpeed);
				swordTime += Time::GetDeltaTime();
				sword.GetTransform().Rotate(0, 0, 3000 * Time::GetDeltaTime());
				if (swordTime > 1.75f) {
					swordThrown = false;
					isAttacking = false;
					thanosAnimationPlayer.ChangeAnimation(thanosRunAnimation);
					thanosAnimationPlayer.SetLoop(true);
					thanosAnimationPlayer.Play();
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
	if (tLoop->hp <= 0) {
		laserPS.Stop();
		thanosState = THANOS_STATE::DEAD;
		tLoop->phase = 3;
		isAttacking = false;
		deathTimer += Time::GetDeltaTime();
		if (isDead == false) {
			thanosAnimationPlayer.ChangeAnimation(thanosOutOfCombat);
			thanosAnimationPlayer.SetLoop(false);
			thanosAnimationPlayer.Play();
			isDead = true;
		}
		
		if (deathTimer > 2.0f) {
			thanosAnimationPlayer.Pause();
		}
	}
}

void ThanosAttacks::MeleeAttack() {

	meleeAttackTime += Time::GetDeltaTime();
	if (meleeAttackTime >= 0.75f) {
		dashTimer = 0.0f;
		melee1.SetActive(false);
		meleeAttackTime = 0.0f;
		isAttacking = false;
		thanosState = THANOS_STATE::SEEKING;
		dashing = false;
		thanosAnimationPlayer.ChangeAnimation(thanosRunAnimation);
		thanosAnimationPlayer.SetLoop(true);
		thanosAnimationPlayer.Play();
	}
	else if (meleeAttackTime > 0.05f) {
		melee1.SetActive(true);
	}
}

void ThanosAttacks::DashAttack() {
	Seek(&boss, playerPosition, 0.5f);
}

void ThanosAttacks::Seek2(API_GameObject* seeker, API_Vector3 target, float speed) {

	API_Vector3 direction = target - seeker->GetTransform().GetGlobalPosition();
	seeker->GetTransform().Translate(direction * speed * Time::GetDeltaTime() * 10);

	if (direction.x < 0.15 && direction.x > -0.15 && direction.y < 0.15 && direction.y && direction.z < 0.15 && direction.z) {
	
		if (thanosState == THANOS_STATE::DASH2) {
			if (areaDmg == false) {
				areaDmg = true;
				area.SetActive(true);
				area2.SetActive(false);
				area.GetTransform().SetPosition({ gameObject.GetTransform().GetGlobalPosition().x, gameObject.GetTransform().GetGlobalPosition().y - 0.25f, gameObject.GetTransform().GetGlobalPosition().z });
				thanosAnimationPlayer.ChangeAnimation(thanosIdle2Animation);
				thanosAnimationPlayer.SetLoop(true);
				thanosAnimationPlayer.Play();
			}
			else if(thanosState != THANOS_STATE::LASER){
				anotherTimer = 0.0f;
				thanosState = THANOS_STATE::IDLE;
				isAttacking = false;
				areaDmg = false;
				charge = 0.0f;
				punchAnimation = false;
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

			areaImpact.SetActive(true);
			areaImpact.GetTransform().SetPosition(0, -1, 0);
			thanosState = THANOS_STATE::PULSE;
			isAttacking = false;
			finalSword = true;
			//defenseSword.SetActive(false);
		}
		if (thanosState == THANOS_STATE::THROWINGATTACK) {

			thanosAnimationPlayer.ChangeAnimation(thanosSwordThrownAnimation);
			thanosAnimationPlayer.SetLoop(true);
			thanosAnimationPlayer.Play();
			if (aimPosition == boss.GetTransform().GetGlobalPosition()) {
				swordThrown = false;
				isAttacking = false;
				swordTime = 0.0f;
				thanosState = THANOS_STATE::SEEKING;

				thanosAnimationPlayer.ChangeAnimation(thanosRunAnimation);
				thanosAnimationPlayer.SetLoop(true);
				thanosAnimationPlayer.Play();

				sword.SetActive(false);
			}
			aimPosition = boss.GetTransform().GetGlobalPosition();
		}
		else if (thanosState != THANOS_STATE::PULSE) {
			thanosAnimationPlayer.ChangeAnimation(thanosMeleeAnimation);
			thanosAnimationPlayer.SetLoop(false);
			thanosAnimationPlayer.Play();
			thanosState = THANOS_STATE::MELEEATTACK;
		}
	}
}

void ThanosAttacks::BulletSeek(API_GameObject* seeker, API_Vector3 target, float speed, int numBullet)
{
	API_Vector3 direction = target - seeker->GetTransform().GetGlobalPosition();
	seeker->GetTransform().Translate(direction * speed * Time::GetDeltaTime());

	if (direction.x < 0.9 && direction.x > -0.9 && direction.y < 0.9 && direction.y && direction.z < 0.9 && direction.z) {
		seeker->SetActive(false);
		//seeker->GetParticleSystem().Stop();
		//seeker->GetParticleSystem().StopEmitting();
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
	rotator->GetTransform().SetRotation(0, -_angle, 0);

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
			//meteors[i].GetMeshRenderer().SetActive(true);
		}
		thanosState = THANOS_STATE::IDLE;
		meteorAnimationCooldown = 0.0f;
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

void ThanosAttacks::OnCollisionStay(API::API_RigidBody other)
{
	std::string detectionName = other.GetGameObject().GetName();
	std::string detectionTag = other.GetGameObject().GetTag();

	if ((detectionTag == "Column" || detectionTag == "Torch" || detectionTag == "Box") && (thanosState == THANOS_STATE::DASHATTACK || thanosState == THANOS_STATE::DASH2)) {
		//other.GetGameObject().SetActive(false);
		other.GetGameObject().GetRigidBody().SetTrigger(true);
		if (detectionTag == "Column") {
			Columna* ColumnaScript = (Columna*)other.GetGameObject().GetScript("Columna");
			if (ColumnaScript->isDestroying == false) {
				Audio::Event("column_breaking");
			}
			whichColumn = ColumnaScript->numColumn;
			ColumnaScript->isDestroying = true;
			columnsStates[whichColumn] = ColumnaScript->isDestroying;
			columns[whichColumn].GetParticleSystem().Play();
			columns[whichColumn].GetRigidBody().SetBoxScale({ 0,0,0 });
		}

	}
}

void ThanosAttacks::OnCollisionEnter(API::API_RigidBody other)
{
	std::string detectionName = other.GetGameObject().GetName();
	std::string detectionTag = other.GetGameObject().GetTag();

	if ((detectionTag == "Column"|| detectionTag == "Torch" || detectionTag == "Box") && (thanosState == THANOS_STATE::DASHATTACK || thanosState == THANOS_STATE::DASH2)) {
		other.GetGameObject().GetRigidBody().SetTrigger(true);
		if (detectionTag == "Column") {
			Columna* ColumnaScript = (Columna*)other.GetGameObject().GetScript("Columna");

			whichColumn = ColumnaScript->numColumn;
			ColumnaScript->isDestroying = true;
			Audio::Event("column_breaking");
			columnsStates[whichColumn] = ColumnaScript->isDestroying;
			columns[whichColumn].GetParticleSystem().Play();
			columns[whichColumn].GetRigidBody().SetBoxScale({ 0,0,0 });
		}
	}
}

void ThanosAttacks::OnCollisionExit(API::API_RigidBody other)
{
	std::string detectionName = other.GetGameObject().GetName();
	std::string detectionTag = other.GetGameObject().GetTag();

	if ((detectionTag == "Torch" || detectionTag == "Box") && (thanosState == THANOS_STATE::DASHATTACK || thanosState == THANOS_STATE::DASH2)) {
		other.GetGameObject().GetRigidBody().SetTrigger(false);
	}
}