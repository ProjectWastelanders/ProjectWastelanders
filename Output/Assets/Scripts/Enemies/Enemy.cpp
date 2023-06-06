#include "Enemy.h"
#include "../Player/PlayerStats.h"
#include "../Shooting/Projectile.h"
#include "EnemyTank.h"
#include "EnemyMeleeMovement.h"
#include "EnemyRanger.h"
#include "../Shooting/StickBomb.h"
#include "../Quests/Secondary_Quests/EnemyDieEvent.h"

HELLO_ENGINE_API_C Enemy* CreateEnemy(ScriptToInspectorInterface* script)
{
    Enemy* classInstance = new Enemy();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragFloat("Max Health", &classInstance->maxHp);
    script->AddDragFloat("Current Helath", &classInstance->currentHp);
    script->AddDragFloat("Start Resistance", &classInstance->minResistence);
    script->AddDragFloat("Resistance", &classInstance->maxResistance);
    script->AddDragFloat("Speed", &classInstance->speed);
    script->AddDragFloat("Acceleration", &classInstance->acceleration);
    script->AddDragFloat("Current speed", &classInstance->currentSpeed);
    script->AddDragBoxGameObject("Enemy Manager", &classInstance->enemyDropManagerGO);
    //script->AddDragBoxRigidBody("Enemy RigidBody", &classInstance->enemyRb);
    //script->AddDragBoxParticleSystem("Hit particle system", &classInstance->hitParticles);
    script->AddCheckBox("Has Shield", &classInstance->hasShield);
    script->AddDragBoxShaderComponent("Color hit", &classInstance->enemyShader);
    script->AddDragFloat("Time hit color", &classInstance->_tHitColor);
    script->AddDragBoxGameObject("Bomb", &classInstance->bomb);
    script->AddDragBoxTextureResource("Texture Bomb 1", &classInstance->textureBomb[0]);
    script->AddDragBoxTextureResource("Texture Bomb 2", &classInstance->textureBomb[1]);
    script->AddDragBoxTextureResource("Texture Bomb 3", &classInstance->textureBomb[2]);
    script->AddDragBoxTextureResource("Texture Bomb 4", &classInstance->textureBomb[3]);
    script->AddDragBoxTextureResource("Texture Bomb 5", &classInstance->textureBomb[4]);
    script->AddDragBoxTextureResource("Texture Bomb 6", &classInstance->textureBomb[5]);
    script->AddDragFloat("Ult Gain", &classInstance->ultGain);
    // script->AddDragBoxRigidBody("RB", &classInstance->enemyRb);
     //script->AddCheckBox("damagessssss", &classInstance->takingDmg);
    script->AddCheckBox("Static Wander", &classInstance->staticWand);
    return classInstance;
}

void Enemy::Start()
{
    enemyDropManager = (EnemyDropManager*)enemyDropManagerGO.GetScript("EnemyDropManager");

    EnemyMeleeMovement* meleeScript = (EnemyMeleeMovement*)gameObject.GetScript("EnemyMeleeMovement");
    EnemyRanger* rangeScript = (EnemyRanger*)gameObject.GetScript("EnemyRanger");
    EnemyTank* tankScript = (EnemyTank*)gameObject.GetScript("EnemyTank");

    API_GameObject player;
    Game::FindGameObjectsWithTag("Player", &player, 1);
    playerStats = (PlayerStats*)player.GetScript("PlayerStats");

    currentHp = maxHp;
    currentResistance = minResistence;
    actSlow = actStun = false;
    baseRot = gameObject.GetTransform().GetGlobalRotation();
    basePos = gameObject.GetTransform().GetGlobalPosition();
    slowVel = stunVel = 1;

    enemyRb = gameObject.GetRigidBody();

    _coldSlow = _coldStun = _coldAnimDie = _coldAnimHit = 0;
    takingDmg = dying = false;

    if (meleeScript)
    {
        _tAnimHit = 1.06f;
        _tAnimDie = 1.7f;
    }
    else if (rangeScript)
    {
        _tAnimHit = 0.83f;
        _tAnimDie = 1.0f;
    }
    else if (tankScript)
    {
        _tAnimDie = 1.43f;
    }

    shotgunLevel = API_QuickSave::GetInt("shotgun_level");

    initialPos.x = gameObject.GetTransform().GetGlobalPosition().x;
    initialPos.z = gameObject.GetTransform().GetGlobalPosition().z;
    enemyAgent = gameObject.GetAgent();
}

