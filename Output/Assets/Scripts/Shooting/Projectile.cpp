#include "Projectile.h"
#include "ProjectilePull.h"
#include "../Enemies/Enemy.h"
#include "../EbonyMaw/BossLoop.h"
#include "../Thanos/ThanosLoop.h"
#include "../Thanos/ThanosAttacks.h"

HELLO_ENGINE_API_C Projectile* CreateProjectile(ScriptToInspectorInterface* script)
{
    Projectile* classInstance = new Projectile();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    return classInstance;
}

void Projectile::Start()
{
    shotgunLevel = API_QuickSave::GetInt("shotgun_level");
    rigidBody = gameObject.GetRigidBody();
}

void Projectile::Update()
{
    lifeTime -= Time::GetDeltaTime();

    if (lifeTime <= 0)
    {
        Destroy();
        return;
    }

    if (type == PROJECTILE_TYPE::FLAMETHROWER)
    {
        float scaleMultiply = 2.0f - lifeTime;
        gameObject.GetRigidBody().SetBoxScale({ 0.1f + scaleMultiply, 0.1f + scaleMultiply, 0.1f + scaleMultiply });
    }

    rigidBody.SetVelocity({ gameObject.GetTransform().GetForward() * speed * Time::GetDeltaTime() * 60.0f });

   // gameObject.GetTransform().Translate(gameObject.GetTransform().GetForward() * speed * Time::GetDeltaTime());
}

void Projectile::Destroy()
{
    gameObject.GetTransform().SetPosition(0, 9999, 0);
    reflected = false;
    gameObject.SetActive(false);
    gameObject.GetParticleSystem().StopEmitting();
}

