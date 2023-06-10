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
    activeAudio = true;

    initalPos = { 0, -1.500, 0 };
    movingPos = { 0, -1.500, 0 };
    //finalPos = { 0, -0.500, 0 };

    currentDialog = 0;

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
}
void stealDivinerDialog::Update()
{
    
    if (activeDialogs) {

        switch (currentDialog)
        {
        case 0:
            PrintDialog(Dialog_1, "collector_hey");
            break;
        case 1:
            PrintDialog(Dialog_2, "starlord_dubitative");         
            break;
        case 2:
            PrintDialog(Dialog_3, "collector_help");
            break;
        case 3:
            PrintDialog(Dialog_4, "starlord_surprised");
            break;
        case 4:
            PrintDialog(Dialog_5, "collector_hey");
            break;
        case 5:
            PrintDialog(Dialog_6, "collector_help");
            break;
        case 6:
            PrintDialog(Dialog_7, "gamora_warning");  
            break;
        case 7:
            PrintDialog(Dialog_8, "rocket_annoyed");
            break;
        default:
            break;
        }
    }   
    
}

void stealDivinerDialog::PrintDialog(API_UIImage& Dialog, string audioEvent)
{
    Dialog.GetGameObject().SetActive(true);
    if (_timer >= timer) {
        if (Dialog.GetGameObject().GetTransform().GetGlobalPosition().y > initalPos.y)
        {
            movingPos.y -= 1 * Time::GetDeltaTime();
        }
        else {
            if (currentDialog == 6)
            {
                if (reachTheSpaceShip) reachTheSpaceShip->EnableMision();
            }
            else if (currentDialog == 7) {
                Dialog.GetGameObject().SetActive(false);
                activeDialogs = false;                
            }
            else {
                currentDialog += 1;
                activeAudio = true;
                Dialog.GetGameObject().SetActive(false);
                _timer = 0;
            }
        }
    }
    else {
        if (activeAudio) {
            Audio::Event(audioEvent.c_str());
            activeAudio = false;
        }
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