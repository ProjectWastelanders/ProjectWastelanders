#include "BossMovement.h"
//Pau Olmos
HELLO_ENGINE_API_C BossMovement* CreateBossMovement(ScriptToInspectorInterface* script)
{
	BossMovement* classInstance = new BossMovement();
    script->AddDragBoxGameObject("Player", &classInstance->player);
    script->AddDragBoxGameObject("Center of the BattleField", &classInstance->target);
    script->AddDragBoxGameObject("Boss", &classInstance->boss);
    script->AddDragFloat("BossSpeed", &classInstance->bossSpeed);
    script->AddCheckBox("Dashing?", &classInstance->bossDash);

	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void BossMovement::Start()
{
   bLoop = (BossLoop*)boss.GetScript("BossLoop");
}
void BossMovement::Update()
{
    distBP = player.GetTransform().GetGlobalPosition().Distance(gameObject.GetTransform().GetGlobalPosition());

    if (distBP < 80.0f) {
        if (bLoop->canTakeDamage == false && bossDash == false) {
            angle = Rotate(player.GetTransform().GetGlobalPosition(), angle);

            if (distBP > 20.0f) {
                Seek(&gameObject, player.GetTransform().GetGlobalPosition(), bossSpeed);
            }
            else if (distBP < 10.0f) {
                Hide(&gameObject, player.GetTransform().GetGlobalPosition(), bossSpeed * 1.5);
            }
        }
        if (bossDash == true) {
            Seek(&gameObject, target.GetTransform().GetGlobalPosition(), 10.0f);

            if (bLoop->animState != BossLoop::AnimationState::MOVE)
            {
                bLoop->animState = BossLoop::AnimationState::MOVE;
                bLoop->animationPlayer.ChangeAnimation(bLoop->movingAnim);
                bLoop->animationPlayer.SetLoop(false);
                bLoop->animationPlayer.Play();
            }
        }
    }
   
}

float BossMovement::Rotate(API_Vector3 target, float _angle)
{
    API_Vector2 lookDir;
    lookDir.x = (target.x - gameObject.GetTransform().GetGlobalPosition().x);
    lookDir.y = (target.z - gameObject.GetTransform().GetGlobalPosition().z);

    API_Vector2 normLookDir;
    normLookDir.x = lookDir.x / sqrt(pow(lookDir.x, 2) + pow(lookDir.y, 2));
    normLookDir.y = lookDir.y / sqrt(pow(lookDir.x, 2) + pow(lookDir.y, 2));
    _angle = 0;
    _angle = atan2(normLookDir.y, normLookDir.x) * RADTODEG - 90.0f;
    gameObject.GetTransform().SetRotation(0, -_angle, 0);

    return _angle;
}

void BossMovement::Seek(API_GameObject* follower_position, API_Vector3 target_position, float speed) 
{
    API_Vector3 direction = target_position - follower_position->GetTransform().GetGlobalPosition();
    follower_position->GetTransform().Translate(direction * speed/ 100);

    if (bossDash) {
        if (direction.x < 0.03 && direction.x > -0.03 && direction.y < 0.03 && direction.y && direction.z < 0.03 && direction.z) {
            bossDash = false;
        }
    }

}

void BossMovement::Hide(API_GameObject* follower_position, API_Vector3 target_position, float speed) 
{
    API_Vector3 direction = target_position - follower_position->GetTransform().GetGlobalPosition();
    follower_position->GetTransform().Translate(-direction.x * Time::GetDeltaTime() * speed, 0, -direction.z * speed * Time::GetDeltaTime());
}


void BossMovement::OnCollisionEnter(API::API_RigidBody other)
{
    std::string detectionTag = other.GetGameObject().GetTag();

        if (detectionTag == "Cover")
        {
            bossDash = true;
        }
}
