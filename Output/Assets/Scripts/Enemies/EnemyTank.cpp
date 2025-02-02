#include "EnemyTank.h"
HELLO_ENGINE_API_C EnemyTank* CreateEnemyTank(ScriptToInspectorInterface* script)
{
	EnemyTank* classInstance = new EnemyTank();

	script->AddDragFloat("Init Pos Range", &classInstance->initPosRange);

	script->AddDragFloat("Max Shield", &classInstance->maxShield);
	script->AddDragFloat("Current Shield", &classInstance->currentShield);
	script->AddDragFloat("Recover Shield Time", &classInstance->recoverShieldTime);
	script->AddDragFloat("Hp/s", &classInstance->healthRestorePerSecond);
	script->AddDragFloat("Hp/s Cooldown", &classInstance->healthRestoreCooldown);

	script->AddDragFloat("Max Shield", &classInstance->maxShield);

	script->AddDragFloat("Detection Distance", &classInstance->detectionDistance);

	script->AddDragInt("Gun Type(0Semi/1Burst/2Shotgun)", &classInstance->gunType);
	script->AddDragBoxGameObject("Enemy gun", &classInstance->gunObj);

	//script->AddDragBoxGameObject("Target", &classInstance->target);
	//script->AddDragBoxRigidBody("Action Rb zone", &classInstance->zoneRb);
	script->AddDragBoxGameObject("Action  zone", &classInstance->zone);
	script->AddDragFloat("Attack Range", &classInstance->attackRange);
	script->AddDragFloat("Approximate Range", &classInstance->approximateRange);
	script->AddDragFloat("Separate Range", &classInstance->separateRange);

	script->AddDragFloat("Return Distance", &classInstance->returnToZoneDistance);

	script->AddCheckBox("Is Protecting", &classInstance->isProtectingAlly);

	script->AddDragBoxAnimationResource("Idle Animation", &classInstance->idleAnim);
	script->AddDragBoxAnimationResource("Walk Animation", &classInstance->walkAnim);
	script->AddDragBoxAnimationResource("Die Animation", &classInstance->dieAnim);

	script->AddDragBoxAnimationPlayer("Animation", &classInstance->animationPlayer);
	script->AddDragBoxParticleSystem("Heal Particles", &classInstance->healParticles);
	//script->AddDragBoxShaderComponent("Material", &classInstance->material);

	return classInstance;
}

