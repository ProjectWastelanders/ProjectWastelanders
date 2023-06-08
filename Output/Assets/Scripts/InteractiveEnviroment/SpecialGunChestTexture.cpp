#include "SpecialGunChestTexture.h"
#include "Chest.h"
HELLO_ENGINE_API_C SpecialGunChestTexture* CreateSpecialGunChestTexture(ScriptToInspectorInterface* script)
{
    SpecialGunChestTexture* classInstance = new SpecialGunChestTexture();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragBoxGameObject("Chest Logic reference", &classInstance->chestLogicReference);
    return classInstance;
}

void SpecialGunChestTexture::Start()
{
    render = gameObject.GetMeshRenderer();

    chestScript = (Chest*)chestLogicReference.GetScript("Chest");
    if (chestScript == nullptr) Console::Log("Chest Script missing on SpecialGunChestTexture GO");
}

void SpecialGunChestTexture::Update()
{
    if (!chestScript) return;
    render.SetActive(chestScript->opened);
}