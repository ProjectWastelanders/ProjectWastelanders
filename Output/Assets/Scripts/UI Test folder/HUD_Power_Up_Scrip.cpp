#include "HUD_Power_Up_Scrip.h"
HELLO_ENGINE_API_C HUD_Power_Up_Scrip* CreateHUD_Power_Up_Scrip(ScriptToInspectorInterface* script)
{
    HUD_Power_Up_Scrip* classInstance = new HUD_Power_Up_Scrip();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragBoxGameObject("Player Stats GO", &classInstance->playerStatsGO);
    //Textures
    script->AddDragBoxTextureResource("Texture_SPEED_INCREASE", &classInstance->Texture_Pick_up[0]);
    script->AddDragBoxTextureResource("Texture_FIRERATE_INCREASE", &classInstance->Texture_Pick_up[1]);
    script->AddDragBoxTextureResource("Texture_SHIELD", &classInstance->Texture_Pick_up[2]);
    script->AddDragBoxTextureResource("Texture_SLOW_TIME", &classInstance->Texture_Pick_up[3]);

    //GameObjects
    script->AddDragBoxGameObject("GamesObjects_1", &classInstance->Gameobjects_Pickables[0]);
    script->AddDragBoxGameObject("GamesObjects_2", &classInstance->Gameobjects_Pickables[1]);
    script->AddDragBoxGameObject("GamesObjects_3", &classInstance->Gameobjects_Pickables[2]);
    script->AddDragBoxGameObject("GamesObjects_4", &classInstance->Gameobjects_Pickables[3]);


    return classInstance;
}

void HUD_Power_Up_Scrip::Start()
{
    playerStats = (PlayerStats*)playerStatsGO.GetScript("PlayerStats");
    if (playerStats == nullptr) Console::Log("PlayerStats missing in HUD_Power_Up_Scrip Script.");

    for (int i = 0; i < 4; i++)
    {
        timer_power_up[i] = 0;
        max_timer_power_up[i] = 0;
        Material_Pick_up[i] = Gameobjects_Pickables[i].GetMaterialCompoennt();
        Gameobjects_Pickables[i].SetActive(false);
    }
}

void HUD_Power_Up_Scrip::Update()
{
    float dt;
    if (playerStats && playerStats->slowTimePowerUp > 0.0f /*&& !paused*/) dt = Time::GetRealTimeDeltaTime();
    else dt = Time::GetDeltaTime();

    for (int i = 0; i < 4; i++)
    {
        if (Gameobjects_Pickables[i].IsActive() && Current_texture[i] != Texture_Pick_up[2]) // shield texture
        {
            timer_power_up[i] -= dt;
            if (timer_power_up[i] <= 0.0f)
            {
                timer_power_up[i] = 0.0f;
                RealocatePowerUps(i);
            }
        }
    }
}

void HUD_Power_Up_Scrip::RealocatePowerUps(int removedIndex)
{
    for (int i = 3; i >= 0; i--) // disable last power up in hud
    {
        if (Gameobjects_Pickables[i].IsActive())
        {
            Gameobjects_Pickables[i].SetActive(false);
            break;
        }
    }

    for (int i = 0; i < 4; i++)
    {
        if (!Gameobjects_Pickables[i].IsActive())
        {
            return;
        }
        else if (i >= removedIndex)
        {
            Material_Pick_up[i].ChangeAlbedoTexture(Current_texture[i + 1]);
            Current_texture[i] = Current_texture[i + 1];
            timer_power_up[i] = timer_power_up[i + 1];
            max_timer_power_up[i] = max_timer_power_up[i + 1];
        }
    }
}

void HUD_Power_Up_Scrip::AddPowerUp(PowerUp_Type Powertype, float duration)
{
    for (int i = 0; i < 4; i++)
    {
        if (Current_texture[i] == Texture_Pick_up[(uint)Powertype])
        {
            timer_power_up[i] = duration;
            return;
        }

        if (!Gameobjects_Pickables[i].IsActive())
        {
            Material_Pick_up[i].ChangeAlbedoTexture(Texture_Pick_up[(uint)Powertype]);
            Current_texture[i] = Texture_Pick_up[(uint)Powertype];
            timer_power_up[i] = duration;
            max_timer_power_up[i] = duration;
            Gameobjects_Pickables[i].SetActive(true);
            return;
        }
    }
}

void HUD_Power_Up_Scrip::RemovePowerUp(PowerUp_Type Powertype)
{
    for (int i = 0; i < 4; i++)
    {
        if (Gameobjects_Pickables[i].IsActive() && Current_texture[i] == Texture_Pick_up[(uint)Powertype])
        {
            RealocatePowerUps(i);
            return;
        }
    }
}