void EnemyTank::Start()
{
	Game::FindGameObjectsWithTag("Player", &target, 1);
	
	zoneRb = zone.GetRigidBody();
	isReturning = false;
	currentShield = maxShield;
	isRecoveringShield = false;
	isRestoringHealth = false;
	shieldRecoverCounter = 0;

	hasToBlinkHealing = false;
	hasToBlinkShield = false;

	nonBlinkingTimer = 0.0f;
	blinkingTimer = 0.0f;

	nonBlinkingTime = 1;
	blinkingTime = 3;

	fadeInTime = blinkingTime / 2;
	fadeOutTime = blinkingTime / 2;

	

	isBlinking = false;

	isProtectingAlly = false;

	_fadeInCooldown_blink = _fadeOutCooldown_blink = 0;
	fading_blink = true;

	r_blink = 0;
	g_blink = 0;
	b_blink = 1;

	animationPlayer.ChangeAnimation(idleAnim);
	animationPlayer.Play();
	animationPlayer.GetGameObject().GetTransform().SetScale(1.5, 1.5, 1.5);
	animState = AnimStates::IDLE;


	enemyScript = (Enemy*)gameObject.GetScript("Enemy");
	if (enemyScript) 
	{
		if (enemyScript->hasShield == false) {
			enemyScript->hasShield = true;
		}
		if (enemyScript->isTank == false) {
			enemyScript->isTank = true;
		}

		material = enemyScript->enemyShader;


		//

		blinkingTimer_Shield = 0;
		blinkingTime_Shield = enemyScript->_tHitColor;

		fadeInTime_Shield = blinkingTime_Shield / 2;
		fadeOutTime_Shield = blinkingTime_Shield / 2;

		fading_blink_shield = true;
		isFirstHitFrame = true;
		//
	}

	initialPosition = gameObject.GetTransform().GetGlobalPosition();
	actionZone = zoneRb.GetGameObject();

	switch (gunType)
	{
	case 0:
		enemyGun = (EnemyGun*)gunObj.GetScript("EnemyAutomatic");
		break;
	case 1:
		enemyGun = (EnemyGun*)gunObj.GetScript("EnemyBurst");
		break;
	case 2:
		enemyGun = (EnemyGun*)gunObj.GetScript("EnemyShotgun");
		break;
	default:
		break;
	}

	state = States::WANDERING;
	
}
void EnemyTank::Update()
{
	if (enemyScript)
	{
		if (enemyScript->dying)DieTank();

		if (state == States::TARGETING)
		{
			enemyScript->targeting = true;
		}
		else
		{
			enemyScript->targeting = false;
		}

		if (!enemyScript->dying)
		{

			Recovering();

			CheckDistance();

			if (isReturning == false) {
				switch (state)
				{
				case States::WANDERING:
					Wander();
					break;
				case States::TARGETING:
					Seek();
					break;
				}
			}
			else {
				ReturnToZone();
			}

			if (enemyScript->_hitShader == false)
			{
				isFirstHitFrame = true;
				blinkingTimer_Shield = 0;
			}
			else 
			{
				if (isFirstHitFrame == true) 
				{
					blinkingTimer = 0.0f;
					nonBlinkingTimer = 0.0f;
					_r = 1;
					_g = 1;
					_b = 1;
					fading_blink = true;
					_fadeOutCooldown_blink = 0;
					_fadeInCooldown_blink = 0;
					isFirstHitFrame = false;
					blinkingTimer_Shield = 0;
				}
			}
			
			if (hasToBlinkShield && enemyScript->_hitShader == false)
			{
				BlinkShield();
			}
			else if (hasToBlinkHealing && enemyScript->_hitShader == false)
			{
				BlinkHealth();
			}
			else if (enemyScript->_hitShader == true)
			{
				if (currentShield > 0) {

					/*if (blinkingTimer_Shield <= blinkingTime_Shield) 
					{
						r_blink = 0;
						g_blink = 0;
						b_blink = 1;
						fading_blink ? FadeIn() : FadeOut();
						material.SetColor(_r, _g, _b, 255);
						blinkingTimer_Shield += Time::GetDeltaTime();
					}*/
				}
				else
				{
					
				}
			}
			else 
			{
				blinkingTimer = 0.0f;
				nonBlinkingTimer = 0.0f;
				_r = 1;
				_g = 1;
				_b = 1;
				fading_blink = true;
				_fadeOutCooldown_blink = 0;
				_fadeInCooldown_blink = 0;

				if (enemyScript->_hitShader == false) 
				{
					material.SetColor(1, 1, 1, 1);
				}
			}
		}
	}
}

void EnemyTank::FadeInShield()
{
	if (_fadeInCooldown_blink < fadeInTime_Shield)
	{
		_fadeInCooldown_blink += Time::GetDeltaTime();
		_r = Lerp(1, r_blink, _fadeInCooldown_blink / fadeInTime_Shield);
		_g = Lerp(1, g_blink, _fadeInCooldown_blink / fadeInTime_Shield);
		_b = Lerp(1, b_blink, _fadeInCooldown_blink / fadeInTime_Shield);

	}
	else if (_fadeInCooldown_blink >= fadeInTime_Shield)
	{
		fading_blink = false;
		_fadeInCooldown_blink = 0;
	}
}

void EnemyTank::FadeOutShield()
{
	if (_fadeOutCooldown_blink < fadeOutTime_Shield)
	{
		_fadeOutCooldown_blink += Time::GetDeltaTime();
		_r = Lerp(r_blink, 1, _fadeOutCooldown_blink / fadeOutTime_Shield);
		_g = Lerp(g_blink, 1, _fadeOutCooldown_blink / fadeOutTime_Shield);
		_b = Lerp(b_blink, 1, _fadeOutCooldown_blink / fadeOutTime_Shield);

	}
	else if (_fadeOutCooldown_blink >= fadeOutTime_Shield)
	{
		fading_blink = true;
		_fadeOutCooldown_blink = 0;
	}
}

