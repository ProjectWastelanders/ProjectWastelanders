#include "BossLoop.h"
#include "../Player/PlayerStats.h"
#include "../Shooting/Projectile.h"
#include "../Shooting/StickBomb.h"
//Pau Olmos

HELLO_ENGINE_API_C BossLoop* CreateBossLoop(ScriptToInspectorInterface* script)
{
    BossLoop* classInstance = new BossLoop();
    script->AddDragFloat("hp", &classInstance->hp);
    script->AddDragInt("Phase", &classInstance->phase);
    script->AddDragFloat("Weakness", &classInstance->weakTime);
    script->AddDragFloat("shield1Hp", &classInstance->shield[0]);
    script->AddDragFloat("shield2Hp", &classInstance->shield[1]);
    script->AddDragFloat("shield3Hp", &classInstance->shield[2]);
    script->AddDragBoxGameObject("PLAYER", &classInstance->player);
    script->AddDragBoxGameObject("SHIELD", &classInstance->rockShield);
    script->AddDragBoxGameObject("Cover1", &classInstance->cover1);
    script->AddDragBoxGameObject("Cover2", &classInstance->cover2);
    script->AddDragBoxGameObject("Cover3", &classInstance->cover3);
    script->AddDragBoxGameObject("Cover4", &classInstance->cover4);
    script->AddDragBoxGameObject("Cover5", &classInstance->cover5);
    script->AddDragBoxGameObject("Cover6", &classInstance->cover6);
    script->AddDragBoxGameObject("Cover7", &classInstance->cover7);
    script->AddDragBoxGameObject("Cover8", &classInstance->cover8);
    script->AddDragBoxGameObject("Cover9", &classInstance->cover9);
    script->AddDragBoxGameObject("Cover10", &classInstance->cover10);
    script->AddDragBoxGameObject("Cover11", &classInstance->cover11);
    script->AddDragBoxGameObject("Cover12", &classInstance->cover12);
    script->AddDragBoxGameObject("Bomb", &classInstance->bomb);
    script->AddDragBoxGameObject("Bomb Shield", &classInstance->bombShield);
    script->AddDragBoxTextureResource("Texture Bomb 1", &classInstance->textureBomb[0]);
    script->AddDragBoxTextureResource("Texture Bomb 2", &classInstance->textureBomb[1]);
    script->AddDragBoxTextureResource("Texture Bomb 3", &classInstance->textureBomb[2]);
    script->AddDragBoxTextureResource("Texture Bomb 4", &classInstance->textureBomb[3]);
    script->AddDragBoxTextureResource("Texture Bomb 5", &classInstance->textureBomb[4]);
    script->AddDragBoxTextureResource("Texture Bomb 6", &classInstance->textureBomb[5]);

    script->AddDragBoxAnimationPlayer("Animation Player", &classInstance->animationPlayer);
    script->AddDragBoxAnimationResource("Idle Animation", &classInstance->idleAnim);
    script->AddDragBoxAnimationResource("Idle 2 Animation", &classInstance->idleAnim2);
    script->AddDragBoxAnimationResource("Knock Up Animation", &classInstance->knockUpAnim);
    script->AddDragBoxAnimationResource("Move Animation", &classInstance->movingAnim);
    script->AddDragBoxAnimationResource("Move with Obj Animation", &classInstance->movingWithObjAnim);
    script->AddDragBoxAnimationResource("Recover Animation", &classInstance->recoverAnim);
    script->AddDragBoxAnimationResource("Special Animation", &classInstance->specialAnim);
    script->AddDragBoxAnimationResource("Special 2 Animation", &classInstance->specialAnim2);
    script->AddDragBoxAnimationResource("Take Obj Animation", &classInstance->takeObjAnim);
    script->AddDragBoxAnimationResource("Throw Obj Animation", &classInstance->throwObjAnim);
    script->AddDragBoxAnimationResource("Die Animation", &classInstance->dieAnim);
    //TEMPORAL FOR ALPHA 1

    script->AddDragBoxGameObject("BLOOD", &classInstance->blood);
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    return classInstance;


}

void BossLoop::Start()
{
    shotgunLevel = API_QuickSave::GetInt("shotgun_level");
    recoverTimer = 0;
    DieTimer = 0;
    knockUpTimer = 0;

    draxDialog = false;

    blood.GetTransform().SetPosition(0, -1000, 0);

}