void Enemy::Update()
{
    if (actSlow)
        EnemySlow(_qSlow, _tSlow);

    if (actStun)
        EnemyStun(_tStun);

    //burn
    if (burnTime > 3.0f)
        TakeDamage(30.0f * Time::GetDeltaTime(), 0.0f);

    if (resetBurn > 0.0f)
    {
        resetBurn -= Time::GetDeltaTime();
        if (resetBurn <= 0.0f)
        {
            resetBurn = 0.0f;
            burnTime -= Time::GetDeltaTime();
        }
    }

    if (takingDmg && !dying)
        TakingDmgState();

    /*if (Input::GetKey(KeyCode::KEY_J) == KeyState::KEY_DOWN)
    {
	    EnemyRanger* rangeScript = (EnemyRanger*)gameObject.GetScript("EnemyRanger");
	    EnemyMeleeMovement* meleeScript = (EnemyMeleeMovement*)gameObject.GetScript("EnemyMeleeMovement");
	    EnemyTank* tankScript = (EnemyTank*)gameObject.GetScript("EnemyTank");
	    if (meleeScript ||rangeScript || tankScript)
	    {
	     currentHp = 0;
	     Die();
	    }
    }*/
    if (_hitShader)
    {
        _coldHitColor += Time::GetDeltaTime();
        if (_coldHitColor >= _tHitColor)
        {
            _coldHitColor = 0;
            // enemyShader.SetColor(255, 255, 255, 255);
            _hitShader = false;
        }
        else
        {
            enemyShader.SetColor(1.1, 0, 0, 0.5);
        }
    }
    else
    {
        enemyShader.SetColor(1, 1, 1, 255);
    }
}

void Enemy::TakingDmgState()
{
    _coldAnimHit += Time::GetDeltaTime();

    if (_coldAnimHit < _tAnimHit)
    {
        EnemyMeleeMovement* meleeScript = (EnemyMeleeMovement*)gameObject.GetScript("EnemyMeleeMovement");
        EnemyRanger* rangeScript = (EnemyRanger*)gameObject.GetScript("EnemyRanger");

        if (meleeScript)
            meleeScript->HitAnim();

        else if (rangeScript)
            rangeScript->HitAnimation();

        enemyRb.SetVelocity(0);
    }
    else
    {
        takingDmg = false;
        _coldAnimHit = 0.0f;
    }
}

void Enemy::TakeDamage(float damage, float resistanceDamage)
{

    isHit = true;
    enemyShader.SetColor(255, 0, 0, 0.5);
    if (!_hitShader) _hitShader = true;

    if (currentHp <= 0.0f)
        return;

    if (!hasShield)
    {
        // Health damage
        EnemyMeleeMovement* meleeScript = (EnemyMeleeMovement*)gameObject.GetScript("EnemyMeleeMovement");
        if (meleeScript)
        {
            if (!meleeScript->dashing)
            {
                currentHp -= damage;
                //takingDmg = true;
                /*EnemyMeleeMovement* meleeScript = (EnemyMeleeMovement*)gameObject.GetScript("EnemyMeleeMovement");
                if (meleeScript)
                {
	                meleeScript->HitAnim();
                }*/

            }
        }
        else
        {
            currentHp -= damage;
            //takingDmg = true;
            /*EnemyRanger* rangeScript = (EnemyRanger*)gameObject.GetScript("EnemyRanger");
            if (rangeScript)
            {
	            rangeScript->HitAnimation();
            }*/
        }

    }
    else
    {
        EnemyTank* tankScript = (EnemyTank*)gameObject.GetScript("EnemyTank");
        if (tankScript)
            currentHp = tankScript->TakeDamageTank(currentHp, damage);
    }

    if (currentHp <= 0)
    {
        currentHp = 0;
        /*hitParticles.Stop();
        hitParticles.StopEmitting();
        hitParticles.Pause();*/
        Die();
    }

    // Resistance damage
    currentResistance -= resistanceDamage;
    if (currentResistance <= 0)
    {
        currentResistance = maxResistance;
        EnemyMeleeMovement* meleeScript = (EnemyMeleeMovement*)gameObject.GetScript("EnemyMeleeMovement");
        EnemyRanger* rangeScript = (EnemyRanger*)gameObject.GetScript("EnemyRanger");

        meleeScript || rangeScript ? takingDmg = true : takingDmg = false;
        // reaction
    }

    /*hitParticles.Play();*/
}