void EnemyTank::FadeIn()
{
	if (_fadeInCooldown_blink < fadeInTime)
	{
		_fadeInCooldown_blink += Time::GetDeltaTime();
		_r = Lerp(1, r_blink, _fadeInCooldown_blink / fadeInTime);
		_g = Lerp(1, g_blink, _fadeInCooldown_blink / fadeInTime);
		_b = Lerp(1, b_blink, _fadeInCooldown_blink / fadeInTime);

	}
	else if (_fadeInCooldown_blink >= fadeInTime)
	{
		fading_blink = false;
		_fadeInCooldown_blink = 0;
	}
}

void EnemyTank::FadeOut()
{
	if (_fadeOutCooldown_blink < fadeOutTime)
	{
		_fadeOutCooldown_blink += Time::GetDeltaTime();
		_r = Lerp(r_blink, 1, _fadeOutCooldown_blink / fadeOutTime);
		_g = Lerp(g_blink, 1, _fadeOutCooldown_blink / fadeOutTime);
		_b = Lerp(b_blink, 1, _fadeOutCooldown_blink / fadeOutTime);

	}
	else if (_fadeOutCooldown_blink >= fadeOutTime)
	{
		fading_blink = true;
		_fadeOutCooldown_blink = 0;
	}
}

float EnemyTank::Lerp(float a, float b, float time)
{
	return a + time * (b - a);
}

void EnemyTank::BlinkShield() 
{
	r_blink = 0;
	g_blink = 0;
	b_blink = 1;

	if (isBlinking) 
	{
		fading_blink ? FadeIn() : FadeOut();
		material.SetColor(_r, _g, _b, 255);

		blinkingTimer += Time::GetDeltaTime();
		nonBlinkingTimer = 0.0f;
		
		if (blinkingTimer >= blinkingTime) 
		{
			_r = 1;
			_g = 1;
			_b = 1;
			material.SetColor(1, 1, 1, 1);
			isBlinking = false;
		}
	}
	else 
	{
		nonBlinkingTimer += Time::GetDeltaTime();
		blinkingTimer = 0.0f;
		
		if (nonBlinkingTimer >= nonBlinkingTime)
		{
			fading_blink = true;
			_fadeOutCooldown_blink = 0;
			_fadeInCooldown_blink = 0;
			isBlinking = true;
		}
	}
}

void EnemyTank::BlinkHealth()
{
	r_blink = 0;
	g_blink = 1;
	b_blink = 0;

	if (isBlinking)
	{
		fading_blink ? FadeIn() : FadeOut();
		material.SetColor(_r, _g, _b, 255);

		blinkingTimer += Time::GetDeltaTime();
		nonBlinkingTimer = 0.0f;

		if (blinkingTimer >= blinkingTime)
		{
			_r = 1;
			_g = 1;
			_b = 1;
			material.SetColor(1, 1, 1, 1);
			isBlinking = false;
		}
	}
	else
	{
		nonBlinkingTimer += Time::GetDeltaTime();
		blinkingTimer = 0.0f;

		if (nonBlinkingTimer >= nonBlinkingTime)
		{
			fading_blink = true;
			_fadeOutCooldown_blink = 0;
			_fadeInCooldown_blink = 0;
			isBlinking = true;
		}
	}

	
}

