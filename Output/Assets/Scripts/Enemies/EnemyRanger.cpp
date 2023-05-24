#include "EnemyRanger.h"
#include <time.h>
HELLO_ENGINE_API_C EnemyRanger* CreateEnemyRanger(ScriptToInspectorInterface* script)
{
	EnemyRanger* classInstance = new EnemyRanger();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragFloat("Detection distance", &classInstance->detectionDis);
	script->AddDragFloat("Lossing Enemy distance", &classInstance->lossingDis);
	script->AddDragFloat("Distance player", &classInstance->disPlayer);
	script->AddDragFloat("Range attack", &classInstance->disShoot);
	script->AddDragFloat("Cooldown betwen points", &classInstance->cooldownPoint);
	script->AddDragFloat("OutsideZone Time", &classInstance->outTime);
	script->AddDragInt("Gun Type(0Semi/1burst)", &classInstance->gunType);
	script->AddDragBoxGameObject("Enemy gun", &classInstance->gunObj);
	// script->AddDragBoxGameObject("Target", &classInstance->target);
	script->AddDragBoxGameObject("Action zone", &classInstance->actionZone);
	// script->AddDragBoxRigidBody("Action Rb zone", &classInstance->zoneRb); 
	script->AddDragBoxGameObject("Point 1", &classInstance->listPoints[0]);
	script->AddDragBoxGameObject("Point 2", &classInstance->listPoints[1]);
	script->AddDragBoxGameObject("Point 3", &classInstance->listPoints[2]);
	//script->AddDragBoxGameObject("Point 4", &classInstance->listPoints[3]);
	//script->AddDragBoxGameObject("Point 5", &classInstance->listPoints[4]);
	script->AddDragBoxAnimationPlayer("Animation Player", &classInstance->animationPlayer);
	script->AddDragBoxAnimationResource("Idle Animation", &classInstance->idleAnim);
	script->AddDragBoxAnimationResource("Walk Animation", &classInstance->walkAnim);
	script->AddDragBoxAnimationResource("Run Animation", &classInstance->runAnim);
	script->AddDragBoxAnimationResource("Aim Animation", &classInstance->aimAnim);
	script->AddDragBoxAnimationResource("Hited Animation", &classInstance->hitAnim);
	script->AddDragBoxAnimationResource("Die Animation", &classInstance->dieAnim);
	//script->AddCheckBox("Dashiing", &classInstance->_canWalk);
	script->AddCheckBox("Scripted For Quest", &classInstance->scriptedForQuest);
	return classInstance;
}

void EnemyRanger::Start()
{
	if (!scriptedForQuest)
	{
		Game::FindGameObjectsWithTag("Player", &target, 1);
		//cooldownPoint = 3.0f;
		actualPoint = listPoints[0].GetTransform().GetGlobalPosition();
		zoneRb = actionZone.GetRigidBody();
		_avalPoints = 3;
		enemState = States::WANDERING;

		_movCooldown = 0;
		_outCooldown = 0;
		_agTime = 0.5;
		_agCooldown = 0;
		_canWalk = true;
	}

	switch (gunType)
	{
	case 0:
		enemyGun = (EnemyGun*)gunObj.GetScript("EnemyAutomatic");
		break;
	case 1:
		enemyGun = (EnemyGun*)gunObj.GetScript("EnemyBurst");
		break;
	default:
		break;
	}

	if (!scriptedForQuest)
	{
		//zoneRb.GetGameObject().
		//clock.s
		targStats = (PlayerStats*)target.GetScript("PlayerStats");
	}

	enemy = (Enemy*)gameObject.GetScript("Enemy");

	// Dirt change
	detectionDis = 10.0f;
	lossingDis = 12.0f;
	disPlayer = 8.0f;

	// SetUp for Exclamation effect;
	// 
	// There have problem
	//_exclamationPool = (ExclamationMarkPool*)Game::FindGameObject("ExclamationMarkPool").GetScript("ExclamationMarkPool");
}

