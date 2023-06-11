#include "ThanosLoop.h"
#include "../Shooting/StickBomb.h"
#include "../InteractiveEnviroment/StickyBombParticle.h"
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
    Game::FindGameObjectsWithTag("StickyBombParticles", &bombParticles[0], 10);

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

    audioTimer += Time::GetDeltaTime();

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
    if (audioTimer > 0.3f) {
        Audio::Event("thanos_damaged");
        audioTimer = 0.0f;
    }
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
        if (shotgunLevel > 2) TakeDamage(15.0f * currentBombNum);
        else TakeDamage(10.0f * currentBombNum);
        currentBombNum = 0;
        bomb.SetActive(false);
        API_GameObject particles = GetFirstInactiveBombParticle();
        particles.SetActive(true);
        particles.GetTransform().SetPosition(gameObject.GetTransform().GetGlobalPosition());
        particles.GetParticleSystem().Play();
        StickyBombParticle* script = (StickyBombParticle*)particles.GetScript("StickyBombParticle");
        script->time = 0.1f;
        Audio::Event("sticky_bomb");
    }
}

void ThanosLoop::AddBurn()
{
    burnTime += Time::GetDeltaTime();
    if (burnTime > 3.0f) burnTime = 6.0f;
    resetBurn = 0.2f;
}

API_GameObject ThanosLoop::GetFirstInactiveBombParticle()
{
    for (size_t i = 0; i < 10; i++)
    {
        if (!bombParticles[i].IsActive()) return bombParticles[i];
    }

    return bombParticles[0];
}