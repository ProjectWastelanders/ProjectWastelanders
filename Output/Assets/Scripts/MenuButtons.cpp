#include "MenuButtons.h"

HELLO_ENGINE_API_C MenuButtons* CreateMenuButtons(ScriptToInspectorInterface* script)
{
    MenuButtons* classInstance = new MenuButtons();
    script->AddDragBoxUIButton("Continue", &classInstance->Continue);
    script->AddDragBoxUIButton("NewGame", &classInstance->NewGame);
    script->AddDragBoxUIButton("Exit", &classInstance->Exit);
    script->AddDragBoxUIButton("Settings", &classInstance->Settings);
    script->AddDragBoxUIButton("Collectibles", &classInstance->Collectibles);

    return classInstance;
}

void MenuButtons::Start()
{

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
    if (Collectibles.OnPress())
    {
    }
    if (Settings.OnPress())
    {
    }
}