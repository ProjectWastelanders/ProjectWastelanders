#include "StartOfLvl3.h"
HELLO_ENGINE_API_C StartOfLvl3* CreateStartOfLvl3(ScriptToInspectorInterface* script)
{
	StartOfLvl3* classInstance = new StartOfLvl3();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragBoxUIImage("Dialog 1", &classInstance->Dialog_1);
    script->AddDragBoxUIImage("Dialog 2", &classInstance->Dialog_2);
    script->AddDragBoxUIImage("Dialog 3", &classInstance->Dialog_3);
    script->AddDragBoxUIImage("Dialog 4", &classInstance->Dialog_4);

    script->AddDragFloat("Dialog timer", &classInstance->timer);

    script->AddDragFloat("Dialog Pos X", &classInstance->finalPos.x);
    script->AddDragFloat("Dialog Pos Y", &classInstance->finalPos.y);
    script->AddDragFloat("Dialog Pos Z", &classInstance->finalPos.z);
	return classInstance;
}

void StartOfLvl3::Start()
{
    nextDialog = false;
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
}
void StartOfLvl3::Update()
{
    if (activeDialogs) {

        switch (currentDialog)
        {
        case 0:
            PrintDialog(Dialog_1, "starlord_surprised");
            break;
        case 1:
            PrintDialog(Dialog_2, "groot_surprised");
            break;
        case 2:
            PrintDialog(Dialog_3, "rocket_annoyed");
            break;
        case 3:
            PrintDialog(Dialog_4, "starlord_sorry");
            break;
        default:
            break;
        }
    }
}

void StartOfLvl3::PrintDialog(API_UIImage& Dialog, string audioEvent)
{
    Dialog.GetGameObject().SetActive(true);
    if (_timer >= timer) {
        if (Dialog.GetGameObject().GetTransform().GetGlobalPosition().y > initalPos.y)
        {
            movingPos.y -= 1 * Time::GetDeltaTime();
        }
        else {
            if (currentDialog == 3) {
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

void StartOfLvl3::OnCollisionEnter(API::API_RigidBody other)
{
    std::string detectionTag = other.GetGameObject().GetTag();
    if (detectionTag == "Player")
    {
        activeDialogs = true;
    }
}