void EnemyTank::ReturnToZone() {

	float xDif = gameObject.GetTransform().GetGlobalPosition().x - actionZone.GetTransform().GetGlobalPosition().x;
	float zDif = gameObject.GetTransform().GetGlobalPosition().z - actionZone.GetTransform().GetGlobalPosition().z;

	if ((abs(xDif) > initPosRange) || (abs(zDif) > initPosRange)) {
		enemyScript->currentSpeed = enemyScript->speed * enemyScript->acceleration * enemyScript->stunVel * enemyScript->slowVel;
		MoveToDirection(actionZone.GetTransform().GetGlobalPosition().x, actionZone.GetTransform().GetGlobalPosition().z, enemyScript->currentSpeed);
	}
	else {
		enemyScript->enemyRb.SetVelocity(gameObject.GetTransform().GetForward() * 0);
	}

	float enemyZoneDistance = gameObject.GetTransform().GetGlobalPosition().Distance(actionZone.GetTransform().GetGlobalPosition());
	if (enemyZoneDistance < (zoneRb.GetRadius() / 2.f) - returnToZoneDistance)
	{
		isReturning = false;
	}
}

void EnemyTank::CheckDistance() {

	float targetZoneDistance = target.GetTransform().GetGlobalPosition().Distance(actionZone.GetTransform().GetGlobalPosition());
	float enemyZoneDistance = gameObject.GetTransform().GetGlobalPosition().Distance(actionZone.GetTransform().GetGlobalPosition());
	if (targetZoneDistance > zoneRb.GetRadius() / 2.f) {
		state = States::WANDERING;
	}
	else if (enemyZoneDistance > zoneRb.GetRadius() / 2.f) {
		state = States::WANDERING;
		isReturning = true;
	}
	else {
		targetDistance = gameObject.GetTransform().GetGlobalPosition().Distance(target.GetTransform().GetGlobalPosition());
		if (targetDistance < detectionDistance) {
			state = States::TARGETING;
		}
		else {
			state = States::WANDERING;
		}
	}
}

void EnemyTank::Wander() {

	float xDif = gameObject.GetTransform().GetGlobalPosition().x - initialPosition.x;
	float zDif = gameObject.GetTransform().GetGlobalPosition().z - initialPosition.z;

	if ((abs(xDif) > initPosRange) || (abs(zDif) > initPosRange)) {

		enemyScript->currentSpeed = enemyScript->speed * enemyScript->stunVel * enemyScript->slowVel;
		MoveToDirection(initialPosition.x, initialPosition.z, enemyScript->currentSpeed);

		if (animState != AnimStates::WALK)
		{
			animationPlayer.ChangeAnimation(walkAnim);
			animationPlayer.Play();
			animState = AnimStates::WALK;
		}
	}
	else {
		
		enemyScript->enemyRb.SetVelocity(gameObject.GetTransform().GetForward() * 0);
		if (animState != AnimStates::IDLE)
		{
			animationPlayer.ChangeAnimation(idleAnim);
			animationPlayer.Play();
			animState = AnimStates::IDLE;
		}
	}
}

