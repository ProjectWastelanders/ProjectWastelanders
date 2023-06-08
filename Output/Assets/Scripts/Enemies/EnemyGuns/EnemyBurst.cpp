#include "EnemyBurst.h"
HELLO_ENGINE_API_C EnemyBurst* CreateEnemyBurst(ScriptToInspectorInterface* script)
{
	EnemyBurst* classInstance = new EnemyBurst();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);

    script->AddDragBoxGameObject("Self Enemy GO", &classInstance->enemyGO);
    script->AddDragBoxGameObject("Projectile Pull", &classInstance->projectilePull);
    script->AddDragFloat("Projectile Speed", &classInstance->projectileSpeed);
    script->AddDragFloat("Projectile Damage", &classInstance->projectileDamage);
    script->AddDragFloat("Projectile Resistance Damage", &classInstance->projectileResistanceDamage);
    script->AddDragFloat("Projectile Lifetime", &classInstance->projectileLifetime);
    script->AddDragBoxTransform("Projectile Spawn", &classInstance->shootingSpawn);
    script->AddDragBoxMeshResource("Projectile Mesh", &classInstance->projectileMesh);
    script->AddDragBoxTextureResource("Projectile Material", &classInstance->projectileMaterial);
    script->AddDragFloat("Projectile ScaleX", &classInstance->projectileScale.x);
    script->AddDragFloat("Projectile ScaleY", &classInstance->projectileScale.y);
    script->AddDragFloat("Projectile ScaleZ", &classInstance->projectileScale.z);
    script->AddDragFloat("Projectiles per second", &classInstance->cadence);
    script->AddDragFloat("Burst Space", &classInstance->fullBurstDelay);
    script->AddDragInt("Projectiles per burst", &classInstance->burstLenght);
    script->AddInputBox("Audio Event String", &classInstance->audioEventString);
	return classInstance;
}

void EnemyBurst::Start()
{
    if (cadence != 0) fullShotCooldown = 1 / cadence;
    else fullShotCooldown = 0;

    shotCount = burstLenght;
    rangerScript = (EnemyRanger*)enemyGO.GetScript("EnemyRanger");
    if (!rangerScript)Console::Log("aaaaaaaaaaaaaaaaaaaaaa");
   
}
void EnemyBurst::Update()
{
    // burst
    if (shotCount < burstLenght)
    {
        if (burstDelay <= 0)
        {
           shotCount++;
            burstDelay = fullBurstDelay;
            LauchProjectile(shootingSpawn);
             if (rangerScript&& rangerScript->scriptedForQuest)
            {

                
                   // Console::Log("obamaaaaaaaaaaaa");
                    if (rangerScript->distanceBeetweenPlayerEnemy < 300)
                        PlayShotSound(audioEventString)/*, Console::Log("uwuwuwuwuwuwuwwuwuwuwuw");*/;
                
            }
            else
            {
                PlayShotSound(audioEventString);
            }
           // PlayShotSound(audioEventString);
        }
        else
        {
            burstDelay -= Time::GetDeltaTime();
        }
    }

    if (canShoot) return;

    if (shotCooldown <= 0)
    {
        canShoot = true;
    }
    else
    {
        shotCooldown -= Time::GetDeltaTime();
    }
}

void EnemyBurst::Shoot()
{
    if (canShoot)
    {
        LauchProjectile(shootingSpawn);
        //PlayShotSound(audioEventString);
        if (rangerScript && rangerScript->scriptedForQuest)
        {


           // Console::Log("obamaaaaaaaaaaaa");
            if (rangerScript->distanceBeetweenPlayerEnemy < 300)
                PlayShotSound(audioEventString)/*, Console::Log("uwuwuwuwuwuwuwwuwuwuwuw");*/;

        }
        else
        {
            PlayShotSound(audioEventString);
        }
        canShoot = false;
        shotCooldown = fullShotCooldown;
        shotCount = 1;
        burstDelay = fullBurstDelay;
    }
}