void BossLoop::Update()
{
    dist = player.GetTransform().GetGlobalPosition().Distance(gameObject.GetTransform().GetGlobalPosition());

    

    if (destroyHighCover == true) {
        highCoverTime += Time::GetDeltaTime();

        cover1.GetTransform().Scale(-2 * Time::GetDeltaTime());
        cover2.GetTransform().Scale(-2 * Time::GetDeltaTime());
        cover3.GetTransform().Scale(-2 * Time::GetDeltaTime());
        cover4.GetTransform().Scale(-2 * Time::GetDeltaTime());
        cover5.GetTransform().Scale(-2 * Time::GetDeltaTime());
        cover6.GetTransform().Scale(-2 * Time::GetDeltaTime());
        cover7.GetTransform().Scale(-2 * Time::GetDeltaTime());
        cover8.GetTransform().Scale(-2 * Time::GetDeltaTime());
        cover9.GetTransform().Scale(-2 * Time::GetDeltaTime());
        cover10.GetTransform().Scale(-2 * Time::GetDeltaTime());
        cover11.GetTransform().Scale(-2 * Time::GetDeltaTime());
        cover12.GetTransform().Scale(-2 * Time::GetDeltaTime());

        if (highCoverTime > 1.0f)
        {
            cover1.GetTransform().SetPosition(0, -1000, 0);
            cover2.GetTransform().SetPosition(0, -1000, 0);
            cover3.GetTransform().SetPosition(0, -1000, 0);
            cover4.GetTransform().SetPosition(0, -1000, 0);
            cover5.GetTransform().SetPosition(0, -1000, 0);
            cover6.GetTransform().SetPosition(0, -1000, 0);
            cover7.GetTransform().SetPosition(0, -1000, 0);
            cover8.GetTransform().SetPosition(0, -1000, 0);
            cover9.GetTransform().SetPosition(0, -1000, 0);
            cover10.GetTransform().SetPosition(0, -1000, 0);
            cover11.GetTransform().SetPosition(0, -1000, 0);
            cover12.GetTransform().SetPosition(0, -1000, 0);

            cover1.SetActive(false);
           
            cover2.SetActive(false);

            cover3.SetActive(false);

            cover4.SetActive(false);

            cover5.SetActive(false);

            cover6.SetActive(false);

            cover7.SetActive(false);

            cover8.SetActive(false);

            cover9.SetActive(false);

            cover10.SetActive(false);

            cover11.SetActive(false);

            cover12.SetActive(false);

            destroyHighCover = false;
        }

    }
    if (damaged == true)
    {
       

        
         bloodTimer += Time::GetDeltaTime();
         if (bloodTimer > 0.2f) 
        {
            bloodTimer = 0;
            blood.GetTransform().SetPosition(0, -1000, 0);
            damaged = false;
           
           
        }
        
        
    }
    if (!battle)
    {
        rockShield.SetActive(false);
    }

    if (dist < 80.0f && battle) {
        if (hp > 0) {
            if (canTakeDamage == true) {
                dt = Time::GetDeltaTime();
                weakTime += dt;
                rockShield.SetActive(false);

                if (knockUpTimer <= 1) {
                    if (animState != AnimationState::KNOCKUP)
                    {
                        animState = AnimationState::KNOCKUP;
                        animationPlayer.ChangeAnimation(knockUpAnim);
                        animationPlayer.SetLoop(true);
                        //animationPlayer.SetStayLast(true);
                        animationPlayer.Play();
                    }
                    knockUpTimer += dt;
                }
                else {
                    animationPlayer.Pause();
                }
            }
            else {
                rockShield.SetActive(true);                
            }
            if (weakTime >= time[phase]) {
                weakTime = 0;
                phase--;
                shield[phase] = maxShield[phase];
                canTakeDamage = false;
                draxDialog = true;
                dt = Time::GetDeltaTime();
                if (recoverTimer <= 6) {
                    if (animState != AnimationState::RECOVER)
                    {
                        animState = AnimationState::RECOVER;
                        animationPlayer.ChangeAnimation(recoverAnim);
                        animationPlayer.SetLoop(true);
                        animationPlayer.Play();
                    }
                    recoverTimer += dt;
                    
                }
                else {
                    animationPlayer.Pause();
                    recoverTimer = 0;
                    knockUpTimer = 0;
                }
            }
            if (hp <= maxHpLoss[phase - 1]) {
                weakTime = 0;
                canTakeDamage = false;

                if (phase == 2) {
                    
                    destroyHighCover = true;

                    cover1.GetParticleSystem().Play();
                    cover2.GetParticleSystem().Play();
                    cover3.GetParticleSystem().Play();
                    cover4.GetParticleSystem().Play();
                    cover5.GetParticleSystem().Play();
                    cover6.GetParticleSystem().Play();
                    cover7.GetParticleSystem().Play();
                    cover8.GetParticleSystem().Play();
                    cover9.GetParticleSystem().Play();
                    cover10.GetParticleSystem().Play();
                    cover11.GetParticleSystem().Play();
                    cover12.GetParticleSystem().Play();


                }
            }
        }
        else {
            dt = Time::GetDeltaTime();
            if (animState != BossLoop::AnimationState::DIE)
            {
                animState = BossLoop::AnimationState::DIE;
                animationPlayer.ChangeAnimation(dieAnim);
                animationPlayer.Play();
            }
            if (DieTimer >= 4) {
                endBattle = true;
                gameObject.SetActive(false);
                blood.SetActive(false);
                blood.GetParticleSystem().Stop();
                blood.GetParticleSystem().StopEmitting();

                gameObject.GetTransform().SetScale(0, 0, 0);
                //TEMPORAL FOR ALPHA 1
                finalTextPanel.SetActive(true);
                API_QuickSave::SetBool("level3_completed", true);
            }
            else {
                DieTimer += dt;
            }
        }
    }

    //burn
    if (burnTime > 3.0f)
    {
        TakeDamage(30.0f * Time::GetDeltaTime());
    }
    if (resetBurn > 0.0f)
    {
        resetBurn -= Time::GetDeltaTime();
        if (resetBurn <= 0.0f)
        {
            resetBurn = 0.0f;
            burnTime -= Time::GetDeltaTime();
        }
    }
}

