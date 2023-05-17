#include "BarrierWithButton.h"
#include "ButtonBarrier.h"
HELLO_ENGINE_API_C BarrierWithButton* CreateBarrierWithButton(ScriptToInspectorInterface* script)
{
    BarrierWithButton* classInstance = new BarrierWithButton();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragBoxGameObject("ButtonBarrier", &classInstance->buttonGO);
    script->AddDragBoxGameObject("Lamp 1", &classInstance->lamp1);
    script->AddDragBoxGameObject("Lamp 2", &classInstance->lamp2);
    script->AddDragBoxTextureResource("Lamps Texture Spawn Alive", &classInstance->materialLamps);
    script->AddDragBoxTextureResource("Lamp 1 Text Spawn Destroy", &classInstance->materialLamp1);
    script->AddDragBoxTextureResource("Lamp 2 Text Spawn Destroy", &classInstance->materialLamp2);
    script->AddDragBoxRigidBody("Barrier Collider", &classInstance->barrierRb);
    return classInstance;
}

void BarrierWithButton::Start()
{
    button = (ButtonBarrier*)buttonGO.GetScript("ButtonBarrier");
    if (button == nullptr) Console::Log("ButtonBarrier missing in BarrierWithButton Script.");
    lamp1.GetMaterialCompoennt().ChangeAlbedoTexture(materialLamps);
    lamp2.GetMaterialCompoennt().ChangeAlbedoTexture(materialLamps);
}

void BarrierWithButton::Update()
{
    if (!button) return;

    if (button->isOver && !opened)
    {
        lamp1.GetMaterialCompoennt().ChangeAlbedoTexture(materialLamp1);
        lamp2.GetMaterialCompoennt().ChangeAlbedoTexture(materialLamp2);
        barrierRb.GetGameObject().GetMeshRenderer().SetActive(false);
        barrierRb.SetTrigger(true);
        opened = true;
    }
}