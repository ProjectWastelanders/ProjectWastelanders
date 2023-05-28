#include "EnemyDrop.h"
HELLO_ENGINE_API_C EnemyDrop* CreateEnemyDrop(ScriptToInspectorInterface* script)
{
    EnemyDrop* classInstance = new EnemyDrop();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragInt("Drop Index", &classInstance->dropIndex);
    return classInstance;
}

void EnemyDrop::Start()
{
    lifeTime = 5.0f;
}

void EnemyDrop::Update()
{
    lifeTime -= Time::GetDeltaTime();

    if (lifeTime <= 0)
    {
        Destroy();
        return;
    }
}

void EnemyDrop::Destroy()
{
    lifeTime = 5.0f;
    gameObject.GetParticleSystem().StopEmitting();
    gameObject.SetActive(false);
}