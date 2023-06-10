#include "ActiveShotsDiviner.h"
HELLO_ENGINE_API_C ActiveShotsDiviner* CreateActiveShotsDiviner(ScriptToInspectorInterface* script)
{
	ActiveShotsDiviner* classInstance = new ActiveShotsDiviner();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddCheckBox("Online?", &classInstance->canHearShoot);
	return classInstance;
}

void ActiveShotsDiviner::Start()
{
     Game::FindGameObjectsWithTag("Milano", &ship, 1);
     shipScript = (ReachTheSpaceship*)ship.GetScript("ReachTheSpaceship");
}
void ActiveShotsDiviner::Update()
{

}

void ActiveShotsDiviner::OnCollisionEnter(API::API_RigidBody other)
{

    std::string detectionTag = other.GetGameObject().GetTag();
   
   
    
    if (detectionTag == "Player")
    {
       
        if (shipScript)
        {
            if (shipScript->enabled )
            {
                Console::Log("can hear shootssss");
                canHearShoot = true;
            }
        }
    }
}