void EnemyTank::Seek() 
{
	if (!enemyScript->actStun)
	{
		if (targetDistance > approximateRange) {
			
			enemyScript->currentSpeed = enemyScript->speed * enemyScript->acceleration * enemyScript->stunVel * enemyScript->slowVel;

			float dirToPlayer[2];
			dirToPlayer[0] = target.GetTransform().GetGlobalPosition().x;
			dirToPlayer[1] = target.GetTransform().GetGlobalPosition().z;

			float dirToPlayerNorm[2];
			dirToPlayerNorm[0] = dirToPlayer[0] / sqrt(pow(dirToPlayer[0], 2) + pow(dirToPlayer[1], 2));
			dirToPlayerNorm[1] = dirToPlayer[1] / sqrt(pow(dirToPlayer[0], 2) + pow(dirToPlayer[1], 2));

			if (isProtectingAlly == true)
			{
				//A is protrected enemy
				//B is player
				//C is tank

				float A[2];
				A[0] = protectedEnemy.GetTransform().GetGlobalPosition().x;
				A[1] = protectedEnemy.GetTransform().GetGlobalPosition().z;

				float B[2];
				B[0] = target.GetTransform().GetGlobalPosition().x;
				B[1] = target.GetTransform().GetGlobalPosition().z;

				float C[2];
				C[0] = gameObject.GetTransform().GetGlobalPosition().x;
				C[1] = gameObject.GetTransform().GetGlobalPosition().z;

				//1 - Ally to player
				float AB[2];
				AB[0] = B[0] - A[0];
				AB[1] = B[1] - A[1];

				//2 - Ally to tank
				float AC[2];
				AC[0] = C[0] - A[0];
				AC[1] = C[1] - A[1];
				
				//Tank to player
				float CB[2] = {0,0};
				CB[0] = B[0] - C[0];
				CB[1] = B[1] - C[1];

				//3
				float CrossProductABAC = ((AB[0] * AC[1]) - (AB[1] * AC[0]));

				float dirToAlly[2] = { 0,0 };
				bool isInZone = false;
				//tank is on the right - need anti schedule rotated vector
				if (CrossProductABAC > 10) {
					dirToAlly[0] = AB[1];
					dirToAlly[1] = -AB[0];
				}
				//tank is on the right - need schedule rotated vector
				else if (CrossProductABAC < -10) {
					dirToAlly[0] = -AB[1];
					dirToAlly[1] = AB[0];
				}
				else 
				{
					isInZone = true;
				}

				float dirToAllyAndPlayer[2] = {0,0};
				if (isInZone == false) 
				{
					float dirToAllyNorm[2];
					dirToAllyNorm[0] = dirToAlly[0] / sqrt(pow(dirToAlly[0], 2) + pow(dirToAlly[1], 2));
					dirToAllyNorm[1] = dirToAlly[1] / sqrt(pow(dirToAlly[0], 2) + pow(dirToAlly[1], 2));

					float CBNorm[2];
					CBNorm[0] = CB[0] / sqrt(pow(CB[0], 2) + pow(CB[1], 2));
					CBNorm[1] = CB[1] / sqrt(pow(CB[0], 2) + pow(CB[1], 2));

					dirToAllyAndPlayer[0] = (dirToAllyNorm[0] + CBNorm[0]) / 2;
					dirToAllyAndPlayer[1] = (dirToAllyNorm[1] + CBNorm[1]) / 2;
				}
				else 
				{
					float CBNorm[2];
					CBNorm[0] = CB[0] / sqrt(pow(CB[0], 2) + pow(CB[1], 2));
					CBNorm[1] = CB[1] / sqrt(pow(CB[0], 2) + pow(CB[1], 2));

					
					dirToAllyAndPlayer[0] = (dirToAlly[0] + CBNorm[0]) / 2;
					dirToAllyAndPlayer[1] = (dirToAlly[1] + CBNorm[1]) / 2;
				}

				float dirToAllyAndPlayerNorm[2];
				dirToAllyAndPlayerNorm[0] = dirToAllyAndPlayer[0] / sqrt(pow(dirToAllyAndPlayer[0], 2) + pow(dirToAllyAndPlayer[1], 2));
				dirToAllyAndPlayerNorm[1] = dirToAllyAndPlayer[1] / sqrt(pow(dirToAllyAndPlayer[0], 2) + pow(dirToAllyAndPlayer[1], 2));

					float _angle = 0;
					_angle = atan2(dirToAllyAndPlayerNorm[1], dirToAllyAndPlayerNorm[0]) * RADTODEG - 90.0f;

					gameObject.GetTransform().SetRotation(0, -_angle, 0);
					enemyScript->enemyRb.SetVelocity(gameObject.GetTransform().GetForward() * enemyScript->currentSpeed);

					API_Vector2 setRotToPlayer;
					setRotToPlayer.x = (target.GetTransform().GetGlobalPosition().x - gameObject.GetTransform().GetGlobalPosition().x);
					setRotToPlayer.y = (target.GetTransform().GetGlobalPosition().z - gameObject.GetTransform().GetGlobalPosition().z);

					API_Vector2 normSetRotToPlayer;
					normSetRotToPlayer.x = setRotToPlayer.x / sqrt(pow(setRotToPlayer.x, 2) + pow(setRotToPlayer.y, 2));
					normSetRotToPlayer.y = setRotToPlayer.y / sqrt(pow(setRotToPlayer.x, 2) + pow(setRotToPlayer.y, 2));

					_angle = 0;
					_angle = atan2(normSetRotToPlayer.y, normSetRotToPlayer.x) * RADTODEG - 90.0f;

					gameObject.GetTransform().SetRotation(0, -_angle, 0);

					if (animState != AnimStates::WALK)
					{
						animationPlayer.ChangeAnimation(walkAnim);
						animationPlayer.Play();
						animState = AnimStates::WALK;
					}
			}
			else
			{
				//B is player
				//C is tank

				float B[2];
				B[0] = target.GetTransform().GetGlobalPosition().x;
				B[1] = target.GetTransform().GetGlobalPosition().z;

				float C[2];
				C[0] = gameObject.GetTransform().GetGlobalPosition().x;
				C[1] = gameObject.GetTransform().GetGlobalPosition().z;

				//Tank to player
				float CB[2] = { 0,0 };
				CB[0] = B[0] - C[0];
				CB[1] = B[1] - C[1];

				MoveToDirection(B[0], B[1], enemyScript->currentSpeed);

				if (animState != AnimStates::WALK)
				{
					animationPlayer.ChangeAnimation(walkAnim);
					animationPlayer.Play();
					animState = AnimStates::WALK;
				}
			}
		}
		else if(targetDistance < separateRange) {
			enemyScript->currentSpeed = enemyScript->speed * enemyScript->acceleration * enemyScript->stunVel * enemyScript->slowVel;
			EscapeFromDirection(target.GetTransform().GetGlobalPosition().x, target.GetTransform().GetGlobalPosition().z, enemyScript->currentSpeed);
		}
		else {
			enemyScript->enemyRb.SetVelocity(gameObject.GetTransform().GetForward() * 0);
		}

		if (targetDistance < attackRange) {
			Attack();
		}
		
	}
}

