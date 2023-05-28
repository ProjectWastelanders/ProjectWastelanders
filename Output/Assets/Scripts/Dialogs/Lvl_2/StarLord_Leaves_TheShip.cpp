#include "StarLord_Leaves_TheShip.h"
HELLO_ENGINE_API_C StarLord_Leaves_TheShip* CreateStarLord_Leaves_TheShip(ScriptToInspectorInterface* script)
{
	StarLord_Leaves_TheShip* classInstance = new StarLord_Leaves_TheShip();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);

    script->AddDragBoxUIImage("Dialog 1", &classInstance->Dialog_1);
    script->AddDragBoxUIImage("Dialog 2", &classInstance->Dialog_2);
    script->AddDragBoxUIImage("Dialog 3", &classInstance->Dialog_3);
    script->AddDragBoxUIImage("Dialog 4", &classInstance->Dialog_4);
    script->AddDragBoxUIImage("Dialog 5", &classInstance->Dialog_5);
    script->AddDragBoxUIImage("Dialog 6", &classInstance->Dialog_6);

    script->AddDragFloat("Dialog timer", &classInstance->timer);

    script->AddDragFloat("Dialog Pos X", &classInstance->finalPos.x);
    script->AddDragFloat("Dialog Pos Y", &classInstance->finalPos.y);
    script->AddDragFloat("Dialog Pos Z", &classInstance->finalPos.z);

	return classInstance;
}

void StarLord_Leaves_TheShip::Start()
{
    nextDialog = false;
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
}
void StarLord_Leaves_TheShip::Update()
{
    if (activeDialogs) {

        switch (currentDialog)
        {
        case 1:
            PrintDialog(Dialog_1);
            break;
        case 2:
            PrintDialog(Dialog_2);
            break;
        case 3:
            PrintDialog(Dialog_3);
            break;
        case 4:
            PrintDialog(Dialog_4);
            break;
        case 5:
            PrintDialog(Dialog_5);
            break;
        case 6:
            PrintDialog(Dialog_6);
            break;
        default:
            break;
        }
    }
}

void StarLord_Leaves_TheShip::PrintDialog(API_UIImage& Dialog)
{
    Dialog.GetGameObject().SetActive(true);
    if (_timer >= timer) {
        if (Dialog.GetGameObject().GetTransform().GetGlobalPosition().y > initalPos.y)
        {
            movingPos.y -= 1 * Time::GetDeltaTime();
        }
        else {
            if (currentDialog == 6) {
                Dialog.GetGameObject().SetActive(false);
                activeDialogs = false;
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

void StarLord_Leaves_TheShip::OnCollisionEnter(API::API_RigidBody other)
{
    std::string detectionTag = other.GetGameObject().GetTag();
    if (detectionTag == "Player")
    {
        activeDialogs = true;
    }
}
