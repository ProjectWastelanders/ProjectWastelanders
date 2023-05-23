#include "ShotgunBomb.h"
#include "ShotgunBombExplosion.h"
HELLO_ENGINE_API_C ShotgunBomb* CreateShotgunBomb(ScriptToInspectorInterface* script)
{
    ShotgunBomb* classInstance = new ShotgunBomb();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    return classInstance;
}

void ShotgunBomb::Start()
{

}

void ShotgunBomb::Update()
{
    if (explosion->triggerActive) return;

    lifeTime -= Time::GetDeltaTime();

    if (lifeTime <= 0)
    {
        Destroy();
        return;
    }

    gameObject.GetTransform().Translate(gameObject.GetTransform().GetForward() * speed * Time::GetDeltaTime());
}

void ShotgunBomb::Destroy()
{
    explosion->triggerActive = true;
    gameObject.GetParticleSystem().Stop();
}

void ShotgunBomb::OnCollisionEnter(API::API_RigidBody other)
{
    std::string detectionTag = other.GetGameObject().GetTag();

    if (detectionTag == "Wall" || (detectionTag == "Enemy" && other.GetGameObject().GetUID() != ignoreGO) || (detectionTag == "Boss" && other.GetGameObject().GetUID() != ignoreGO))
    {
        Destroy();
    }
}

void ShotgunBomb::ResetExposion()
{
    explosion->triggerActive = false;
}