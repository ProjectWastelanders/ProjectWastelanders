#include "Mision_TheRuleOfFive.h"
#include "../../Quests/Secondary_Quests/EnemyDieEvent.h"
#include "../../Player/PlayerStorage.h"

HELLO_ENGINE_API_C Mision_TheRuleOfFive* CreateMision_TheRuleOfFive(ScriptToInspectorInterface* script)
{
    Mision_TheRuleOfFive* classInstance = new Mision_TheRuleOfFive();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);

    script->AddDragBoxGameObject("Player Storage", &classInstance->playerStorageGO);
    script->AddCheckBox("misioncompled", &classInstance->misionCompleted);
    script->AddDragFloat("ruleof5timer", &classInstance->ruleOfFiveTimer);

    return classInstance;
}

void Mision_TheRuleOfFive::Start()
{

    playerStorage = (PlayerStorage*)playerStorageGO.GetScript("PlayerStorage");
    if (playerStorage == nullptr) Console::Log("PlayerStorage missing in GetDiviner Script.");

    misionCompleted = false;
    EnemyDieEvent::numOfEnemiesDead = 0;
}
void Mision_TheRuleOfFive::Update()
{


    if (misionCompleted)
    {

        Console::Log("MISSION 4 COMPLETED CONGRATULATIONS!");

    }
    else
    {
        if (EnemyDieEvent::numOfEnemiesDead >= 1)
        {
            ruleOfFiveTimer += Time::GetDeltaTime();
        }


        if (ruleOfFiveTimer < 5.0f && EnemyDieEvent::numOfEnemiesDead >= 2)
        {

            misionCompleted = true;
            playerStorage->skillPoints += 2;
            playerStorage->SaveData();

        }
        else if (ruleOfFiveTimer > 5.0f)
        {

            EnemyDieEvent::numOfEnemiesDead = 0;
            ruleOfFiveTimer = 0;

            misionCompleted = false;

        }
    }

}