void Enemy::Die()
{
    EnemyMeleeMovement* meleeScript = (EnemyMeleeMovement*)gameObject.GetScript("EnemyMeleeMovement");
    EnemyRanger* rangeScript = (EnemyRanger*)gameObject.GetScript("EnemyRanger");
    EnemyTank* tankScript = (EnemyTank*)gameObject.GetScript("EnemyTank");
    // some animation
    API_Vector3 dropPos = { gameObject.GetTransform().GetGlobalPosition().x, gameObject.GetTransform().GetGlobalPosition().y + 1.0f, gameObject.GetTransform().GetGlobalPosition().z };
    if (enemyDropManager != nullptr)enemyDropManager->SpinDropRate(dropPos);

    //hitParticles.StopEmitting();
    if (!meleeScript && !rangeScript && !tankScript)
    {
        gameObject.SetActive(false);
    }
    else
    {
        dying = true;
    }

    EnemyDieEvent::enemyDead();

    // add player ult
    if (playerStats && playerStats->ultPercentage < 100.0f)
    {
        playerStats->ultPercentage += ultGain;
        if (playerStats->ultPercentage > 100.0f) playerStats->ultPercentage = 100.0f;
    }

    //gameObject.SetActive(false);
}

void Enemy::OnCollisionEnter(API::API_RigidBody other)
{
    std::string detectionTag = other.GetGameObject().GetTag();
    if (detectionTag == "Player")
    {
        EnemyMeleeMovement* meleeScript = (EnemyMeleeMovement*)gameObject.GetScript("EnemyMeleeMovement");
        if (meleeScript)
        {
            PlayerStats* pStats = (PlayerStats*)other.GetGameObject().GetScript("PlayerStats");
            if (meleeScript->enemState == EnemyMeleeMovement::States::ATTACKIG && meleeIsAtking) pStats->TakeDamage(10, 0), meleeIsAtking = false;
        }
    }
    /*if (detectionTag == "Projectile")
    {
        isHit = true;
        enemyShader.SetColor(255, 0, 0, 0.5);
        if (!_hitShader) _hitShader = true;
    }*/
}

void Enemy::ActiveSlow(float q, float time)
{
    actSlow = true;
    _qSlow = q;
    _tSlow = time;
}

void Enemy::ActiveStun(float time)
{
    actStun = true;
    _tStun = time;
}

void Enemy::EnemySlow(float xSlow, float time)
{
    float dt = Time::GetDeltaTime();
    if (xSlow < 0) xSlow = 0;

    _coldSlow += dt;
    if (_coldSlow >= time)
    {

        _coldSlow = 0;
        slowVel = 1;
        actSlow = false;
        Console::Log("slowingggggggggggg");

    }
    else
    {
        slowVel = xSlow;
    }
}

void Enemy::EnemyRecoil(float xRec)
{

    // rb.SetVelocity(gameObject.GetTransform().GetBackward() * xRec);
    gameObject.GetTransform().Translate(gameObject.GetTransform().GetBackward() * xRec);
    Console::Log("aaaaaa");
}

void Enemy::EnemyStun(float timeStun)
{
    float dt = Time::GetDeltaTime();
    _coldStun += dt;
    if (_coldStun >= timeStun)
    {
        _coldStun = 0;
        stunVel = 1;
        actStun = false;
        //Console::Log("slowingggggggggggg");
    }
    else
    {
        stunVel = 0;
    }
}

void Enemy::AddBomb()
{
    currentBombNum++;
    if (currentBombNum > maxBombNum) currentBombNum = maxBombNum;
    else if (currentBombNum == 1) bomb.SetActive(true);
    bomb.GetMaterialCompoennt().ChangeAlbedoTexture(textureBomb[currentBombNum - 1]);
}

void Enemy::CheckBombs()
{
    if (currentBombNum > 0)
    {
        if (shotgunLevel > 2) TakeDamage(15.0f * currentBombNum, 15.0f * currentBombNum);
        else TakeDamage(10.0f * currentBombNum, 10.0f * currentBombNum);
        currentBombNum = 0;
        bomb.SetActive(false);
    }
}

void Enemy::AddBurn()
{
    burnTime += Time::GetDeltaTime();
    if (burnTime > 6.0f) burnTime = 6.0f;
    resetBurn = 0.2f;
}