void BossLoop::OnCollisionEnter(API::API_RigidBody other)
{
    std::string detectionName = other.GetGameObject().GetName();
    if (hp > 0) {
        if (detectionName == "Player")
        {
            PlayerStats* pStats = (PlayerStats*)other.GetGameObject().GetScript("PlayerStats");
            pStats->TakeDamage(meleeDmg, 0);
        }
    }
}

void BossLoop::TakeDamage(float damage)
{
 

    if (hp <= 0) return;
    
    if (canTakeDamage == true) {

        bloodTimer = 0.0f;
        blood.GetTransform().SetPosition(0,0, 0);

        damaged = true;
       
        hp -= damage;
        if (hp <= maxHpLoss[phase - 1]) {
            exploting = true;
        }

    }
    else {
        shield[phase] -= damage;
        Audio::Event("ebony_bullet_rock");
    }

    if (shield[phase] <= 0) {
        phase++;
        canTakeDamage = true;
    }
}

void BossLoop::AddBomb()
{
    currentBombNum++;
    if (currentBombNum > maxBombNum) currentBombNum = maxBombNum;
    else if (currentBombNum == 1)
    {
        if (canTakeDamage) bomb.SetActive(true);
        else bombShield.SetActive(true);
    }
    if (canTakeDamage)bomb.GetMaterialCompoennt().ChangeAlbedoTexture(textureBomb[currentBombNum - 1]);
    else bombShield.GetMaterialCompoennt().ChangeAlbedoTexture(textureBomb[currentBombNum - 1]);
}

void BossLoop::CheckBombs()
{
    if (currentBombNum > 0)
    {
        if (shotgunLevel > 2) TakeDamage(15.0f * currentBombNum);
        else TakeDamage(10.0f * currentBombNum);
        currentBombNum = 0;
        bomb.SetActive(false);
    }
}

void BossLoop::AddBurn()
{
    burnTime += Time::GetDeltaTime();
    if (burnTime > 6.0f) burnTime = 6.0f;
    resetBurn = 0.2f;
}