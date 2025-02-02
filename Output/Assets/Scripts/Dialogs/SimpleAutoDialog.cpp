#include "SimpleAutoDialog.h"
HELLO_ENGINE_API_C SimpleAutoDialog* CreateSimpleAutoDialog(ScriptToInspectorInterface* script)
{
	SimpleAutoDialog* classInstance = new SimpleAutoDialog();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);

	script->AddDragBoxUIImage("Dialog", &classInstance->Dialog);

	script->AddDragFloat("Dialog timer", &classInstance->timer);

    script->AddDragFloat("Dialog Pos X", &classInstance->finalPos.x);
    script->AddDragFloat("Dialog Pos Y", &classInstance->finalPos.y);
    script->AddDragFloat("Dialog Pos Z", &classInstance->finalPos.z);

    script->AddInputBox("Audio Event String", &classInstance->audioEvent);

	return classInstance;
}

void SimpleAutoDialog::Start()
{
	activeDialog = false;
    activeAudio = true;

	initalPos = { 0, -1.500, 0 };
	movingPos = { 0, -1.500, 0 };
	//finalPos = { 0, -0.500, 0 };

	Dialog.GetGameObject().GetTransform().SetPosition(initalPos);
	Dialog.GetGameObject().SetActive(false);

}
void SimpleAutoDialog::Update()
{
	if (activeDialog) {
		PrintDialog(Dialog);
        if (activeAudio) {
            Audio::Event(audioEvent.c_str());
            activeAudio = false;
        }        
	}
}

void SimpleAutoDialog::PrintDialog(API_UIImage& Dialog)
{
    Dialog.GetGameObject().SetActive(true);

    if (_timer >= timer) {
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
            _timer += Time::GetDeltaTime();
        }
    }

    Dialog.GetGameObject().GetTransform().SetPosition(movingPos);
}

void SimpleAutoDialog::OnCollisionEnter(API::API_RigidBody other)
{
    std::string detectionTag = other.GetGameObject().GetTag();
    if (detectionTag == "Player")
    {
        activeDialog = true;
    }
}
