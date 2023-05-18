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

    script->AddDragBoxUIImage("Dialog", &classInstance->Dialog);
    script->AddDragFloat("Dialog timer", &classInstance->dialogTimer);

    return classInstance;
}

void Mision_TheRuleOfFive::Start()
{

    playerStorage = (PlayerStorage*)playerStorageGO.GetScript("PlayerStorage");
    if (playerStorage == nullptr) Console::Log("PlayerStorage missing in GetDiviner Script.");

    initalPos = { 0, -1.500, 0 };
    movingPos = { 0, -1.500, 0 };
    finalPos = { 0, -0.500, 0 };

    misionCompleted = false;
    EnemyDieEvent::numOfEnemiesDead = 0;
}
void Mision_TheRuleOfFive::Update()
{


    if (misionCompleted)
    {
        PrintDialog(Dialog);
        Console::Log("MISSION 4 COMPLETED CONGRATULATIONS!");
    }
    else
    {
        if (EnemyDieEvent::numOfEnemiesDead >= 1)
        {
            ruleOfFiveTimer += Time::GetDeltaTime();
        }


        if (ruleOfFiveTimer < 5.0f && EnemyDieEvent::numOfEnemiesDead >= 5)
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

void Mision_TheRuleOfFive::PrintDialog(API_UIImage& Dialog)
{
    Dialog.GetGameObject().SetActive(true);

    if (_dialogTimer >= dialogTimer) {
        if (Dialog.GetGameObject().GetTransform().GetGlobalPosition().y > initalPos.y)
        {
            movingPos.y -= 1 * Time::GetDeltaTime();
        }
        else {
            Dialog.GetGameObject().SetActive(false);
        }
    }
    else {
        if (Dialog.GetGameObject().GetTransform().GetGlobalPosition().y < finalPos.y)
        {
            movingPos.y += 1 * Time::GetDeltaTime();
        }
        else
        {
            _dialogTimer += Time::GetDeltaTime();
        }
    }

    Dialog.GetGameObject().GetTransform().SetPosition(movingPos);
}
