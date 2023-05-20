#include "AidKitRotation.h"
HELLO_ENGINE_API_C AidKitRotation* CreateAidKitRotation(ScriptToInspectorInterface* script)
{
    AidKitRotation* classInstance = new AidKitRotation();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragFloat("X Rotation", &classInstance->rorationX);
    script->AddDragFloat("Y Rotation", &classInstance->rorationY);
    script->AddDragFloat("Z Rotation", &classInstance->rorationZ);
    return classInstance;
}

void AidKitRotation::Start()
{

}
void AidKitRotation::Update()
{
    gameObject.GetTransform().Rotate(rorationX, rorationY, rorationZ);
}