#include "stealDivinerDialog.h"
#include "../../Quests/StealTheDivinerAgain.h"
#include "../../Quests/ReachTheSpaceship.h"
HELLO_ENGINE_API_C stealDivinerDialog* CreatestealDivinerDialog(ScriptToInspectorInterface* script)
{
	stealDivinerDialog* classInstance = new stealDivinerDialog();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragBoxUIImage("Dialog 1", &classInstance->Dialog_1);
    script->AddDragBoxUIImage("Dialog 2", &classInstance->Dialog_2);
    script->AddDragBoxUIImage("Dialog 3", &classInstance->Dialog_3);
    script->AddDragBoxUIImage("Dialog 4", &classInstance->Dialog_4);
    script->AddDragBoxUIImage("Dialog 5", &classInstance->Dialog_5);
    script->AddDragBoxUIImage("Dialog 6", &classInstance->Dialog_6);
    script->AddDragBoxUIImage("Dialog 7", &classInstance->Dialog_7);
    script->AddDragBoxUIImage("Dialog 8", &classInstance->Dialog_8);
    script->AddDragBoxUIImage("Dialog 9", &classInstance->Dialog_9);
    script->AddDragBoxUIImage("Dialog 10", &classInstance->Dialog_10);
    script->AddDragBoxUIImage("Dialog 11", &classInstance->Dialog_11);
    script->AddDragBoxUIImage("Dialog 12", &classInstance->Dialog_12);

    script->AddDragFloat("Dialog timer", &classInstance->timer);

    script->AddDragFloat("Dialog Pos X", &classInstance->finalPos.x);
    script->AddDragFloat("Dialog Pos Y", &classInstance->finalPos.y);
    script->AddDragFloat("Dialog Pos Z", &classInstance->finalPos.z);

    script->AddDragBoxGameObject("Reach The Spaceship GO", &classInstance->reachTheSpaceShipGO);

	return classInstance;
}

void stealDivinerDialog::Start()
{
    reachTheSpaceShip = (ReachTheSpaceship*)reachTheSpaceShipGO.GetScript("ReachTheSpaceship");
    if (reachTheSpaceShip == nullptr) Console::Log("ReachTheSpaceship missing in StealTheDivinerAgain Script.");

    activeDialogs = false;

    initalPos = { 0, -1.500, 0 };
    movingPos = { 0, -1.500, 0 };
    //finalPos = { 0, -0.500, 0 };

    currentDialog = 1;

    Dialog_1.GetGameObject().GetTransform().SetPosition(initalPos);
    Dialog_1.GetGameObject().SetActive(false);

    Dialog_2.GetGameObject().GetTransform().SetPosition(initalPos);
    Dialog_2.GetGameObject().SetActive(false);

    Dialog_3.GetGameObject().GetTransform().SetPosition(initalPos);
    Dialog_3.GetGameObject().SetActive(false);

    Dialog_4.GetGameObject().GetTransform().SetPosition(initalPos);
    Dialog_4.GetGameObject().SetActive(false);

    Dialog_5.GetGameObject().GetTransform().SetPosition(initalPos);
    Dialog_5.GetGameObject().SetActive(false);

    Dialog_6.GetGameObject().GetTransform().SetPosition(initalPos);
    Dialog_6.GetGameObject().SetActive(false);

    Dialog_7.GetGameObject().GetTransform().SetPosition(initalPos);
    Dialog_7.GetGameObject().SetActive(false);

    Dialog_8.GetGameObject().GetTransform().SetPosition(initalPos);
    Dialog_8.GetGameObject().SetActive(false);

    Dialog_9.GetGameObject().GetTransform().SetPosition(initalPos);
    Dialog_9.GetGameObject().SetActive(false);

    Dialog_10.GetGameObject().GetTransform().SetPosition(initalPos);
    Dialog_10.GetGameObject().SetActive(false);

    Dialog_11.GetGameObject().GetTransform().SetPosition(initalPos);
    Dialog_11.GetGameObject().SetActive(false);

    Dialog_12.GetGameObject().GetTransform().SetPosition(initalPos);
    Dialog_12.GetGameObject().SetActive(false);
}
void stealDivinerDialog::Update()
{
    
    if (activeDialogs) {

        switch (currentDialog)
        {
        case 1:
            PrintDialog(Dialog_1);
            Audio::Event("starlord_confident");
            break;
        case 2:
            PrintDialog(Dialog_2);
            Audio::Event("collector_help");
            break;
        case 3:
            PrintDialog(Dialog_3);
            Audio::Event("starlord_dubitative");
            break;
        case 4:
            PrintDialog(Dialog_4);
            Audio::Event("collector_help");
            break;
        case 5:
            PrintDialog(Dialog_5);
            Audio::Event("starlord_surprised");
            break;
        case 6:
            PrintDialog(Dialog_6);
            Audio::Event("collector_hey");
            break;
        case 7:
            PrintDialog(Dialog_7);
            Audio::Event("starlord_surprised");
            break;
        case 8:
            PrintDialog(Dialog_8);
            Audio::Event("collector_hey");
            break;
        case 9:
            PrintDialog(Dialog_9);
            Audio::Event("gamora_warning");
            break;
        case 10:
            PrintDialog(Dialog_10);
            Audio::Event("starlord_confident");
            break;
        case 11:
            PrintDialog(Dialog_11);
            Audio::Event("rocket_annoyed");
            break;
        case 12:
            PrintDialog(Dialog_12);
            Audio::Event("starlord_confident");
            break;
        default:
            break;
        }
    }   
    
}

void stealDivinerDialog::PrintDialog(API_UIImage& Dialog)
{
    Dialog.GetGameObject().SetActive(true);
    if (_timer >= timer) {
        if (Dialog.GetGameObject().GetTransform().GetGlobalPosition().y > initalPos.y)
        {
            movingPos.y -= 1 * Time::GetDeltaTime();
        }
        else {
            if (currentDialog == 12) {
                Dialog.GetGameObject().SetActive(false);
                activeDialogs = false;
                if (reachTheSpaceShip) reachTheSpaceShip->EnableMision();
            }
            else {
                currentDialog += 1;
                Dialog.GetGameObject().SetActive(false);
                _timer = 0;
            }
        }
    }
    else {
        if (Dialog.GetGameObject().GetTransform().GetGlobalPosition().y < finalPos.y)
        {
            movingPos.y += 1 * Time::GetDeltaTime();
        }
        else
        {
            _timer += Time::GetDeltaTime();
        }
    }

    Dialog.GetGameObject().GetTransform().SetPosition(movingPos);
}