void EnemyRanger::Update()
{
	if (!enemy)
		return;

	float dt = Time::GetDeltaTime();

	if (targStats && !scriptedForQuest)
	{
		float distanceBeetweenPlayerEnemy = gameObject.GetTransform().GetGlobalPosition().Distance(target.GetTransform().GetGlobalPosition());

		// Change States 
		do
		{
			// When enemy get some hit, it will reset cooldown
			if (enemy->isHit)
			{
				enemy->isHit = false;
				_outCooldown = 0;
			}

			// If enemy has been slain
			if (enemy->dying)
			{
				enemState = States::DYING;
				break;
			}

			// In Stun state
			if (enemy->takingDmg)
			{
				enemy->enemyAgent.Stop();
				enemy->enemyRb.SetVelocity(0);
				return;
			}

			// Out of zone
			float disZone = gameObject.GetTransform().GetLocalPosition().Distance(actionZone.GetTransform().GetGlobalPosition());
			enemy->isOut = (disZone > (zoneRb.GetRadius() / 2));
			if (enemy->isOut)
			{
				if (enemState == States::WANDERING)
					break;

				_outCooldown += dt;

				if (_outCooldown >= outTime)
				{
					enemState = States::WANDERING;
					if (playerEnterInFollowDis)
					{
						playerEnterInFollowDis = false;
						targStats->detectedCount--;
					}
					break;
				}
			}

			// Not in the zone
			float targDisZone = target.GetTransform().GetGlobalPosition().Distance(actionZone.GetTransform().GetGlobalPosition());
			targDisZone < (zoneRb.GetRadius() / 2) ? enemy->isTargIn = true : enemy->isTargIn = false;
			if (!enemy->isTargIn && enemState == States::WANDERING)
				return;

			// Attack Escape Range
			if (distanceBeetweenPlayerEnemy <= attackEscapeDis)
			{
				enemState = States::ATTACKING_ESCAPE;
				break;
			}
			// Attack Range
			if (distanceBeetweenPlayerEnemy <= attackDis)
			{
				enemState = States::ATTACKING;
				break;
			}
			// Attack Follow Range
			if (distanceBeetweenPlayerEnemy <= attackFollowDis)
			{
				enemState = States::ATTACKING_FOLLOW;
				break;
			}
			// Follow Range
			if (distanceBeetweenPlayerEnemy <= followDis || targStats->detectedCount > 0)
			{
				enemState = States::FOLLOWING;

				// If enemy is out of his wonder range, _outCooldown never will be reset by followDistance
				if (enemy->isOut)
					break;

				_outCooldown = 0;

				if (!playerEnterInFollowDis && enemy->isTargIn)
				{
					playerEnterInFollowDis = true;
					targStats->detectedCount++;
					// For The Mark Effect

					//if (_exclamationPool)
					//	_exclamationPool->ActivateMark(gameObject);
				}
				break;
			}
			// If has been stay of wander UnfollowRange
			if (_outCooldown > 0)
			{
				// We must count cooldown for unfollow the player
				_outCooldown += dt;
				break;
			}
			// UnfollowRange Range
			if (distanceBeetweenPlayerEnemy >= disfollowDis)
			{
				_outCooldown += dt;

				if (_outCooldown >= outTime)
				{
					if (playerEnterInFollowDis)
					{
						playerEnterInFollowDis = false;
						targStats->detectedCount--;
					}
					if (targStats->detectedCount > 0 && !enemy->isOut)
						enemState = States::FOLLOWING;
					else
						enemState = States::WANDERING;
				}
				break;
			}
		} while (false);

		// Execute state behaviour
		switch (enemState)
		{
		case EnemyRanger::States::WANDERING:
		{
			enemy->currentSpeed = enemy->speed * enemy->stunVel * enemy->slowVel/** dt*/;
			enemy->enemyAgent.SetSpeed(enemy->currentSpeed);
			enemy->enemyAgent.Move();
			if (!enemy->staticWand)
			{

				if (gameObject.GetTransform().GetGlobalPosition().Distance(actualPoint) < 4)
				{
					if (++numPoint >= _avalPoints)
						numPoint = 0;
					_canWalk = false;
				}
				if (!_canWalk)
				{
					_movCooldown += dt;
					if (animState != AnimationState::IDLE && !enemy->takingDmg)
					{
						animState = AnimationState::IDLE;
						animationPlayer.ChangeAnimation(idleAnim);
						animationPlayer.Play();
						//Console::Log("Walk");
					}
				}
				else
				{
					_agCooldown += dt;
					if (_agCooldown >= _agTime)
					{
						_agCooldown = 0;
						//Console::Log("vel:" + std::to_string(enemy->enemyAgent.GetSpeed()));
						enemy->enemyAgent.SetDestination(API_Vector3(actualPoint.x, gameObject.GetTransform().GetGlobalPosition().y, actualPoint.z));
					}
				}
				LoookAt(actualPoint);
				if (_movCooldown > cooldownPoint)
				{
					_movCooldown = 0;
					_canWalk = true;
				}
				actualPoint = listPoints[numPoint].GetTransform().GetGlobalPosition();
				if (animState != AnimationState::WALK && !enemy->takingDmg && _canWalk)
				{
					animState = AnimationState::WALK;
					animationPlayer.ChangeAnimation(walkAnim);
					animationPlayer.Play();
					//Console::Log("Walk");
				}
			}
			else
			{
				_agCooldown += dt;
				if (_agCooldown >= _agTime)
				{
					_agCooldown = 0;
					//Console::Log("vel:" + std::to_string(enemy->enemyAgent.GetSpeed()));
					enemy->enemyAgent.SetDestination(API_Vector3(enemy->basePos.x, gameObject.GetTransform().GetGlobalPosition().y, enemy->basePos.z));
				}

				if (animState != AnimationState::WALK && !enemy->takingDmg && _canWalk && enemy->basePos!=gameObject.GetTransform().GetGlobalPosition())
				{
					animState = AnimationState::WALK;
					animationPlayer.ChangeAnimation(walkAnim);
					animationPlayer.Play();
					//Console::Log("Walk");
				}
				else if (animState != AnimationState::IDLE && !enemy->takingDmg)
				{
					animState = AnimationState::IDLE;
					animationPlayer.ChangeAnimation(idleAnim);
					animationPlayer.Play();
					//Console::Log("Walk");
				}


			}
		}
		break;
		case EnemyRanger::States::FOLLOWING:
		{
			enemy->currentSpeed = enemy->speed * enemy->acceleration * enemy->stunVel * enemy->slowVel/** dt*/;

			LoookAt(target.GetTransform().GetGlobalPosition());

			_agCooldown += dt;

			if (_agCooldown >= _agTime)
			{
				enemy->enemyAgent.SetSpeed(enemy->currentSpeed);
				_agCooldown = 0;
				enemy->enemyAgent.SetDestination(API_Vector3(target.GetTransform().GetGlobalPosition().x, gameObject.GetTransform().GetGlobalPosition().y, target.GetTransform().GetGlobalPosition().z));
			}
			if (animState != AnimationState::RUN && !enemy->takingDmg)
			{
				animState = AnimationState::RUN;
				animationPlayer.ChangeAnimation(runAnim);
				animationPlayer.Play();
				//Console::Log("Walk");
			}
		}
		break;
		case EnemyRanger::States::ATTACKING_FOLLOW:
		{
			enemy->currentSpeed = enemy->speed * enemy->acceleration * enemy->stunVel * enemy->slowVel /** dt*/;

			enemy->enemyAgent.Move();
			_agCooldown += dt;
			if (_agCooldown >= _agTime)
			{
				enemy->enemyAgent.SetSpeed(enemy->currentSpeed);
				_agCooldown = 0;
				enemy->enemyAgent.SetDestination(API_Vector3(target.GetTransform().GetGlobalPosition().x, gameObject.GetTransform().GetGlobalPosition().y, target.GetTransform().GetGlobalPosition().z));
			}

			Attacking(target.GetTransform().GetGlobalPosition(), enemy->enemyRb);
		}
		break;
		case EnemyRanger::States::ATTACKING:
		{
			enemy->enemyAgent.Stop();
			enemy->enemyRb.SetVelocity(0);

			Attacking(target.GetTransform().GetGlobalPosition(), enemy->enemyRb);
		}
		break;
		case EnemyRanger::States::ATTACKING_ESCAPE:
		{
			enemy->currentSpeed = enemy->speed * enemy->acceleration * enemy->stunVel * enemy->slowVel /** dt*/;

			enemy->enemyAgent.Stop();
			enemy->enemyRb.SetVelocity(gameObject.GetTransform().GetBackward() * enemy->currentSpeed * 0.7);

			Attacking(target.GetTransform().GetGlobalPosition(), enemy->enemyRb);
		}
		break;
		case EnemyRanger::States::DYING:
		{
			enemy->_coldAnimDie += dt;
			// enemy->dying = true;
			enemy->enemyAgent.Stop();
			enemy->enemyRb.SetVelocity(0);
			if (enemy->_coldAnimDie < enemy->_tAnimDie)
			{
				if (animState != AnimationState::DIE)
				{
					animState = AnimationState::DIE;
					animationPlayer.ChangeAnimation(dieAnim);
					animationPlayer.Play();
				}
			}
			else
			{
				gameObject.SetActive(false);
			}
		}
		break;
		default:
			break;
		}
		return;
	}
	if (scriptedForQuest)
	{
		enemyGun->Shoot();
		enemy->enemyRb.SetVelocity(0);
	}
}

