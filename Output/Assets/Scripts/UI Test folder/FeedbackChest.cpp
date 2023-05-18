#include "FeedbackChest.h"
HELLO_ENGINE_API_C FeedbackChest* CreateFeedbackChest(ScriptToInspectorInterface* script)
{
	FeedbackChest* classInstance = new FeedbackChest();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragBoxUIImage("Hold Button", &classInstance->HoldButton);
	script->AddDragBoxUIImage("Blue Bar", &classInstance->HoldBlue);
	script->AddDragBoxUIImage("Black Bar", &classInstance->HoldBlack);
	script->AddDragBoxUIImage("Hold Text", &classInstance->HoldText);
	return classInstance;
}

void FeedbackChest::Start()
{
	HoldButton.GetGameObject().SetActive(false);
	HoldBlue.GetGameObject().SetActive(false);
	HoldBlack.GetGameObject().SetActive(false);
	HoldText.GetGameObject().SetActive(false);
	openBarrierTime = maxOpenBarrierTime;
	picked = false;
}
void FeedbackChest::Update()
{
    if (isIn)
    {
        HoldBlue.FillImage(openBarrierTimeBar / maxOpenBarrierTime);
    }
}

void FeedbackChest::OnCollisionStay(API::API_RigidBody other)
{
    std::string detectionTag = other.GetGameObject().GetTag();
    if (detectionTag == "Player")
    {
        if (picked)
        {
            openBarrierTime -= Time::GetRealTimeDeltaTime();
            openBarrierTimeBar += Time::GetRealTimeDeltaTime();

            if (Input::GetGamePadButton(GamePadButton::BUTTON_X) == KeyState::KEY_UP || Input::GetKey(KeyCode::KEY_E) == KeyState::KEY_UP)
            {
                openBarrierTime = maxOpenBarrierTime;
                openBarrierTimeBar = 0;
                picked = false;
            }
            if (openBarrierTime <= 0.0f)
            {
                picked = false;
                HoldButton.GetGameObject().SetActive(false);
                HoldBlue.GetGameObject().SetActive(false);
                HoldBlack.GetGameObject().SetActive(false);
                HoldText.GetGameObject().SetActive(false);
                isOver = true;
            }
        }

        if (Input::GetGamePadButton(GamePadButton::BUTTON_X) == KeyState::KEY_DOWN || Input::GetKey(KeyCode::KEY_E) == KeyState::KEY_DOWN)
        {
            openBarrierTimeBar = 0;
            picked = true;
        }
    }
}

void FeedbackChest::OnCollisionEnter(API::API_RigidBody other)
{
    std::string detectionTag = other.GetGameObject().GetTag();
    if (detectionTag == "Player")
    {
        if (isOver == false)
        {
            isIn = true;
            HoldButton.GetGameObject().SetActive(true);
            HoldBlue.GetGameObject().SetActive(true);
            HoldBlack.GetGameObject().SetActive(true);
            HoldText.GetGameObject().SetActive(true);
        }
    }
}

void FeedbackChest::OnCollisionExit(API::API_RigidBody other)
{
    std::string detectionTag = other.GetGameObject().GetTag();
    if (detectionTag == "Player")
    {
        isIn = false;
        HoldButton.GetGameObject().SetActive(false);
        HoldBlue.GetGameObject().SetActive(false);
        HoldBlack.GetGameObject().SetActive(false);
        HoldText.GetGameObject().SetActive(false);

    }
}