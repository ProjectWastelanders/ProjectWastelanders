#include "PlayerIndicator.h"
HELLO_ENGINE_API_C PlayerIndicator* CreatePlayerIndicator(ScriptToInspectorInterface* script)
{
    PlayerIndicator* classInstance = new PlayerIndicator();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragBoxGameObject("Map GO", &classInstance->map);
    script->AddDragInt("Level Index", &classInstance->levelIndex);
    script->AddDragBoxGameObject("Player", &classInstance->player);
    script->AddDragFloat("Movement Reduction X", &classInstance->movementReductionX);
    script->AddDragFloat("Movement Reduction Y", &classInstance->movementReductionY);
    return classInstance;
}

void PlayerIndicator::Start()
{
    if (levelIndex == 0) return;
    else if (levelIndex == 1)
    {
        movementReductionX = 249.65f;
        movementReductionY = 122.53;
    }

    gameObject.GetTransform().SetPosition(API_QuickSave::GetFloat("PlayerIndicatorPosX"), API_QuickSave::GetFloat("PlayerIndicatorPosY"), -0.002);

    originIndicatorPos = API_Vector2(gameObject.GetTransform().GetGlobalPosition().x, gameObject.GetTransform().GetGlobalPosition().y);
    originPlayerPos = API_Vector2(player.GetTransform().GetGlobalPosition().x, player.GetTransform().GetGlobalPosition().z);
    map.SetActive(false);
}

void PlayerIndicator::Update()
{
    API_Vector2 variation = originPlayerPos - API_Vector2(player.GetTransform().GetGlobalPosition().x, player.GetTransform().GetGlobalPosition().z);
    float newPosX = originIndicatorPos.x + (variation.x / movementReductionX);
    float newPosY = originIndicatorPos.y - (variation.y / movementReductionY);
    gameObject.GetTransform().SetPosition(newPosX, newPosY, -0.002f);
}

void PlayerIndicator::SavePosition()
{
    if (levelIndex == 0) return;

    API_Vector2 variation = originPlayerPos - API_Vector2(player.GetTransform().GetGlobalPosition().x, player.GetTransform().GetGlobalPosition().z);
    float newPosX = originIndicatorPos.x + (variation.x / movementReductionX);
    float newPosY = originIndicatorPos.y - (variation.y / movementReductionY);
    gameObject.GetTransform().SetPosition(newPosX, newPosY, -0.002f);

    API_QuickSave::SetFloat("PlayerIndicatorPosX", gameObject.GetTransform().GetGlobalPosition().x);
    API_QuickSave::SetFloat("PlayerIndicatorPosY", gameObject.GetTransform().GetGlobalPosition().y);
}