void EnemyRanger::Seek(float vel, API_Vector3 tarPos, API_RigidBody enemyRb)
{
	if (!enemy->actStun)
	{
		API_Vector3 _bRot = enemy->baseRot;
		API_Vector2 lookDir;
		/* lookDir.x = (point.x - gameObject.GetTransform().GetGlobalPosition().x);
		lookDir.y = (point.z - gameObject.GetTransform().GetGlobalPosition().z);*/
		lookDir.x = (tarPos.x - gameObject.GetTransform().GetLocalPosition().x);
		lookDir.y = (tarPos.z - gameObject.GetTransform().GetLocalPosition().z);

		API_Vector2 normLookDir;
		normLookDir.x = lookDir.x / sqrt(pow(lookDir.x, 2) + pow(lookDir.y, 2));
		normLookDir.y = lookDir.y / sqrt(pow(lookDir.x, 2) + pow(lookDir.y, 2));
		float _angle = 0;
		_angle = atan2(normLookDir.y, normLookDir.x) * RADTODEG - 90.0f;

		gameObject.GetTransform().SetRotation(0, -_angle, 0);
		//gameObject.GetTransform().SetRotation(0 + _bRot.x, -_angle + _bRot.y, 0 + _bRot.z);
	}
	enemyRb.SetVelocity(gameObject.GetTransform().GetForward() * vel);
	//gameObject.GetTransform().Translate(gameObject.GetTransform().GetForward() * vel);

}

