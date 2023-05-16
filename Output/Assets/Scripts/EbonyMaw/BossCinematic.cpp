#include "BossCinematic.h"
HELLO_ENGINE_API_C BossCinematic* CreateBossCinematic(ScriptToInspectorInterface* script)
{
	BossCinematic* classInstance = new BossCinematic();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);

    script->AddDragBoxGameObject("Player", &classInstance->player);
    script->AddDragBoxGameObject("Boss", &classInstance->boss);
    script->AddDragBoxGameObject("Camera", &classInstance->camera);

    script->AddDragBoxUIImage("Dialog 1", &classInstance->Dialog_1);
    script->AddDragBoxUIImage("Dialog 2", &classInstance->Dialog_2);
    script->AddDragBoxUIImage("Dialog 3", &classInstance->Dialog_3);
    script->AddDragBoxUIImage("Dialog 4", &classInstance->Dialog_4);
    script->AddDragBoxUIImage("Dialog 5", &classInstance->Dialog_5);
    script->AddDragBoxUIImage("Dialog 5", &classInstance->Dialog_6);

	return classInstance;
}

void BossCinematic::Start()
{
    bLoop = (BossLoop*)boss.GetScript("BossLoop");
    camMov = (CamMov*)camera.GetScript("CamMov");
    playerMov = (PlayerMove*)player.GetScript("PlayerMove");

    activeCinematic = false;
    nextDialog = false;
    animBoss = false;

    initalPos = { 0, -1.500, 0 };
    movingPos = { 0, -1.500, 0 };
    finalPos = { 0, -0.500, 0 };

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
}
void BossCinematic::Update()
{
    if (bLoop != nullptr && camMov != nullptr && playerMov != nullptr)
    {
        if (!animBoss)
        {
            animBoss = true;
            bLoop->animationPlayer.ChangeAnimation(bLoop->idleAnim);
            bLoop->animationPlayer.SetLoop(true);
            bLoop->animationPlayer.Play();
        } 


        if (activeCinematic) {            

            switch (currentDialog)
            {
            case 1:
                camMov->target = boss;
                PrintDialog(Dialog_1);
                break;
            case 2:
                camMov->target = player;
                PrintDialog(Dialog_2);
                break;
            case 3:
                camMov->target = boss;
                PrintDialog(Dialog_3);
                break;
            case 4:
                camMov->target = player;
                PrintDialog(Dialog_4);
                break;
            case 5:
                camMov->target = boss;
                PrintDialog(Dialog_5);
                break;
            }
        }
    }
}

void BossCinematic::PrintDialog(API_UIImage &Dialog)
{
    Dialog.GetGameObject().SetActive(true);

    if (nextDialog) {
        if (Dialog.GetGameObject().GetTransform().GetGlobalPosition().y > initalPos.y)
        {
            movingPos.y -= 1 * Time::GetDeltaTime();
        }
        else {
            if (currentDialog == 5) {
                camMov->target = player;
                bLoop->battle = true;
                activeCinematic = false;
                playerMov->openingChest = false;
            }
            else {
                currentDialog += 1;
                nextDialog = false;
                Dialog.GetGameObject().SetActive(false);
            }
        }
    }
    else {
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

void BossCinematic::OnCollisionEnter(API::API_RigidBody other)
{
    std::string detectionTag = other.GetGameObject().GetTag();
    if (detectionTag == "Player")
    {
        activeCinematic = true;
        //playerMov->openingChest = true;
    }
}