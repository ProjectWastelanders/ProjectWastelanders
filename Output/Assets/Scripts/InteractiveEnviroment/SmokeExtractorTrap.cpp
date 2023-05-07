#include "SmokeExtractorTrap.h"
#include "SmokeTrapProjectile.h"
HELLO_ENGINE_API_C SmokeExtractorTrap* CreateSmokeExtractorTrap(ScriptToInspectorInterface* script)
{
    SmokeExtractorTrap* classInstance = new SmokeExtractorTrap();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);

    script->AddDragBoxParticleSystem("Smoke Particle", &classInstance->smoke);
    script->AddDragBoxParticleSystem("Fire Particle", &classInstance->fire);
    //Cuanto tarda a empezar a escupir fuego
    script->AddDragFloat("Delay on Hit", &classInstance->delay);
    script->AddDragFloat("Seconds emits fire", &classInstance->fireSeconds);
    script->AddDragInt("Pull Size", &classInstance->pullSize);
    script->AddDragBoxPrefabResource("Projectile Prefab", &classInstance->projectilePrefab);
    script->AddDragFloat("Projectile Delay", &classInstance->maxProjectileDelay);
    script->AddDragFloat("Projectile Lifetime", &classInstance->lifeTime);
    script->AddDragFloat("Projectile Speed", &classInstance->speed);

    return classInstance;
}

void SmokeExtractorTrap::Start()
{
    smoke.Play();

    currentDelay = delay;

    fireTimer = 0.0f;

    for (size_t i = 0; i < pullSize; i++)
    {
        API_GameObject newProjectile = Game::InstancePrefab(projectilePrefab, API_GameObject());
        pull.push_back(newProjectile);
    }
}
void SmokeExtractorTrap::Update()
{
    currentDelay -= Time::GetDeltaTime();

    if (currentDelay <= 0.0f && fireTimer < fireSeconds)
    {
        fireTimer += Time::GetDeltaTime();

        smoke.StopEmitting();
        fire.SetInitialSpeed(gameObject.GetTransform().GetForward() * 5);
        fire.Play();

        throwFire = true;

        //Audio::Event("trap_fire");

    }
    else
    {

        if (fireTimer > fireSeconds)
        {
            currentDelay = delay;
        }

        fireTimer = 0.0f;

        fire.StopEmitting();
        smoke.SetInitialSpeed(gameObject.GetTransform().GetForward() * 2.5f);
        smoke.Play();
        hitPlayer = true;
        throwFire = false;

        //Audio::Event("trap_smoke");

    }

    if (projectileDelay > 0.0f)
    {
        projectileDelay -= Time::GetDeltaTime();
    }

    if (throwFire && playerOnRange && projectileDelay <= 0.0f)
    {
        API_GameObject go = GetFirstInactiveProjectile();
        go.SetActive(true);
        go.GetTransform().SetPosition(gameObject.GetTransform().GetGlobalPosition());
        go.GetTransform().SetRotation(gameObject.GetTransform().GetGlobalRotation());

        SmokeTrapProjectile* projectile = (SmokeTrapProjectile*)go.GetScript("SmokeTrapProjectile");
        projectile->lifeTime = lifeTime;
        projectile->speed = speed;

        projectileDelay = maxProjectileDelay;
    }
}

void SmokeExtractorTrap::OnCollisionEnter(API_RigidBody other)
{
    std::string detectionTag = other.GetGameObject().GetTag();

    if (detectionTag == "Player")
    {
        playerOnRange = true;
    }
}

void SmokeExtractorTrap::OnCollisionExit(API_RigidBody other)
{
    std::string detectionTag = other.GetGameObject().GetTag();

    if (detectionTag == "Player")
    {
        playerOnRange = false;
    }
}

API_GameObject SmokeExtractorTrap::GetFirstInactiveProjectile()
{
    for (size_t i = 0; i < pullSize; i++)
    {
        if (!pull[i].IsActive()) return pull[i];
    }

    return pull[0];
}