void EnemyRanger::Wander(float vel, API_Vector3 point, API_RigidBody enemyRb)
{
	if (!enemy->actStun)
	{
		API_Vector3 _bRot = enemy->baseRot;
		API_Vector2 lookDir;
		/* lookDir.x = (point.x - gameObject.GetTransform().GetGlobalPosition().x);
		lookDir.y = (point.z - gameObject.GetTransform().GetGlobalPosition().z);*/
		lookDir.x = (point.x - gameObject.GetTransform().GetLocalPosition().x);
		lookDir.y = (point.z - gameObject.GetTransform().GetLocalPosition().z);

		API_Vector2 normLookDir;
		normLookDir.x = lookDir.x / sqrt(pow(lookDir.x, 2) + pow(lookDir.y, 2));
		normLookDir.y = lookDir.y / sqrt(pow(lookDir.x, 2) + pow(lookDir.y, 2));
		float _angle = 0;
		_angle = atan2(normLookDir.y, normLookDir.x) * RADTODEG - 90.0f;
		gameObject.GetTransform().SetRotation(0, -_angle, 0);
		//gameObject.GetTransform().SetRotation(0 + _bRot.x, -_angle + _bRot.y, 0 + _bRot.z);
	}
	enemyRb.SetVelocity(gameObject.GetTransform().GetForward() * vel);
	//gameObject.GetTransform().Translate(gameObject.GetTransform().GetForward() * vel);
}

