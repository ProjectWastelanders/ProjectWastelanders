#include "GoToLevelTrigger.h"
HELLO_ENGINE_API_C GoToLevelTrigger* CreateGoToLevelTrigger(ScriptToInspectorInterface* script)
{
    GoToLevelTrigger* classInstance = new GoToLevelTrigger();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
   /*script->AddCheckBox("Is Hub Scene", &classInstance->isHub);
    script->AddDragInt("(IF NO HUB) Current Level", &classInstance->nextLevel);
    script->AddInputBox("Scene path and name: ", &classInstance->scene);*/
    script->AddDragBoxGameObject("Active Panel", &classInstance->panel);
    return classInstance;
}

void GoToLevelTrigger::Start()
{
    /*if (isHub)
    {
        if (API_QuickSave::GetBool("level4_completed"))
        {
            scene = std::string("LVL5_Blockout");
            nextLevel = 5;
        }
        else if (API_QuickSave::GetBool("level3_completed"))
        {
            scene = std::string("LVL4_Blockout.HScene");
            nextLevel = 4;
        }
        if (API_QuickSave::GetBool("level2_completed"))
        {
            scene = std::string("LVL5_Blockout.HScene");
            nextLevel = 3;
        }
        else if (API_QuickSave::GetBool("level1_completed"))
        {
            scene = std::string("LVL2_Blockout.HScene");
            nextLevel = 2;
        }
        else
        {
            scene = std::string("LVL1_Blockout.HScene");
            nextLevel = 1;
        } 
    }*/
}

void GoToLevelTrigger::Update()
{

}

void GoToLevelTrigger::OnCollisionEnter(API_RigidBody other)
{
    std::string detectionName = other.GetGameObject().GetName();
    if (detectionName == "Player")
    {
        panel.SetActive(true);

        /*if (isHub)
        {
            if (API_QuickSave::GetBool("IsInMiddleOfLevel") == false)
            {
                switch (nextLevel)
                {
                case 1:
                    API_QuickSave::SetFloat("PlayerPosX", 110.5f);
                    API_QuickSave::SetFloat("PlayerPosY", 0.0f);
                    API_QuickSave::SetFloat("PlayerPosZ", -29.2f);
                    API_QuickSave::SetFloat("PlayerIndicatorPosX", -0.7f);
                    API_QuickSave::SetFloat("PlayerIndicatorPosY", -0.39f);
                    API_QuickSave::SetBool("IsInMiddleOfLevel", false);
                    break;
                case 2:
                    API_QuickSave::SetFloat("PlayerPosX", 147.6f);
                    API_QuickSave::SetFloat("PlayerPosY", 2.115f);
                    API_QuickSave::SetFloat("PlayerPosZ", 14.54f);
                    API_QuickSave::SetFloat("PlayerIndicatorPosX", 0);
                    API_QuickSave::SetFloat("PlayerIndicatorPosY", 0);
                    API_QuickSave::SetBool("IsInMiddleOfLevel", false);
                    break;
                case 3:
                    API_QuickSave::SetFloat("PlayerPosX", -61.7f);
                    API_QuickSave::SetFloat("PlayerPosY", 92.5f);
                    API_QuickSave::SetFloat("PlayerPosZ", 47.3f);
                    API_QuickSave::SetFloat("PlayerIndicatorPosX", 0);
                    API_QuickSave::SetFloat("PlayerIndicatorPosY", 0);
                    API_QuickSave::SetBool("IsInMiddleOfLevel", false);
                    break;
                default:
                    break;
                }
            }
            else
            {
            }
        }
        else
        {
            API_QuickSave::SetBool("level" + std::to_string(nextLevel) + "_completed", true);
        }
        API_QuickSave::SetBool("ComesFromHub", true);
        Scene::LoadScene(scene.c_str());*/
    }
}
