#include "OrbitalTrigger.h"
#include "CamMov.h"
#include "ShadowManager.h"  

HELLO_ENGINE_API_C OrbitalTrigger* CreateOrbitalTrigger(ScriptToInspectorInterface* script)
{
	OrbitalTrigger* classInstance = new OrbitalTrigger();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragBoxGameObject("Camera", &classInstance->cam);
    script->AddDragBoxGameObject("DirectionalLight", &classInstance->directionalLight);
	return classInstance;
}

void OrbitalTrigger::Start()
{
   camScript = (CamMov*)cam.GetScript("CamMov");
   shadowManager = (ShadowManager*)directionalLight.GetScript("ShadowManager");
}
void OrbitalTrigger::Update()
{

}

void OrbitalTrigger::OnCollisionEnter(API::API_RigidBody other)
{
    std::string detectionTag = other.GetGameObject().GetTag();
    if (detectionTag == "Player")
    {
        if (camScript != nullptr) 
        {
            camScript->SetOrbital(true);

            if (shadowManager != nullptr) shadowManager->SetShadowsHigh();
        }
    }
}

void OrbitalTrigger::OnCollisionExit(API::API_RigidBody other)
{
    std::string detectionTag = other.GetGameObject().GetTag();
    if (detectionTag == "Player")
    {
        if (camScript != nullptr)
        {
            camScript->SetOrbital(false);
            if (shadowManager != nullptr) shadowManager->SetShadowsLow();
        }
    }
}