void EnemyTank::ProtectEnemy() 
{

	if (isProtectingAlly == false) 
	{
		return; 
	}

	//A is protrected enemy
	//B is player
	//C is tank

	float A[2];
	A[0] = protectedEnemy.GetTransform().GetGlobalPosition().x;
	A[1] = protectedEnemy.GetTransform().GetGlobalPosition().z;

	float B[2];
	B[0] = target.GetTransform().GetGlobalPosition().x;
	B[1] = target.GetTransform().GetGlobalPosition().z;

	float C[2];
	C[0] = gameObject.GetTransform().GetGlobalPosition().x;
	C[1] = gameObject.GetTransform().GetGlobalPosition().z;

	//1
	float AB[2];
	AB[0] = B[0] - A[0];
	AB[1] = B[1] - A[1];

	//2
	float AC[2];
	AC[0] = C[0] - A[0];
	AC[1] = C[1] - A[1];

	//3
	float CrossProductABAC = ((AB[0] * AC[1]) - (AB[1] * AC[0]));

	float rotatedVec[2] = { 0,0 };
	//tank is on the right - need anti schedule rotated vector
	if (CrossProductABAC > 0) {
		rotatedVec[0] = AB[1];
		rotatedVec[1] = -AB[0];

		MoveToDirection(rotatedVec[0], rotatedVec[1], enemyScript->currentSpeed);

	}
	//tank is on the right - need schedule rotated vector
	else if (CrossProductABAC < 0) {
		rotatedVec[0] = -AB[1];
		rotatedVec[1] = AB[0];

		MoveToDirection(rotatedVec[0], rotatedVec[1], enemyScript->currentSpeed);
	
	}

	if (animState!= AnimStates::WALK) 
	{
		animationPlayer.ChangeAnimation(walkAnim);
		animationPlayer.Play();
		animState = AnimStates::WALK;
	}
	
	
}

void EnemyTank::Attack() 
{
	if (enemyGun != nullptr)
	{
		enemyGun->Shoot();
	}
}

