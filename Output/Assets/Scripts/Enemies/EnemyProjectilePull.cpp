#include "EnemyProjectilePull.h"
HELLO_ENGINE_API_C EnemyProjectilePull* CreateEnemyProjectilePull(ScriptToInspectorInterface* script)
{
    EnemyProjectilePull* classInstance = new EnemyProjectilePull();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragInt("Pull Size", &classInstance->pullSize);
    //script->AddDragBoxParticleSystem("Particles", &classInstance->particleTest);
    return classInstance;
}

void EnemyProjectilePull::Start()
{
    for (size_t i = 0; i < pullSize; i++)
    {
        API_GameObject newProjectile = Game::CreateGameObject("EnemyProjectile", "EnemyProjectile");
        newProjectile.AddMeshRenderer();
        newProjectile.AddMaterial();
        API_RigidBody rb = newProjectile.CreateRigidBodyBox((0, 0, 0), (0, 0, 0), (0.3f, 0.3f, 0.3f), false);
        rb.SetTrigger(true);
        //newProjectile.AddParticleSystem(particleTest);
        EnemyProjectile* bullet = (EnemyProjectile*)newProjectile.AddScript("EnemyProjectile");
        bullet->Start();
        newProjectile.SetActive(false);
        pull.push_back(newProjectile);
    }
}

void EnemyProjectilePull::Update()
{

}

API_GameObject EnemyProjectilePull::GetFirstActiveProjectile()
{
    for (size_t i = 0; i < pullSize; i++)
    {
        if (!pull[i].IsActive()) return pull[i];
    }

    return pull[0];
}

void EnemyProjectilePull::LauchProjectile(float projectileSpeed, float projectileDamage, float projectileResistanceDamage, float projectileLifetime, API_Transform shootingSpawn, uint projectileMesh, uint projectileMaterial, API_Vector3 enemyRotation, API_Vector3 projectileScale, bool randomDirection)
{
    API_GameObject go = GetFirstActiveProjectile();
    go.SetActive(true);
    go.GetTransform().SetPosition(shootingSpawn.GetGlobalPosition());
    go.GetTransform().SetRotation(enemyRotation);
    go.GetTransform().SetScale(projectileScale);

    go.GetMeshRenderer().ChangeMesh(projectileMesh);
    go.GetMaterialCompoennt().ChangeAlbedoTexture(projectileMaterial);
    go.GetParticleSystem().Play();


    if (randomDirection)
    {
        float offsetX = (-49 + rand() % (100)) / 5.0f; // values between -25 and 25
        float offsetY = (-49 + rand() % (100)) / 5.0f;
        go.GetTransform().Rotate(offsetX, offsetY, 0);
    }

    EnemyProjectile* projectile = (EnemyProjectile*)go.GetScript("EnemyProjectile");
    projectile->speed = projectileSpeed;
    projectile->damage = projectileDamage;
    projectile->resistanceDamage = projectileResistanceDamage;
    projectile->lifeTime = projectileLifetime;
}