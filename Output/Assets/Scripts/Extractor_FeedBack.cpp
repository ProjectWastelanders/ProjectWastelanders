#include "Extractor_FeedBack.h"
//CREATED BY ALEX GARCIA
HELLO_ENGINE_API_C Extractor_FeedBack* CreateExtractor_FeedBack(ScriptToInspectorInterface* script)
{
	Extractor_FeedBack* classInstance = new Extractor_FeedBack();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragBoxUIImage("Extractor_Tutorial", &classInstance->Extractor_Img);
	return classInstance;
}

void Extractor_FeedBack::Start()
{
	initalPos = { -1.250, -0.700, 0 };
	movingPos = { -1.250, -0.700, 0 };
	Extractor_Img.GetGameObject().GetTransform().SetPosition(initalPos);
	Extractor_Img.GetGameObject().SetActive(false);
	finalPos = { -0.780, -0.700, 0 };
}
void Extractor_FeedBack::Update()
{
    if (activeTutorial == true && hideChest == false)
    {
        Extractor_Img.GetGameObject().SetActive(true);
        if (Extractor_Img.GetGameObject().GetTransform().GetLocalPosition().x < finalPos.x)
        {
            movingPos.x += 0.32 * Time::GetDeltaTime();
            timerTutorial = true;

        }
        if (timerTutorial == true)
        {
            showTutorial += 1.0f * Time::GetDeltaTime();

        }

    }

    if (showTutorial >= 8.0f)
    {

        if (Extractor_Img.GetGameObject().GetTransform().GetLocalPosition().x > initalPos.x)
        {
            movingPos.x -= 0.32 * Time::GetDeltaTime();
            timerTutorial = false;
            hideChest = true;
        }

        else if (Extractor_Img.GetGameObject().GetTransform().GetLocalPosition().x < initalPos.x) {
            endTutorial = true;
        }



    }
    if (endTutorial == true) 
    {
        Extractor_Img.GetGameObject().SetActive(false);
    }
    Extractor_Img.GetGameObject().GetTransform().SetPosition(movingPos);
}

void Extractor_FeedBack::OnCollisionEnter(API::API_RigidBody other)
{
    std::string detectionTag = other.GetGameObject().GetTag();
    if (detectionTag == "Player")
    {
        Console::Log("AAA");

        if (activeTutorial == false)
        {
            Audio::Event("info_alert");
        }
        activeTutorial = true;
    }
}

void Extractor_FeedBack::OnCollisionExit(API::API_RigidBody other)
{
    std::string detectionTag = other.GetGameObject().GetTag();
    if (detectionTag == "Player")
    {
    }
}