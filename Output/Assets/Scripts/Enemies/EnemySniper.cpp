#include "EnemySniper.h"
HELLO_ENGINE_API_C EnemySniper* CreateEnemySniper(ScriptToInspectorInterface* script)
{
	EnemySniper* classInstance = new EnemySniper();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragFloat("Range", &classInstance->range);
   // script->AddDragBoxGameObject("Target", &classInstance->target);
    script->AddDragBoxGameObject("Enemy gun", &classInstance->gunObj);
    script->AddDragBoxAnimationPlayer("Animation Player", &classInstance->animationPlayer);
    script->AddDragBoxAnimationResource("Idle Animation", &classInstance->idleAnim);
    script->AddDragBoxAnimationResource("Aim Animation", &classInstance->aimAnim);
    script->AddDragBoxAnimationResource("Hited Animation", &classInstance->hitAnim);
    script->AddDragBoxAnimationResource("Die Animation", &classInstance->dieAnim);
	return classInstance;
}

void EnemySniper::Start()
{
    Game::FindGameObjectsWithTag("Player", &target, 1);
    enemState = States::TARGETING;
    enemyGun = (EnemyGun*)gunObj.GetScript("EnemyAutomatic");

    enemy = (Enemy*)gameObject.GetScript("Enemy");
    targStats = (PlayerStats*)target.GetScript("PlayerStats");
}
void EnemySniper::Update()
{
    float dt = Time::GetDeltaTime(); 

    if (enemy != nullptr)
    {
        float dis = gameObject.GetTransform().GetGlobalPosition().Distance(target.GetTransform().GetGlobalPosition());
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
            LookAt(target.GetTransform().GetGlobalPosition());
            if (dis < range) {
                enemState = States::ATTACKIG;
            }
            else {
                enemState = States::TARGETING;
            }
        }

        switch (enemState)
        {
        case States::TARGETING:
            //animation
            if (animState != AnimationState::IDLE && !enemy->takingDmg)
            {
                animState = AnimationState::IDLE;
                animationPlayer.ChangeAnimation(idleAnim);
                animationPlayer.Play();
            }
            break;
        case States::ATTACKIG:

            if (enemyGun != nullptr)
            {
                enemyGun->Shoot();
            }
            //animation
            if (animState != AnimationState::SHOOT && !enemy->takingDmg)
            {
                animState = AnimationState::SHOOT;
                animationPlayer.ChangeAnimation(aimAnim);
                animationPlayer.Play();
            }
            break;
        case States::DYING:
            enemy->_coldAnimDie += dt;
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
}

void EnemySniper::LookAt(API_Vector3 tarPos)
{
    API_Vector2 lookDir;
    lookDir.x = (tarPos.x - gameObject.GetTransform().GetGlobalPosition().x);
    lookDir.y = (tarPos.z - gameObject.GetTransform().GetGlobalPosition().z);

    API_Vector2 normLookDir;
    normLookDir.x = lookDir.x / sqrt(pow(lookDir.x, 2) + pow(lookDir.y, 2));
    normLookDir.y = lookDir.y / sqrt(pow(lookDir.x, 2) + pow(lookDir.y, 2));
    float _angle = 0;
    _angle = atan2(normLookDir.y, normLookDir.x) * RADTODEG - 90.0f;

    gameObject.GetTransform().SetRotation(0, -_angle, 0);
}

