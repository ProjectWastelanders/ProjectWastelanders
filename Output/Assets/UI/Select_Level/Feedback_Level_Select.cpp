#include "Feedback_Level_Select.h"
HELLO_ENGINE_API_C Feedback_Level_Select* CreateFeedback_Level_Select(ScriptToInspectorInterface* script)
{
	Feedback_Level_Select* classInstance = new Feedback_Level_Select();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragFloat("Hold Button Time", &classInstance->maxOpenSelectTime);
    script->AddDragBoxUIImage("Hold Bar", &classInstance->chargingBar);
    script->AddDragBoxUIImage("Selected Feedback", &classInstance->isSelected);

    script->AddDragBoxUIInput("Hold Conjunt", &classInstance->chargingConjunt);

    script->AddDragBoxUICheckBox("Level 1", &classInstance->level1);
    script->AddDragBoxUICheckBox("Level 2", &classInstance->level2);
    script->AddDragBoxUICheckBox("Level 3", &classInstance->level3);
    script->AddDragBoxUICheckBox("Level 4", &classInstance->level4);

	return classInstance;
}

void Feedback_Level_Select::Start()
{
    indexLevles = 3;
    selecting = false;
}
void Feedback_Level_Select::Update()
{
    //TODO ROGER: al completar la barra que faci coses
    if (Input::GetGamePadButton(GamePadButton::BUTTON_DOWN) == KeyState::KEY_DOWN && indexLevles >= 1)
    {
        openSelecTime = 0;
        indexLevles--;
        SetCurrentLevel();
        chargingConjunt.GetGameObject().GetTransform().SetPosition(chargingConjunt.GetGameObject().GetTransform().GetGlobalPosition().x, chargingConjunt.GetGameObject().GetTransform().GetGlobalPosition().y - 0.385, chargingConjunt.GetGameObject().GetTransform().GetGlobalPosition().z);
        isSelected.GetGameObject().GetTransform().SetPosition(isSelected.GetGameObject().GetTransform().GetGlobalPosition().x, isSelected.GetGameObject().GetTransform().GetGlobalPosition().y - 0.385, isSelected.GetGameObject().GetTransform().GetGlobalPosition().z);

        if (currentLevel.getIsActive())
        {
            chargingConjunt.GetGameObject().SetActive(false);
            isSelected.GetGameObject().SetActive(true);
        }
        else
        {
            chargingConjunt.GetGameObject().SetActive(true);
            isSelected.GetGameObject().SetActive(false);
        }
    }
    else if (Input::GetGamePadButton(GamePadButton::BUTTON_UP) == KeyState::KEY_DOWN && indexLevles <= 2)
    {
        openSelecTime = 0;
        indexLevles++;
        SetCurrentLevel();
        chargingConjunt.GetGameObject().GetTransform().SetPosition(chargingConjunt.GetGameObject().GetTransform().GetGlobalPosition().x, chargingConjunt.GetGameObject().GetTransform().GetGlobalPosition().y + 0.385, chargingConjunt.GetGameObject().GetTransform().GetGlobalPosition().z);
        isSelected.GetGameObject().GetTransform().SetPosition(isSelected.GetGameObject().GetTransform().GetGlobalPosition().x, isSelected.GetGameObject().GetTransform().GetGlobalPosition().y + 0.385, isSelected.GetGameObject().GetTransform().GetGlobalPosition().z);
        
        if (currentLevel.getIsActive())
        {
            chargingConjunt.GetGameObject().SetActive(false);
            isSelected.GetGameObject().SetActive(true);
        }
        else
        {
            chargingConjunt.GetGameObject().SetActive(true);
            isSelected.GetGameObject().SetActive(false);
        }
    }

    OnCharge();
}

void Feedback_Level_Select::OnCollisionEnter(API::API_RigidBody other)
{
    std::string detectionTag = other.GetGameObject().GetTag();
    if (detectionTag == "Player")
    {
        chargingBar.GetGameObject().SetActive(true);
    }
}

void Feedback_Level_Select::OnCollisionExit(API::API_RigidBody other)
{
    std::string detectionTag = other.GetGameObject().GetTag();
    if (detectionTag == "Player")
    {
        chargingBar.GetGameObject().SetActive(false);
    }
}

void Feedback_Level_Select::OnCharge()
{
    chargingBar.FillImage(openSelecTime / maxOpenSelectTime);

    if (selecting)
    {
        openSelecTime += Time::GetRealTimeDeltaTime();

        if (Input::GetGamePadButton(GamePadButton::BUTTON_X) == KeyState::KEY_UP || Input::GetKey(KeyCode::KEY_E) == KeyState::KEY_UP)
        {
            openSelecTime = 0;
            selecting = false;
        }
    }
    if (Input::GetGamePadButton(GamePadButton::BUTTON_X) == KeyState::KEY_DOWN || Input::GetKey(KeyCode::KEY_E) == KeyState::KEY_DOWN)
    {
        selecting = true;
    }
}

void Feedback_Level_Select::SetCurrentLevel()
{
    switch (indexLevles)
    {
    case 3:
        currentLevel = level1;
        break;
    case 2:
        currentLevel = level2;
        break;
    case 1:
        currentLevel = level3;
        break;
    case 0:
        currentLevel = level4;
        break;
    default:
        break;
    }
}