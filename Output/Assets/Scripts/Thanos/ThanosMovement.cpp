#include "ThanosMovement.h"
#include "ThanosAttacks.h"
#include "ThanosLoop.h"

HELLO_ENGINE_API_C ThanosMovement* CreateThanosMovement(ScriptToInspectorInterface* script)
{
	ThanosMovement* classInstance = new ThanosMovement();
	script->AddDragBoxGameObject("Player", &classInstance->player);
	script->AddDragBoxGameObject("Boss", &classInstance->boss);
    script->AddDragFloat("distBP", &classInstance->distBP);

	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void ThanosMovement::Start()
{
    Tattack = (ThanosAttacks*)boss.GetScript("ThanosAttacks");
    Tloop = (ThanosLoop*)boss.GetScript("ThanosLoop");

    Tattack->thanosAnimationPlayer.ChangeAnimation(Tattack->thanosWakeUp);
    Tattack->thanosAnimationPlayer.SetLoop(false);
    Tattack->thanosAnimationPlayer.Play();

}
void ThanosMovement::Update()
{
    distBP = player.GetTransform().GetGlobalPosition().Distance(gameObject.GetTransform().GetGlobalPosition());
    if (Tloop->phase < 3 && Tloop->hp>0) {
        if (distBP < 20.0f && Tloop->phase == 0) {
            //Tattack->thanosAnimationPlayer.ChangeAnimation(Tattack->thanosWakeUp);
            //Tattack->thanosAnimationPlayer.SetLoop(false);
            Tattack->thanosAnimationPlayer.Resume();
            Console::Log("kkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkk");
            startFightTimer += Time::GetDeltaTime();

            if (startFightTimer > 6.25f) {
                Tattack->thanosAnimationPlayer.ChangeAnimation(Tattack->thanosRunAnimation);
                Tattack->thanosAnimationPlayer.SetLoop(true);
                Tattack->thanosAnimationPlayer.Play();
                Tloop->phase = 1;
            }

        }
        else if (Tloop->phase == 0) {
            Tattack->thanosAnimationPlayer.Pause();
        }

        if (Tloop->phase == 1 && Tattack->thanosState != ThanosAttacks::THANOS_STATE::IDLE && Tattack->thanosState != ThanosAttacks::THANOS_STATE::DASHATTACK && Tattack->thanosState != ThanosAttacks::THANOS_STATE::THROWINGATTACK) {
            if (Tattack->isAttacking == false) {
                angle = Rotate(player.GetTransform().GetGlobalPosition(), angle);

                Seek(&gameObject, player.GetTransform().GetGlobalPosition(), bossSpeed);

            }
            dashCooldown += Time::GetDeltaTime();
        }
        else if (Tloop->phase == 2 && Tattack->hasPulsed == true) {
            if (Tattack->thanosState == ThanosAttacks::THANOS_STATE::LASER)  angle = Rotate(Tattack->laserPosition.GetTransform().GetGlobalPosition(), angle);
            else angle = Rotate(player.GetTransform().GetGlobalPosition(), angle);

            if (distBP > 15.0f && Tattack->thanosState != ThanosAttacks::THANOS_STATE::PULSE && Tattack->thanosState != ThanosAttacks::THANOS_STATE::LASER && Tattack->thanosState != ThanosAttacks::THANOS_STATE::BEAM && Tattack->thanosState != ThanosAttacks::THANOS_STATE::BURST) {
                walkCooldown += Time::GetDeltaTime();
                walkState = WALKSTATE::IDLE;
                Seek2(&gameObject, player.GetTransform().GetGlobalPosition(), bossSpeed);
            }
            else if (distBP < 10.0f && Tattack->thanosState != ThanosAttacks::THANOS_STATE::PULSE && Tattack->thanosState != ThanosAttacks::THANOS_STATE::LASER && Tattack->thanosState != ThanosAttacks::THANOS_STATE::BEAM && Tattack->thanosState != ThanosAttacks::THANOS_STATE::BURST) {
                walkCooldown += Time::GetDeltaTime();
                walkState = WALKSTATE::IDLE;
                Hide(&gameObject, player.GetTransform().GetGlobalPosition(), bossSpeed * 1.5f);
            }

            if (distBP > 12.5f && walkState != WALKSTATE::NORMAL && Tattack->thanosState != ThanosAttacks::THANOS_STATE::PULSE && Tattack->thanosState != ThanosAttacks::THANOS_STATE::LASER && Tattack->thanosState != ThanosAttacks::THANOS_STATE::BEAM && Tattack->thanosState != ThanosAttacks::THANOS_STATE::BURST && walkCooldown > 0.8f && Tattack->isMeteorAnimationReady == false) {
                walkState = WALKSTATE::NORMAL;
                walkCooldown = 0.0f;
                Tattack->thanosAnimationPlayer.ChangeAnimation(Tattack->thanosWalk2Animation);
                Tattack->thanosAnimationPlayer.SetLoop(true);
                Tattack->thanosAnimationPlayer.Play();

            }
            else if (distBP < 12.5f && walkState != WALKSTATE::BACKWARDS && Tattack->thanosState != ThanosAttacks::THANOS_STATE::PULSE && Tattack->thanosState != ThanosAttacks::THANOS_STATE::LASER && Tattack->thanosState != ThanosAttacks::THANOS_STATE::BEAM && Tattack->thanosState != ThanosAttacks::THANOS_STATE::BURST && walkCooldown > 0.8f && Tattack->isMeteorAnimationReady == false) {
                walkState = WALKSTATE::BACKWARDS;
                walkCooldown = 0.0f;

                Tattack->thanosAnimationPlayer.ChangeAnimation(Tattack->thanosWalkBackwardsAnimation);
                Tattack->thanosAnimationPlayer.SetLoop(true);
                Tattack->thanosAnimationPlayer.Play();
            }
        }
    }
    

}

float ThanosMovement::Rotate(API_Vector3 target, float _angle)
{
    API_Vector2 lookDir;
    lookDir.x = (target.x - boss.GetTransform().GetGlobalPosition().x);
    lookDir.y = (target.z - boss.GetTransform().GetGlobalPosition().z);

    API_Vector2 normLookDir;
    normLookDir.x = lookDir.x / sqrt(pow(lookDir.x, 2) + pow(lookDir.y, 2));
    normLookDir.y = lookDir.y / sqrt(pow(lookDir.x, 2) + pow(lookDir.y, 2));
    _angle = 0;
    _angle = atan2(normLookDir.y, normLookDir.x) * RADTODEG - 90.0f;
    boss.GetTransform().SetRotation(0, -_angle, 0);

    return _angle;
}

void ThanosMovement::Seek(API_GameObject* seeker, API_Vector3 target, float speed)
{
    API_Vector3 direction = target - seeker->GetTransform().GetGlobalPosition();

    seeker->GetTransform().Translate(direction * speed * Time::GetDeltaTime());

    float distTP = player.GetTransform().GetGlobalPosition().Distance(gameObject.GetTransform().GetGlobalPosition());
    Tattack->defenseSword.SetActive(true);
    //Console::Log("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    if (distTP > 3 && dashCooldown > 5.0f && Tattack->isAttacking == false) {

            Tattack->isAttacking = true;
            dashCooldown = 0.0f;
            Tattack->thanosState = ThanosAttacks::THANOS_STATE::IDLE;
            Tattack->defenseSword.SetActive(false);

    }
    else if (distTP < 2 && dashCooldown < 5.0f && Tattack->isAttacking == false) {

        Tattack->thanosAnimationPlayer.ChangeAnimation(Tattack->thanosMeleeAnimation);
        Tattack->thanosAnimationPlayer.SetLoop(false);
        Tattack->thanosAnimationPlayer.Play();
        Tattack->thanosState = ThanosAttacks::THANOS_STATE::MELEEATTACK;
        Tattack->isAttacking = true;
        dashCooldown = 0.0f;
        Tattack->defenseSword.SetActive(false);

    }
    
}

void ThanosMovement::Seek2(API_GameObject* seeker, API_Vector3 target, float speed)
{
    API_Vector3 direction = target - seeker->GetTransform().GetGlobalPosition();

    seeker->GetTransform().Translate(direction * (speed) * Time::GetDeltaTime());

    float distTP = player.GetTransform().GetGlobalPosition().Distance(gameObject.GetTransform().GetGlobalPosition());

    
}

void ThanosMovement::Hide(API_GameObject* follower_position, API_Vector3 target_position, float speed)
{
    API_Vector3 direction = target_position - follower_position->GetTransform().GetGlobalPosition();
    follower_position->GetTransform().Translate(-direction.x * Time::GetDeltaTime() * speed, 0, -direction.z * speed * Time::GetDeltaTime());
}
