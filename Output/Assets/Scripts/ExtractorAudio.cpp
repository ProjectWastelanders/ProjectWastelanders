#include "ExtractorAudio.h"
#include "InteractiveEnviroment/SmokeExtractorTrap.h"

HELLO_ENGINE_API_C ExtractorAudio* CreateExtractorAudio(ScriptToInspectorInterface* script)
{
	ExtractorAudio* classInstance = new ExtractorAudio();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);

	script->AddDragBoxGameObject("Extractor", &classInstance->extractorGO);

	return classInstance;
}

void ExtractorAudio::Start()
{
	smokeExtractor = (SmokeExtractorTrap*)extractorGO.GetScript("SmokeExtractorTrap");
	if (smokeExtractor == nullptr) Console::Log("smokeExtractor missing in ExtractorAudio Script.");

    activeAudio = false;
	playSmoke = true;
	playFire = true;
}
void ExtractorAudio::Update()
{
	if (smokeExtractor)
	{
		if (activeAudio)
		{
			if (smokeExtractor->throwFire && playFire)
			{
				Audio::Event("trap_fire");
				playFire = false;
				playSmoke = true;
			}
			if (!smokeExtractor->throwFire && playSmoke)
			{
				Audio::Event("trap_smoke");
				playSmoke = false;
				playFire = true;
			}
		}
	}	
}

void ExtractorAudio::OnCollisionEnter(API::API_RigidBody other)
{
    std::string detectionTag = other.GetGameObject().GetTag();
    if (detectionTag == "Player") activeAudio = true;
}

void ExtractorAudio::OnCollisionExit(API::API_RigidBody other)
{
	std::string detectionTag = other.GetGameObject().GetTag();
	if (detectionTag == "Player")
	{
		activeAudio = false;
		playSmoke = true;
		playFire = true;
	}
}