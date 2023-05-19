#include "Headers.h"
#include "ModuleAudio.h"

#include "AK/SoundEngine/Common/AkMemoryMgr.h" // Memory Manager interface
#include "AK/SoundEngine/Common/AkModule.h" // Default memory manager
#include "AK/SoundEngine/Common/IAkStreamMgr.h" // Streaming Manager
#include "AK/Tools/Common/AkPlatformFuncs.h" // Thread defines
#include "AK/SoundEngine/Common/AkSoundEngine.h" // Sound Engine
#include "AK/MusicEngine/Common/AkMusicEngine.h" // Music Engine
#include "AK/SpatialAudio/Common/AkSpatialAudio.h" // Spatial Audio
//// Include for communication between Wwise and the game -- Not needed in the release version
//#ifndef AK_OPTIMIZED
//#include "AK/Comm/AkCommunication.h"
//#endif // AK_OPTIMIZED

#include "LayerGame.h"
#include "Wwise_IDs.h"

#include "GameObject.h"
#include "ModuleLayers.h"
#include "CameraComponent.h"

AkGameObjectID ModuleAudio::defaultListener;
AkGameObjectID ModuleAudio::defaultSource;
GameObject* ModuleAudio::defaultListenerGameObject = nullptr;

ModuleAudio::ModuleAudio(bool start_enabled)
{
}

ModuleAudio::~ModuleAudio()
{
    TerminateSoundBanks();
    TerminateSoundEngine();
}

bool ModuleAudio::Init()
{
    InitSoundEngine();
    InitSoundBanks();

    defaultListener = RegisterGameObject(HelloUUID::GenerateUUID());
    SetDefaultListener(defaultListener);

    defaultSource = RegisterGameObject(HelloUUID::GenerateUUID());

    return true;
}

bool ModuleAudio::Start()
{
    return true;
}

UpdateStatus ModuleAudio::PreUpdate()
{
    if (defaultListenerGameObject != nullptr)
    {
        float3 pos = defaultListenerGameObject->transform->GetGlobalMatrix().TranslatePart();

        AkSoundPosition position;
        AkVector64 akPos;
        akPos.X = pos.x;
        akPos.Y = pos.y;
        akPos.Z = pos.z;
        position.SetPosition(akPos);

        AK::SoundEngine::SetPosition(defaultListener, position);
    }

    ProcessAudio();
    return UpdateStatus::UPDATE_CONTINUE;
}

UpdateStatus ModuleAudio::Update()
{
    return UpdateStatus::UPDATE_CONTINUE;
}

UpdateStatus ModuleAudio::PostUpdate()
{
    return UpdateStatus::UPDATE_CONTINUE;
}

uint ModuleAudio::S_ProduceEvent(const char* eventName, AkGameObjectID gameObjectID)
{
    AkPlayingID playing = AK::SoundEngine::PostEvent(eventName, defaultSource);
    if (playing == 0) 
    {
        std::string strName = eventName;
        Console::S_Log("Error at reproducing audio event: " + strName);
    }
    return playing;
}

uint ModuleAudio::S_ProduceEvent(AkUniqueID eventID, AkGameObjectID gameObjectID)
{
    AkPlayingID playing = AK::SoundEngine::PostEvent(eventID, defaultSource);
    if (playing == 0)
        Console::S_Log("Error at reproducing audio event with ID: " + std::to_string(eventID));
    return playing;
}

void ModuleAudio::S_StopEvent(uint playingID)
{
    AK::SoundEngine::StopPlayingID(playingID);
}

void ModuleAudio::SetGameParameter(const char* paramName, float value)
{
    AK::SoundEngine::SetRTPCValue(paramName, value);
}

bool ModuleAudio::CleanUp()
{
    return true;
}

AkGameObjectID ModuleAudio::RegisterGameObject(AkGameObjectID id)
{
    AK::SoundEngine::RegisterGameObj(id);
    return id;
}

void ModuleAudio::SetDefaultListener(const AkGameObjectID id)
{
    AK::SpatialAudio::RegisterListener(id);
    AK::SoundEngine::SetDefaultListeners(&id, 1);

}

void ModuleAudio::StopAllAudioEvents()
{
    AK::SoundEngine::StopAll();
}

void ModuleAudio::SetDefaultListener()
{
    for (auto& gameObject : ModuleLayers::gameObjects)
    {
        CameraComponent* camera = gameObject.second->GetComponent<CameraComponent>(); 
        if (camera != nullptr) // we asume that there is only 1 camera component per level.
        {
            defaultListenerGameObject = gameObject.second;
            break;
        }
    }
}

