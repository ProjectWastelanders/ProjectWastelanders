#include "ThanosCinematic.h"
#include "../../Thanos/ThanosMovement.h"
#include "../../Player/PlayerMove.h"
#include "../../CamMov.h"

HELLO_ENGINE_API_C ThanosCinematic* CreateThanosCinematic(ScriptToInspectorInterface* script)
{
	ThanosCinematic* classInstance = new ThanosCinematic();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragBoxGameObject("Player", &classInstance->player);
    script->AddDragBoxGameObject("Boss", &classInstance->boss);
    script->AddDragBoxGameObject("Camera", &classInstance->camera);

    script->AddDragBoxUIImage("Dialog 1", &classInstance->Dialog_1);
    script->AddDragBoxUIImage("Dialog 2", &classInstance->Dialog_2);
    script->AddDragBoxUIImage("Dialog 3", &classInstance->Dialog_3);
    script->AddDragBoxUIImage("Dialog 4", &classInstance->Dialog_4);
    script->AddDragBoxUIImage("Dialog 5", &classInstance->Dialog_5);
    script->AddDragBoxUIImage("Dialog 6", &classInstance->Dialog_6);
    script->AddDragBoxUIImage("Dialog 7", &classInstance->Dialog_7);
    script->AddDragBoxUIImage("Dialog 8", &classInstance->Dialog_8);
    script->AddDragBoxUIImage("Dialog 9", &classInstance->Dialog_9);

    script->AddDragFloat("Dialog Pos X", &classInstance->finalPos.x);
    script->AddDragFloat("Dialog Pos Y", &classInstance->finalPos.y);
    script->AddDragFloat("Dialog Pos Z", &classInstance->finalPos.z);
	return classInstance;
}

void ThanosCinematic::Start()
{
    tMovement = (ThanosMovement*)boss.GetScript("ThanosMovement");
    if (tMovement == nullptr) Console::Log("ThanosMovement missing in ThanosCinematic Script.");

    camMov = (CamMov*)camera.GetScript("CamMov");
    if (camMov == nullptr) Console::Log("CamMov missing in ThanosCinematic Script.");

    playerMov = (PlayerMove*)player.GetScript("PlayerMove");
    if (playerMov == nullptr) Console::Log("PlayerMove missing in ThanosCinematic Script.");

    activeCinematic = false;
    showedDialog = false;
    nextDialog = false;
    //ambienceMusic_L_4 = Audio::Event("L4previous_ambience");
    initalPos = { 0, -1.500, 0 };
    movingPos = { 0, -1.500, 0 };
    //finalPos = { 0, -0.500, 0 };

    currentDialog = 0;

    activeAudio = true;

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
}
void ThanosCinematic::Update()
{
    
    if (activeCinematic && !showedDialog) {
        playerMov->openingChest = true;
        player.GetRigidBody().SetVelocity((0, 0, 0));
        playerMov->PlayIdleAnim();
        switch (currentDialog)
        {
        case 0:
            camMov->target = player;
            PrintDialog(Dialog_1, "starlord_confident");
            break;
        case 1:
            camMov->target = boss;
            PrintDialog(Dialog_2, "thanos_doubt");
            break;
        case 2:
            camMov->target = player;
            PrintDialog(Dialog_3, "starlord_confident");
            break;
        case 3:
            camMov->target = boss;
            PrintDialog(Dialog_4, "thanos_cool");
            break;
        case 4:
            camMov->target = boss;
            PrintDialog(Dialog_5, "thanos_calm");
            break;
        case 5:
            camMov->target = player;
            PrintDialog(Dialog_6, "starlord_sorry");
            break;
        case 6:
            camMov->target = boss;
            PrintDialog(Dialog_7, "thanos_facing");
            break;
        case 7:
            camMov->target = player;
            PrintDialog(Dialog_8, "starlord_confident");
            break;
        case 8:
            camMov->target = boss;
            PrintDialog(Dialog_9, "thanos_threatening");
            break;
        }
    }
}

void ThanosCinematic::PrintDialog(API_UIImage& Dialog, string audioEvent)
{
    Dialog.GetGameObject().SetActive(true);

    if (nextDialog) {
        if (Dialog.GetGameObject().GetTransform().GetGlobalPosition().y > initalPos.y)
        {
            movingPos.y -= 1 * Time::GetDeltaTime();
        }
        else {
            if (currentDialog == 8) {
                Dialog.GetGameObject().SetActive(false);
                camMov->target = player;
                tMovement->cinematic = false;
                activeCinematic = false;
                showedDialog = true;
                playerMov->openingChest = false;

            }
            else {
                currentDialog += 1;
                nextDialog = false;
                activeAudio = true;
                Dialog.GetGameObject().SetActive(false);
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
        if (Input::GetGamePadButton(GamePadButton::BUTTON_A) == KeyState::KEY_DOWN || Input::GetKey(KeyCode::KEY_RETURN) == KeyState::KEY_DOWN)
        {
            nextDialog = true;
        }
    }
    Dialog.GetGameObject().GetTransform().SetPosition(movingPos);
}

void ThanosCinematic::OnCollisionEnter(API::API_RigidBody other)
{
    std::string detectionTag = other.GetGameObject().GetTag();
    if (detectionTag == "Player")
    {
        activeCinematic = true;
        //Audio::StopEvent(ambienceMusic_L_4);
        //Audio::Event("L4_bossfight");

    }
    //other.GetGameObject().SetActive(false);
}