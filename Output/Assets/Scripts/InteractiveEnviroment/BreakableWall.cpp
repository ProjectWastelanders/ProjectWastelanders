#include "BreakableWall.h"
#include "../Shooting/Projectile.h"
#include "../Player/PlayerMove.h"
HELLO_ENGINE_API_C BreakableWall* CreateBreakableWall(ScriptToInspectorInterface* script)
{
    BreakableWall* classInstance = new BreakableWall();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragBoxParticleSystem("Wall Destroyed Particle", &classInstance->wallDestroyed);
    script->AddDragInt("MaxHp", &classInstance->maxHp);
    script->AddDragBoxGameObject("Fence Destroyed", &classInstance->fenceDestroyed);
    script->AddDragBoxGameObject("Fence Entire", &classInstance->fenceEntire);
    script->AddDragBoxRigidBody("Fence Entire Rb", &classInstance->fenceRigidbody);

    script->AddDragBoxUIImage("Tutorial_Img", &classInstance->Tutorial_Img);

    return classInstance;
}

void BreakableWall::Start()
{
    currentHp = maxHp;

    initalPos = { -1.250, -0.700, 0 };
    movingPos = { -1.250, -0.700, 0 };
    finalPos = { -0.780, -0.700, 0 };

    Tutorial_Img.GetGameObject().GetTransform().SetPosition(initalPos);
    Tutorial_Img.GetGameObject().SetActive(false);

    fenceDestroyed.SetActive(false);
}

void BreakableWall::Update()
{
    PopUpTutorial(tutorial_active);
}

void BreakableWall::OnCollisionEnter(API_RigidBody other)
{
    std::string detectionTag = other.GetGameObject().GetTag();

    if (detectionTag == "Projectile")
    {
        Projectile* projectile = (Projectile*)other.GetGameObject().GetScript("Projectile");
        ShootWall(projectile->damage);

    }
    else if (detectionTag == "Player")
    {
        PlayerMove* playerMove = (PlayerMove*)other.GetGameObject().GetScript("PlayerMove");

        if (playerMove->isDashing)
        {
            DestroyWall();
        }
    }
}

void BreakableWall::ShootWall(float projectileDamage)
{
    if (fenceDestroyed.IsActive())return;
    // Health damage
    currentHp -= projectileDamage;
    if (currentHp <= 0)
    {
        currentHp = 0;
        DestroyWall();

        
    }
}

void BreakableWall::DestroyWall()
{
    if (fenceDestroyed.IsActive())return;

    wallDestroyed.Play();

    Audio::Event("fence_breaking");

    fenceDestroyed.SetActive(true);
    fenceEntire.SetActive(false);
    fenceRigidbody.SetTrigger(true);
    tutorial_active = true;
    

}

void BreakableWall::PopUpTutorial(bool active)
{
    if (active == true && hideChest == false)
    {
        Tutorial_Img.GetGameObject().SetActive(true);
        if (Tutorial_Img.GetGameObject().GetTransform().GetLocalPosition().x < finalPos.x)
        {
            movingPos.x += 0.32 * Time::GetDeltaTime();
            timerTutorial = true;
        }
        if (timerTutorial == true)
        {
            showTutorial += 1.0f * Time::GetDeltaTime();

        }

    }

    if (showTutorial >= 8.0f)
    {

        if (Tutorial_Img.GetGameObject().GetTransform().GetLocalPosition().x > initalPos.x)
        {
            movingPos.x -= 0.32 * Time::GetDeltaTime();
            timerTutorial = false;
            active = false;
            hideChest = true;
        }

        else if (Tutorial_Img.GetGameObject().GetTransform().GetLocalPosition().x < initalPos.x) {
            Tutorial_Img.GetGameObject().SetActive(false);
        }



    }
    Tutorial_Img.GetGameObject().GetTransform().SetPosition(movingPos);
}