bool ModuleAudio::InitSoundEngine()
{
    AkMemSettings memSettings;
    AK::MemoryMgr::GetDefaultSettings(memSettings);

    if (AK::MemoryMgr::Init(&memSettings) != AK_Success)
    {
        Console::S_Log("Could not create the memory manager.");
        return false;
    }

    AkStreamMgrSettings stmSettings;
    AK::StreamMgr::GetDefaultSettings(stmSettings);

    // Customize the Stream Manager settings here.
    if (!AK::StreamMgr::Create(stmSettings))
    {
        Console::S_Log("Could not create the Streaming Manager");
        return false;
    }
    
    AkDeviceSettings deviceSettings;
    AK::StreamMgr::GetDefaultDeviceSettings(deviceSettings);

    // Customize the streaming device settings here.
    // CAkFilePackageLowLevelIOBlocking::Init() creates a streaming device
    // in the Stream Manager, and registers itself as the File Location Resolver.

    if (g_lowLevelIO.Init(deviceSettings) != AK_Success)
    {
        Console::S_Log("Could not create the streaming device and Low-Level I/O system");
        return false;
    }

    // Create the Sound Engine
    // Using default initialization parameters

    AkInitSettings initSettings;
    AkPlatformInitSettings platformInitSettings;
    AK::SoundEngine::GetDefaultInitSettings(initSettings);
    AK::SoundEngine::GetDefaultPlatformInitSettings(platformInitSettings);

    if (AK::SoundEngine::Init(&initSettings, &platformInitSettings) != AK_Success)
    {
        Console::S_Log("Could not initialize the Sound Engine.");
        return false;
    }

    // Initialize the music engine
    // Using default initialization parameters

    AkMusicSettings musicInit;
    AK::MusicEngine::GetDefaultInitSettings(musicInit);

    if (AK::MusicEngine::Init(&musicInit) != AK_Success)
    {
        Console::S_Log("Could not initialize the Music Engine.");
        return false;
    }

    //Initialize Spatial Audio
    //Using default initialization parameters

    AkSpatialAudioInitSettings settings; // The constructor fills AkSpatialAudioInitSettings with the recommended default settings. 

    if (AK::SpatialAudio::Init(settings) != AK_Success)
    {
        Console::S_Log("Could not initialize the Spatial Audio.");
        return false;
    }

//#ifndef AK_OPTIMIZED
//
//    // Initialize communications (not in release build!)
//    AkCommSettings commSettings;
//    AK::Comm::GetDefaultInitSettings(commSettings);
//    if (AK::Comm::Init(commSettings) != AK_Success)
//    {
//        Console::S_Log("Could not initialize communication.");
//        return false;
//    }
//#endif // AK_OPTIMIZEDs

    return true;
}

void ModuleAudio::TerminateSoundEngine()
{
#ifndef AK_OPTIMIZED
    // Terminate Communication Services
    /*AK::Comm::Term();*/
#endif // AK_OPTIMIZED

    AK::MusicEngine::Term();
    AK::SoundEngine::Term();

    g_lowLevelIO.Term();

    if (AK::IAkStreamMgr::Get())
        AK::IAkStreamMgr::Get()->Destroy();

    AK::MemoryMgr::Term();
}

void ModuleAudio::ProcessAudio()
{
    /*We call AK::SoundEngine::RenderAudio() so the sound engine processes all pending bank load/unload requests, events, position changes, states, switches, RTPCs, etc. 
    If communications are enabled, they are also processed within RenderAudio().*/
    AKRESULT eResult = AK::SoundEngine::RenderAudio();
    if (eResult != AK_Success)
    {
        Console::S_Log("Error in ProcessAudio()");
    }
}

void ModuleAudio::InitSoundBanks()
{
    g_lowLevelIO.SetBasePath(AKTEXT("Assets/Wwise/Banks"));

    AKRESULT eResult = AK::SoundEngine::LoadBank(L"Init.bnk", _initBank);

    if (eResult == AK_Success)
    {
        eResult = AK::SoundEngine::LoadBank(L"General_Soundbank.bnk", _sfxBank);
    }
    if (eResult == AK_Success)
    {
        eResult = AK::SoundEngine::LoadBank(L"Music_Soundbank.bnk", _musicBank);
    }
    if (eResult == AK_Success)
    {
        eResult = AK::SoundEngine::LoadBank(L"Music_Soundbank2.bnk", _musicBank);
    }
}

void ModuleAudio::TerminateSoundBanks()
{
    AK::SoundEngine::UnloadBank(L"General_Soundbank.bnk", NULL);
    AK::SoundEngine::UnloadBank(L"Music_Soundbank.bnk", NULL);
    AK::SoundEngine::UnloadBank(L"Music_Soundbank2.bnk", NULL);
    AK::SoundEngine::UnloadBank(L"Init.bnk", NULL);
}
