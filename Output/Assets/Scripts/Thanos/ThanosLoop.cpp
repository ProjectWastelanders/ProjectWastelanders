#include "ThanosLoop.h"
#include "../Shooting/StickBomb.h"
HELLO_ENGINE_API_C ThanosLoop* CreateThanosLoop(ScriptToInspectorInterface* script)
{
    ThanosLoop* classInstance = new ThanosLoop();
    script->AddDragFloat("Hp", &classInstance->hp);
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragBoxGameObject("Bomb", &classInstance->bomb);
    script->AddDragBoxTextureResource("Texture Bomb 1", &classInstance->textureBomb[0]);
    script->AddDragBoxTextureResource("Texture Bomb 2", &classInstance->textureBomb[1]);
    script->AddDragBoxTextureResource("Texture Bomb 3", &classInstance->textureBomb[2]);
    script->AddDragBoxTextureResource("Texture Bomb 4", &classInstance->textureBomb[3]);
    script->AddDragBoxTextureResource("Texture Bomb 5", &classInstance->textureBomb[4]);
    script->AddDragBoxTextureResource("Texture Bomb 6", &classInstance->textureBomb[5]);
    return classInstance;
}

void ThanosLoop::Start()
{
    shotgunLevel = API_QuickSave::GetInt("shotgun_level");
}
void ThanosLoop::Update()
{
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

    if (hp < 0) {
        //gameObject.SetActive(false);
    }
    if(hp < 2000) {
        phase = 2;
    }

}

void ThanosLoop::OnCollisionEnter(API::API_RigidBody other)
{
    std::string detectionName = other.GetGameObject().GetName();
    std::string detectionTag = other.GetGameObject().GetTag();

    if (hp > 0) {
        if (detectionName == "Player")
        {
            PlayerStats* pStats = (PlayerStats*)other.GetGameObject().GetScript("PlayerStats");
            //pStats->TakeDamage(meleeDmg, 0);
        }
    }
}
void ThanosLoop::TakeDamage(float damage)
{
    hp -= damage;
}

void ThanosLoop::AddBomb()
{
    currentBombNum++;
    if (currentBombNum > maxBombNum) currentBombNum = maxBombNum;
    else if (currentBombNum == 1)
    {
        bomb.SetActive(true);
    }
    bomb.GetMaterialCompoennt().ChangeAlbedoTexture(textureBomb[currentBombNum - 1]);
}

void ThanosLoop::CheckBombs()
{
    if (currentBombNum > 0)
    {
        StickBomb* stickBomb = (StickBomb*)bomb.GetScript("StickBomb");
        if (stickBomb == nullptr) Console::Log("StickyBomb missing in Bomb from enemy.");
        else
        {
            stickBomb->triggerActive = true;
            if (shotgunLevel > 2) stickBomb->damage = 15.0f * currentBombNum;
            else stickBomb->damage = 10.0f * currentBombNum;
        }
        currentBombNum = 0;
        bomb.SetActive(false);
    }
}

void ThanosLoop::AddBurn()
{
    burnTime += Time::GetDeltaTime();
    if (burnTime > 3.0f) burnTime = 6.0f;
    resetBurn = 0.2f;
}