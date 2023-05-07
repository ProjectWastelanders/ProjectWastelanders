#include "SpawnerArea.h"
HELLO_ENGINE_API_C SpawnerArea* CreateSpawnerArea(ScriptToInspectorInterface* script)
{
    SpawnerArea* classInstance = new SpawnerArea();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    return classInstance;
}

void SpawnerArea::Start()
{

}

void SpawnerArea::Update()
{

}

void SpawnerArea::OnCollisionEnter(API_RigidBody other)
{
    std::string detectionTag = other.GetGameObject().GetTag();

    if (detectionTag == "Player")
    {
        playerIn = true;
    }
}

void SpawnerArea::OnCollisionExit(API_RigidBody other)
{
    std::string detectionTag = other.GetGameObject().GetTag();

    if (detectionTag == "Player")
    {
        playerIn = false;
    }
}