void EnemyRanger::Attacking(API_Vector3 tarPos, API_RigidBody enemyRb)
{
	if (!enemy->actStun)
	{
		API_Vector3 _bRot = enemy->baseRot;
		API_Vector2 lookDir;
		/* lookDir.x = (point.x - gameObject.GetTransform().GetGlobalPosition().x);
		lookDir.y = (point.z - gameObject.GetTransform().GetGlobalPosition().z);*/
		lookDir.x = (tarPos.x - gameObject.GetTransform().GetLocalPosition().x);
		lookDir.y = (tarPos.z - gameObject.GetTransform().GetLocalPosition().z);

		API_Vector2 normLookDir;
		normLookDir.x = lookDir.x / sqrt(pow(lookDir.x, 2) + pow(lookDir.y, 2));
		normLookDir.y = lookDir.y / sqrt(pow(lookDir.x, 2) + pow(lookDir.y, 2));
		float _angle = 0;
		_angle = atan2(normLookDir.y, normLookDir.x) * RADTODEG - 90.0f;
		gameObject.GetTransform().SetRotation(0, -_angle, 0);

		if (enemyGun)
			enemyGun->Shoot();

		if (animState != AnimationState::SHOOT && !enemy->takingDmg)
		{
			animState = AnimationState::SHOOT;
			animationPlayer.ChangeAnimation(aimAnim);
			animationPlayer.Play();
			//Console::Log("Walk");
		}
		//Console::Log(std::to_string(_angle));
	}
}

API_Vector3 EnemyRanger::NormalizeVec3(float x, float y, float z)
{
	float lenght = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));

	x = x / lenght;
	y = y / lenght;
	z = z / lenght;
	return (x, y, z);
}

float EnemyRanger::Lerp(float a, float b, float time)
{
	return a + time * (b - a);
}

void EnemyRanger::HitAnimation()
{
	if (animState != AnimationState::HITTED)
	{
		animState = AnimationState::HITTED;
		animationPlayer.ChangeAnimation(hitAnim);
		animationPlayer.Play();
	}

}

void EnemyRanger::LoookAt(API_Vector3 target)
{
	if (!enemy->actStun)
	{
		API_Vector2 lookDir;
		lookDir.x = (target.x - gameObject.GetTransform().GetGlobalPosition().x);
		lookDir.y = (target.z - gameObject.GetTransform().GetGlobalPosition().z);


		API_Vector2 normLookDir;
		normLookDir.x = lookDir.x / sqrt(pow(lookDir.x, 2) + pow(lookDir.y, 2));
		normLookDir.y = lookDir.y / sqrt(pow(lookDir.x, 2) + pow(lookDir.y, 2));
		float _angle = 0;
		_angle = atan2(normLookDir.y, normLookDir.x) * RADTODEG - 90.0f;
		gameObject.GetTransform().SetRotation(0, -_angle, 0);
	}
}