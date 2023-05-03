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
}
void EnemyRanger::Update()
{
    float dt = Time::GetDeltaTime();

    Console::Log(std::to_string(actualPoint.x) +" " + std::to_string(actualPoint.z) );
    Console::Log("_move coldowb "+std::to_string(_movCooldown));
    if (enemy != nullptr /*&& targStats != nullptr*/ && !scriptedForQuest)
    {
            // float dis = gameObject.GetTransform().GetGlobalPosition().Distance(target.GetTransform().GetGlobalPosition());
          //float disZone = gameObject.GetTransform().GetGlobalPosition().Distance(actionZone.GetTransform().GetGlobalPosition());
            float dis = gameObject.GetTransform().GetLocalPosition().Distance(target.GetTransform().GetGlobalPosition());
            float disZone = gameObject.GetTransform().GetLocalPosition().Distance(actionZone.GetTransform().GetGlobalPosition());
            float targDisZone = target.GetTransform().GetGlobalPosition().Distance(actionZone.GetTransform().GetGlobalPosition());
        if (enemy->dying)enemState = States::DYING;

        if (enemState == States::TARGETING || enemState == States::ATTACKIG)
        {
            enemy->targeting = true;
        }
        else
        {
            enemy->targeting = false;
        }

        if (!enemy->dying)
        {

            float zoneRad = zoneRb.GetRadius() / 2;


            disZone > (zoneRad) ? enemy->isOut = true : enemy->isOut = false;
            targDisZone < (zoneRad) ? enemy->isTargIn = true : enemy->isTargIn = false;
            disZone > zoneRad ? _outCooldown += dt : _outCooldown = 0;
            enemy->isHit ? _hitOutCooldown += dt : _hitOutCooldown = 0;

            if (_hitOutCooldown >= hitOutTime) enemy->isHit = false, enemy->hitParticles.Stop();

            if (enemy->isTargIn)
            {
                if (zoneRb.GetGameObject().GetTransform().GetGlobalPosition() != targStats->actualZone.GetGameObject().GetTransform().GetGlobalPosition())
                {
                    targStats->actualZone = zoneRb;
                    targStats->detected = false;
                }
            }
            if (!enemy->isTargIn)
            {
                if (zoneRb.GetGameObject().GetTransform().GetGlobalPosition() == targStats->actualZone.GetGameObject().GetTransform().GetGlobalPosition())
                {
                    //targStats->actualZone = zoneRb;
                    targStats->detected = false;
                }
            }
            if ( !enemy->takingDmg && !enemy->actStun)
            {

                if ((enemState == States::ATTACKIG || enemState == States::TARGETING || enemy->isHit) && enemy->isTargIn)
                {
                    targStats->detected = true;
                }

                if ((dis < detectionDis) && (dis > disShoot) && enemState != States::TARGETING && !enemy->isOut && enemy->isTargIn || enemy->isHit || targStats->detected && enemy->isTargIn)
                {
                    _movCooldown = 0;
                    //_outCooldown = 0;
                    enemState = States::TARGETING;
                }
                else if (dis > lossingDis || enemy->isOut && !enemy->isTargIn && _outCooldown >= outTime)
                {
                    enemState = States::WANDERING;
                }

                if ((dis < disShoot) && enemState == States::TARGETING)
                {
                    enemState = States::ATTACKIG;
                }

            }
            if ((disZone > zoneRb.GetRadius() / 2))_outCooldown += dt;
            else _outCooldown = 0;

        }
        switch (enemState)
        {
        case States::WANDERING:

            enemy->currentSpeed = enemy->speed * enemy->stunVel * enemy->slowVel/** dt*/;
            

                //if ((gameObject.GetTransform().GetLocalPosition().Distance(actualPoint) < 5))
                if ((gameObject.GetTransform().GetGlobalPosition().Distance(actualPoint) < 4))
                {
                    numPoint++;
                    if (numPoint >= _avalPoints)numPoint = 0;
                    _canWalk = false;
                }
                if (!_canWalk)
                {
                    _movCooldown += Time::GetDeltaTime();
                    if (animState != AnimationState::IDLE && !enemy->takingDmg )
                    {
                        animState = AnimationState::IDLE;
                        animationPlayer.ChangeAnimation(idleAnim);
                        animationPlayer.Play();
                        //Console::Log("Walk");
                    }
                }
                else Wander(enemy->currentSpeed, actualPoint, enemy->enemyRb);

                if (_movCooldown > cooldownPoint)
                {
                    _movCooldown = 0;
                    _canWalk = true;
                }

                actualPoint = listPoints[numPoint].GetTransform().GetGlobalPosition();

               // if (!_canWalk)Wander(0, actualPoint, enemy->enemyRb);

                if (animState != AnimationState::WALK && !enemy->takingDmg && _canWalk)
                {
                    animState = AnimationState::WALK;
                    animationPlayer.ChangeAnimation(walkAnim);
                    animationPlayer.Play();
                    //Console::Log("Walk");
                }
                
               
            
            break;

        case States::TARGETING:

            enemy->currentSpeed = enemy->speed * enemy->acceleration * enemy->stunVel * enemy->slowVel/** dt*/;
           

                Seek(enemy->currentSpeed, target.GetTransform().GetGlobalPosition(), enemy->enemyRb);

                if (animState != AnimationState::RUN && !enemy->takingDmg)
                {
                    animState = AnimationState::RUN;
                    animationPlayer.ChangeAnimation(runAnim);
                    animationPlayer.Play();
                    //Console::Log("Walk");
                }
            
            break;

        case States::ATTACKIG:

            enemy->currentSpeed = enemy->speed * enemy->acceleration * enemy->stunVel * enemy->slowVel /** dt*/;
            if ( !enemy->takingDmg &&!enemy->actStun)
            {

                if (dis > disPlayer)
                {
                    Seek(enemy->currentSpeed, target.GetTransform().GetGlobalPosition(), enemy->enemyRb);

                }
                else if (dis < disPlayer - 5)
                {
                    enemy->enemyRb.SetVelocity(gameObject.GetTransform().GetBackward() * enemy->currentSpeed * 0.7);

                }
                else
                {
                    Seek(enemy->currentSpeed * 0, target.GetTransform().GetGlobalPosition(), enemy->enemyRb);
                }




                //gameObject.GetTransform().Translate(gameObject.GetTransform().GetBackward() * enemy->currentSpeed);

                Attacking(enemy->currentSpeed * 0.5f, target.GetTransform().GetGlobalPosition(), enemy->enemyRb);
                if (animState != AnimationState::SHOOT && !enemy->takingDmg)
                {
                    animState = AnimationState::SHOOT;
                    animationPlayer.ChangeAnimation(aimAnim);
                    animationPlayer.Play();
                    //Console::Log("Walk");
                }

            }
            break;
        case States::DYING:
            enemy->_coldAnimDie += dt;
            // enemy->dying = true;
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

            break;
        default:
            break;
        }
    }
    else if (enemy && scriptedForQuest)
    {
        enemyGun->Shoot();
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

void EnemyRanger::Attacking(float vel, API_Vector3 tarPos, API_RigidBody enemyRb)
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
        // gameObject.GetTransform().SetRotation(0 + _bRot.x, -_angle + _bRot.y, 0 + _bRot.z);

        if (enemyGun != nullptr)
        {
            enemyGun->Shoot();
        }
    }
    //Console::Log(std::to_string(_angle));
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