void EnemyTank::MoveToDirection(float pointX, float pointY, float velocity)
{
	API_Vector2 lookDir;
	lookDir.x = (pointX - gameObject.GetTransform().GetLocalPosition().x);
	lookDir.y = (pointY - gameObject.GetTransform().GetLocalPosition().z);

	API_Vector2 normLookDir;
	normLookDir.x = lookDir.x / sqrt(pow(lookDir.x, 2) + pow(lookDir.y, 2));
	normLookDir.y = lookDir.y / sqrt(pow(lookDir.x, 2) + pow(lookDir.y, 2));

	float _angle = 0;
	_angle = atan2(normLookDir.y, normLookDir.x) * RADTODEG - 90.0f;

	gameObject.GetTransform().SetRotation(0, -_angle, 0);
	enemyScript->enemyRb.SetVelocity(gameObject.GetTransform().GetForward() * velocity);
}

void EnemyTank::EscapeFromDirection(float pointX, float pointY, float velocity)
{
	API_Vector2 lookDir;
	lookDir.x = (pointX - gameObject.GetTransform().GetLocalPosition().x);
	lookDir.y = (pointY - gameObject.GetTransform().GetLocalPosition().z);

	API_Vector2 normLookDir;
	normLookDir.x = lookDir.x / sqrt(pow(lookDir.x, 2) + pow(lookDir.y, 2));
	normLookDir.y = lookDir.y / sqrt(pow(lookDir.x, 2) + pow(lookDir.y, 2));

	float _angle = 0;
	_angle = atan2(normLookDir.y, normLookDir.x) * RADTODEG - 90.0f;

	gameObject.GetTransform().SetRotation(0, -_angle, 0);
	enemyScript->enemyRb.SetVelocity(gameObject.GetTransform().GetBackward() * velocity);
}

void EnemyTank::Recovering()
{
	//Shield
	if (currentShield <= 0) {
		if (isRecoveringShield == false) {
			isRecoveringShield = true;
		}
		//hasToBlinkShield = false;
	}
	else
	{
		//hasToBlinkShield = true;
	}

	if (isRecoveringShield == true) {
		shieldRecoverCounter += Time::GetDeltaTime();
		if (shieldRecoverCounter >= recoverShieldTime) {
			currentShield = maxShield;
			isRecoveringShield = false;
			shieldRecoverCounter = 0;
		}
	}

	//Health
	if (enemyScript->currentHp < enemyScript->maxHp) {
		if (hasToRestoreHealth == false) {
			hasToRestoreHealth = true;
		}
	}
	else 
	{
		hasToBlinkHealing = false;
	}

	if (hasToRestoreHealth == true) {
		healthRestoreCounter += Time::GetDeltaTime();
		if (healthRestoreCounter >= healthRestoreCooldown) {
			if (isRestoringHealth == false) {
				isRestoringHealth = true;
				healParticles.Play();
			}
		}
	}

	if (isRestoringHealth == true) {

		hasToBlinkHealing = true;

		if (enemyScript->currentHp + Time::GetDeltaTime() * healthRestorePerSecond < enemyScript->maxHp) {
			enemyScript->currentHp += Time::GetDeltaTime() * healthRestorePerSecond;
		}
		else {
			enemyScript->currentHp = enemyScript->maxHp;
			hasToRestoreHealth = false;
			isRestoringHealth = false;
			healthRestoreCounter = 0;
			healParticles.Stop();
		}
	}
}

float EnemyTank::TakeDamageTank(float life, float damage)
{
	float prevShield = currentShield;

	currentShield -= damage;

	if (currentShield < 0) {

		life -= std::abs(prevShield - damage);
		currentShield = 0;
		hasToRestoreHealth = false;
		isRestoringHealth = false;
		healthRestoreCounter = 0;
		healParticles.Stop();
		hasToBlinkHealing = false;
	}

	return life;
}

void EnemyTank::DieTank()
{
	healParticles.Stop();
	enemyScript->_coldAnimDie += Time::GetDeltaTime();

	enemyScript->enemyRb.SetVelocity(0);
	if (enemyScript->_coldAnimDie < enemyScript->_tAnimDie)
	{
		if (animState != AnimStates::DIE)
		{
			animState = AnimStates::DIE;
			animationPlayer.ChangeAnimation(dieAnim);
			animationPlayer.Play();
		}
	}
	else if (enemyScript->_coldAnimDie >= enemyScript->_tAnimDie)
	{
		gameObject.SetActive(false);
	}
}