void Projectile::OnCollisionEnter(API::API_RigidBody other)
{
    std::string detectionTag = other.GetGameObject().GetTag();
    
    switch (type)
    {
    case PROJECTILE_TYPE::NONE:
        if (detectionTag == "Wall")
        {
            Destroy();
        }
        else if (detectionTag == "Enemy")
        {
            Enemy* enemy = (Enemy*)other.GetGameObject().GetScript("Enemy");
            switch (action)
            {
            case PROJECTILE_ACTION::SLOW:
                if (enemy) enemy->ActiveSlow(0.5f, 2);
                break;
            case PROJECTILE_ACTION::FREEZE:
                if (enemy) enemy->ActiveStun(2);
                break;
            default:
                break;
            }
            if (enemy)
            {
                enemy->TakeDamage(damage, resistanceDamage);
                enemy->CheckBombs();
            }
            Destroy();
        }
        else if (detectionTag == "Boss")
        {
            BossLoop* miniBoss = (BossLoop*)other.GetGameObject().GetScript("BossLoop");
            if (miniBoss)
            {
                miniBoss->TakeDamage(damage);
                miniBoss->CheckBombs();
            }
            Destroy();
        }
        else if (detectionTag == "Thanos")
        {
            ThanosLoop* boss = (ThanosLoop*)other.GetGameObject().GetScript("ThanosLoop");
            ThanosAttacks* bosss = (ThanosAttacks*)other.GetGameObject().GetScript("ThanosAttacks");
                if (bosss->defenseSword.IsActive() == true) {
                    gameObject.GetTransform().Rotate(0, 180, 0);
                    reflected = true;
                }
                else {
                    if (boss) {

                        boss->TakeDamage(damage);
                        boss->CheckBombs();
                        Destroy();
                    }
                }
           
        }
        else if (detectionTag == "Player" && reflected)
        {
            PlayerStats* stats = (PlayerStats*)other.GetGameObject().GetScript("PlayerStats");
            if (stats)
            {
                stats->TakeDamage(10, 10);
            }
            Destroy();
        }
        break;
    case PROJECTILE_TYPE::SEMI:
        if (detectionTag == "Wall")
        {
            Destroy();
        }
        else if (detectionTag == "Enemy") // EXIT
        {
            Enemy* enemy = (Enemy*)other.GetGameObject().GetScript("Enemy");
            if (enemy) enemy->TakeDamage(damage, resistanceDamage);
            pull->LauchProjectileSECONDARY_SEMI(speed, damage / 3.0f, resistanceDamage / 3.0f, 0.3f, gameObject.GetTransform(), { 0.1f, 0.1f, 0.1f }, 30.0f, other.GetGameObject().GetUID());
            pull->LauchProjectileSECONDARY_SEMI(speed, damage / 3.0f, resistanceDamage / 3.0f, 0.3f, gameObject.GetTransform(), { 0.1f, 0.1f, 0.1f }, 0.0f, other.GetGameObject().GetUID());
            pull->LauchProjectileSECONDARY_SEMI(speed, damage / 3.0f, resistanceDamage / 3.0f, 0.3f, gameObject.GetTransform(), { 0.1f, 0.1f, 0.1f }, -30.0f, other.GetGameObject().GetUID());
            Destroy();
        }
        else if (detectionTag == "Boss")
        {
            BossLoop* miniBoss = (BossLoop*)other.GetGameObject().GetScript("BossLoop");
            if (miniBoss) miniBoss->TakeDamage(damage);
            pull->LauchProjectileSECONDARY_SEMI(speed, damage / 3.0f, resistanceDamage / 3.0f, 0.3f, gameObject.GetTransform(), { 0.1f, 0.1f, 0.1f }, 30.0f, other.GetGameObject().GetUID());
            pull->LauchProjectileSECONDARY_SEMI(speed, damage / 3.0f, resistanceDamage / 3.0f, 0.3f, gameObject.GetTransform(), { 0.1f, 0.1f, 0.1f }, 0.0f, other.GetGameObject().GetUID());
            pull->LauchProjectileSECONDARY_SEMI(speed, damage / 3.0f, resistanceDamage / 3.0f, 0.3f, gameObject.GetTransform(), { 0.1f, 0.1f, 0.1f }, -30.0f, other.GetGameObject().GetUID());
            Destroy();
        }
        else if (detectionTag == "Thanos")
        {
            ThanosAttacks* bosss = (ThanosAttacks*)other.GetGameObject().GetScript("ThanosAttacks");
            ThanosLoop* boss = (ThanosLoop*)other.GetGameObject().GetScript("ThanosLoop");
            
                 
                 if (bosss->defenseSword.IsActive() == true) {
                     gameObject.GetTransform().Rotate(0, 180, 0);
                     reflected = true;
                 }
                 else {
                     if(boss) boss->TakeDamage(damage);
                     pull->LauchProjectileSECONDARY_SEMI(speed, damage / 3.0f, resistanceDamage / 3.0f, 0.3f, gameObject.GetTransform(), { 0.1f, 0.1f, 0.1f }, 30.0f, other.GetGameObject().GetUID());
                     pull->LauchProjectileSECONDARY_SEMI(speed, damage / 3.0f, resistanceDamage / 3.0f, 0.3f, gameObject.GetTransform(), { 0.1f, 0.1f, 0.1f }, 0.0f, other.GetGameObject().GetUID());
                     pull->LauchProjectileSECONDARY_SEMI(speed, damage / 3.0f, resistanceDamage / 3.0f, 0.3f, gameObject.GetTransform(), { 0.1f, 0.1f, 0.1f }, -30.0f, other.GetGameObject().GetUID());
                     Destroy();
                 }
                
                
               
            
        }
        else if (detectionTag == "Player" && reflected)
        {
            PlayerStats* stats = (PlayerStats*)other.GetGameObject().GetScript("PlayerStats");
            if (stats)
            {
                stats->TakeDamage(10, 10);
            }
            Destroy();
        }
        break;
    case PROJECTILE_TYPE::SECONDARY_SEMI:
        if (detectionTag == "Wall")
        {
            Destroy();
        }
        else if (detectionTag == "Enemy" && other.GetGameObject().GetUID() != ignoreGO)
        {
            Enemy* enemy = (Enemy*)other.GetGameObject().GetScript("Enemy");
            if (enemy) enemy->TakeDamage(damage, resistanceDamage);
            Destroy();
        }
        else if (detectionTag == "Boss" && other.GetGameObject().GetUID() != ignoreGO)
        {
            BossLoop* miniBoss = (BossLoop*)other.GetGameObject().GetScript("BossLoop");
            if (miniBoss) miniBoss->TakeDamage(damage);
            Destroy();
        }
        else if (detectionTag == "Thanos" && other.GetGameObject().GetUID() != ignoreGO)
        {
            ThanosLoop* boss = (ThanosLoop*)other.GetGameObject().GetScript("ThanosLoop");
            if (boss) boss->TakeDamage(damage);
            Destroy();
        }
        break;
    case PROJECTILE_TYPE::AUTO:
        if (detectionTag == "Wall")
        {
            Destroy();
        }
        if (detectionTag == "Enemy")
        {
            Enemy* enemy = (Enemy*)other.GetGameObject().GetScript("Enemy");
            if (enemy) enemy->TakeDamage(damage, resistanceDamage);
            Destroy();
        }
        else if (detectionTag == "Boss")
        {
            BossLoop* miniBoss = (BossLoop*)other.GetGameObject().GetScript("BossLoop");
            if (miniBoss) miniBoss->TakeDamage(damage);
            Destroy();
        }
        else if (detectionTag == "Thanos")
        {
            ThanosLoop* boss = (ThanosLoop*)other.GetGameObject().GetScript("ThanosLoop");
            ThanosAttacks* bosss = (ThanosAttacks*)other.GetGameObject().GetScript("ThanosAttacks");


            if (bosss->defenseSword.IsActive() == true) {
                gameObject.GetTransform().Rotate(0, 180, 0);
                reflected = true;
            }
            else {
                if (boss) boss->TakeDamage(damage);
                Destroy();
            }

            
        }
        else if (detectionTag == "Player" && reflected)
        {
            PlayerStats* stats = (PlayerStats*)other.GetGameObject().GetScript("PlayerStats");
            if (stats)
            {
                stats->TakeDamage(10, 10);
            }
            Destroy();
        }
        break;
    case PROJECTILE_TYPE::BURST:
        if (detectionTag == "Wall")
        {
            Destroy();
        }
        else if (detectionTag == "Enemy")
        {
            Enemy* enemy = (Enemy*)other.GetGameObject().GetScript("Enemy");
            if (enemy) enemy->AddBomb();
            Destroy();
        }
        else if (detectionTag == "Boss")
        {
            BossLoop* miniBoss = (BossLoop*)other.GetGameObject().GetScript("BossLoop");
            if (miniBoss) miniBoss->AddBomb();
            Destroy();
        }
        else if (detectionTag == "Thanos")
        {
            ThanosLoop* boss = (ThanosLoop*)other.GetGameObject().GetScript("ThanosLoop");
            if (boss) boss->TakeDamage(damage);
            Destroy();
        }
        break;
    case PROJECTILE_TYPE::SHOTGUN:
        if (detectionTag == "Wall")
        {
            Destroy();
        }
        else if (detectionTag == "Enemy") // EXIT
        {
            Enemy* enemy = (Enemy*)other.GetGameObject().GetScript("Enemy");
            if (enemy) enemy->TakeDamage(damage, resistanceDamage);
            Destroy();
        }
        else if (detectionTag == "Boss")
        {
            BossLoop* miniBoss = (BossLoop*)other.GetGameObject().GetScript("BossLoop");
            if (miniBoss) miniBoss->TakeDamage(damage);
            Destroy();
        }
        else if (detectionTag == "Thanos")
        {
            ThanosLoop* boss = (ThanosLoop*)other.GetGameObject().GetScript("ThanosLoop");
            ThanosAttacks* bosss = (ThanosAttacks*)other.GetGameObject().GetScript("ThanosAttacks");

            if (bosss->defenseSword.IsActive() == true) {
                gameObject.GetTransform().Rotate(0, 180, 0);
                reflected = true;
            }
            else {
                if (boss) boss->TakeDamage(damage);
                Destroy();
            }
            
        }
        else if (detectionTag == "Player" && reflected)
        {
            PlayerStats* stats = (PlayerStats*)other.GetGameObject().GetScript("PlayerStats");
            if (stats)
            {
                stats->TakeDamage(10, 10);
            }
            Destroy();
        }
        break;
    case PROJECTILE_TYPE::FLAMETHROWER:
        if (detectionTag == "Wall")
        {
            Destroy();
        }
        else if (detectionTag == "Enemy")
        {
            Enemy* enemy = (Enemy*)other.GetGameObject().GetScript("Enemy");
            if (enemy)
            {
                enemy->TakeDamage(damage, resistanceDamage);
                enemy->CheckBombs();
            }
            Destroy();
        }
        else if (detectionTag == "Boss")
        {
            BossLoop* miniBoss = (BossLoop*)other.GetGameObject().GetScript("BossLoop");
            if (miniBoss)
            {
                miniBoss->TakeDamage(damage);
                miniBoss->CheckBombs();
                miniBoss->AddBurn();
            }
            Destroy();
        }
        else if (detectionTag == "Thanos")
        {
            ThanosLoop* boss = (ThanosLoop*)other.GetGameObject().GetScript("ThanosLoop");
            if (boss)
            {
                boss->TakeDamage(damage);
                boss->CheckBombs();
                boss->AddBurn();
            }
            Destroy();
        }
        break;
    case PROJECTILE_TYPE::RICOCHET:
        if (detectionTag == "Wall")
        {
            Destroy();
        }
        else if (detectionTag == "Enemy" || detectionTag == "Boss" || detectionTag == "Thanos")
        {
            if (targetsHitted == 0)
            {
                API_Vector3 rotation = pull->CheckTargetDirectionRICOCHET(gameObject.GetTransform().GetGlobalPosition(), ricochetTarget, other.GetGameObject().GetUID());
                if (rotation.x == 0 && rotation.z == 0) // means enemy in range
                {
                    gameObject.GetTransform().SetRotation(rotation);
                    lifeTime = 1.0f;
                    targetsHitted++;
                }
                else
                {
                    Destroy();
                }
            }
            else if (other.GetGameObject().GetUID() == ricochetTarget)
            {
                if (targetsHitted > 3)
                {
                    Destroy();
                }
                else
                {
                    API_Vector3 rotation = pull->CheckTargetDirectionRICOCHET(gameObject.GetTransform().GetGlobalPosition(), ricochetTarget, other.GetGameObject().GetUID());
                    if (rotation.x == 0 && rotation.z == 0) // means enemy in range
                    {
                        gameObject.GetTransform().SetRotation(rotation);
                        lifeTime = 1.0f;
                        targetsHitted++;
                    }
                    else
                    {
                        Destroy();
                    }
                }
            }
            if (detectionTag == "Enemy")
            {
                Enemy* enemy = (Enemy*)other.GetGameObject().GetScript("Enemy");
                if (enemy)
                {
                    enemy->TakeDamage(damage, resistanceDamage);
                    enemy->CheckBombs();
                }
            }
            else if (detectionTag == "Boss")
            {
                BossLoop* miniBoss = (BossLoop*)other.GetGameObject().GetScript("BossLoop");
                if (miniBoss)
                {
                    miniBoss->TakeDamage(damage);
                    miniBoss->CheckBombs();
                }
            }
            else if (detectionTag == "Thanos")
            {
                ThanosLoop* boss = (ThanosLoop*)other.GetGameObject().GetScript("ThanosLoop");
                if (boss)
                {
                    boss->TakeDamage(damage);
                    boss->CheckBombs();
                }
                Destroy();
            }
        }
        break;
    default:
        break;
    }
}