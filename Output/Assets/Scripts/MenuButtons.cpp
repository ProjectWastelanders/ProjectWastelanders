#include "MenuButtons.h"

HELLO_ENGINE_API_C MenuButtons* CreateMenuButtons(ScriptToInspectorInterface* script)
{
    MenuButtons* classInstance = new MenuButtons();
    script->AddDragBoxUIButton("Continue", &classInstance->Continue);
    script->AddDragBoxUIButton("NewGame", &classInstance->NewGame);
    script->AddDragBoxUIButton("Exit", &classInstance->Exit);
    script->AddDragBoxUIButton("Settings", &classInstance->Settings);

    script->AddDragBoxGameObject("Panel Main Menu", &classInstance->mainPanel);
    script->AddDragBoxGameObject("Panel Settings Menu", &classInstance->settingsPanel);

    return classInstance;
}

void MenuButtons::Start()
{
    settingsActive = true;
}
void MenuButtons::Update()
{
    if (Continue.OnPress())
    {
        // If we completed at least level 1, we must have a current save active.
        if (API_QuickSave::GetBool("level1_completed"))
        {
            Scene::LoadScene("SpaceshipHUB_Scene.HScene");
        }
        else
            Audio::Event("error");
    }
    if (NewGame.OnPress())
    {
        API_QuickSave::ResetAll();
        API_QuickSave::SetInt("equipedNormalGun", 2); // starter weapon

        API_QuickSave::SetFloat("PlayerPosX", 110.5f);
        API_QuickSave::SetFloat("PlayerPosY", 0.0f);
        API_QuickSave::SetFloat("PlayerPosZ", -29.2f);

        Scene::LoadScene("Level1.HScene");

        API_QuickSave::SetBool("level1_completed", false);
        API_QuickSave::SetBool("level2_completed", false);
        API_QuickSave::SetBool("level3_completed", false);
        API_QuickSave::SetBool("level4_completed", false);

        API_QuickSave::SetBool("level1Selected", true);
        API_QuickSave::SetBool("level2Selected", false);
        API_QuickSave::SetBool("level3Selected", false);
        API_QuickSave::SetBool("level4Selected", false);

    }
    if (Exit.OnPress())
    {
        Game::ExitApplication();
    }
    if (Settings.OnPress() && settingsActive)
    {
        settingsPanel.SetActive(true);
        mainPanel.SetActive(false);
    }

    if (Input::GetGamePadButton(GamePadButton::BUTTON_B) == KeyState::KEY_DOWN)
    {
        if (settingsPanel.IsActive())
        {
            settingsPanel.SetActive(false);
            mainPanel.SetActive(true);
            settingsActive = true;
        }
    }
}