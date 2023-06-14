#include "MenuButtons.h"
#include "Audio/S_AudioMixer.h"
#include "UI Test folder/PlayCreditsMainMenu.h"
HELLO_ENGINE_API_C MenuButtons* CreateMenuButtons(ScriptToInspectorInterface* script)
{
    MenuButtons* classInstance = new MenuButtons();
    script->AddDragBoxUIButton("Continue", &classInstance->Continue);
    script->AddDragBoxUIButton("NewGame", &classInstance->NewGame);
    script->AddDragBoxUIButton("Exit", &classInstance->Exit);
    script->AddDragBoxUIButton("Settings", &classInstance->Settings);
    script->AddDragBoxUIButton("Credits", &classInstance->Credits);

    script->AddDragBoxUIInput("Panel Main Menu", &classInstance->mainPanel);
    script->AddDragBoxGameObject("Panel Settings Menu", &classInstance->settingsPanel);

    return classInstance;
}

void MenuButtons::Init()
{
    credits = (PlayCreditsMainMenu*)Game::FindGameObject("Credits").GetScript("PlayCreditsMainMenu");
    // Initialize Audio quicksave parameters:
    S_AudioMixer::SetMasterVolume(API_QuickSave::GetFloat("MasterVolume", 1.0f));
    S_AudioMixer::SetMusicVolume(API_QuickSave::GetFloat("MusicVolume", 100.0f));
    S_AudioMixer::SetSFXVolume(API_QuickSave::GetFloat("SFXVolume", 100.0f));
}

void MenuButtons::Start()
{
    settingsActive = true;
    Audio::Event("main_menu_song");
}
void MenuButtons::Update()
{
    if (shouldActivate)
    {
        shouldActivate = false;
        mainPanel.SetEnable(true);
    }

    if (Continue.OnPress())
    {
        // If we completed at least level 1, we must have a current save active.
        if (API_QuickSave::GetBool("level1_completed"))
        {
            Scene::LoadScene("SpaceshipHUB_Scene.HScene");
        }
   
    }
    if (NewGame.OnPress())
    {
        float tempSFX = API_QuickSave::GetFloat("SFXVolume", 100.0f);
        float tempMus = API_QuickSave::GetFloat("MusicVolume", 100.0f);
        float tempMas = API_QuickSave::GetFloat("MasterVolume", 1.0f);

        API_QuickSave::ResetAll();

        S_AudioMixer::SetMasterVolume(tempMas);
        S_AudioMixer::SetMusicVolume(tempMus);
        S_AudioMixer::SetSFXVolume(tempSFX);

        API_QuickSave::SetInt("equipedNormalGun", 2); // starter weapon

        API_QuickSave::SetFloat("PlayerPosX", 110.5f);
        API_QuickSave::SetFloat("PlayerPosY", 0.0f);
        API_QuickSave::SetFloat("PlayerPosZ", -29.2f);

        API_QuickSave::SetBool("GameStart", true);

        API_QuickSave::SetBool("level1_completed", false);
        API_QuickSave::SetBool("level2_completed", false);
        API_QuickSave::SetBool("level3_completed", false);
        API_QuickSave::SetBool("level4_completed", false);

        API_QuickSave::SetBool("level1Selected", true);
        API_QuickSave::SetBool("level2Selected", false);
        API_QuickSave::SetBool("level3Selected", false);
        API_QuickSave::SetBool("level4Selected", false);

        Scene::LoadScene("Level1.HScene");


    }
    if (Exit.OnPress())
    {
        Game::ExitApplication();
    }
    if (Settings.OnPress() && settingsActive)
    {
        settingsPanel.SetActive(true);
        mainPanel.GetGameObject().SetActive(false);
    }

    if (Credits.OnPress())
    {
        if (credits && !credits->justPlayed)
            credits->PlayCinematic();
    }

    if (Input::GetGamePadButton(GamePadButton::BUTTON_B) == KeyState::KEY_DOWN)
    {
        if (settingsPanel.IsActive())
        {
            settingsPanel.SetActive(false);
            mainPanel.GetGameObject().SetActive(true);
            settingsActive = true;
        }
    }
}

