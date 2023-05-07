#pragma once
#include "Module.h"

#include "Ak/samples/AkFilePackageLowLevelIOBlocking.h"// Sample low-level I/O implementation

class ModuleAudio : public Module
{
public:

	ModuleAudio(bool start_enabled = true);
	~ModuleAudio();

	bool Init() override;

	bool Start() override;

	UpdateStatus PreUpdate() override;

	UpdateStatus Update() override;

	UpdateStatus PostUpdate() override;

	static void S_ProduceEvent(const char* eventName, AkGameObjectID gameObjectID = 0);
	static void S_ProduceEvent(AkUniqueID eventID, AkGameObjectID gameObjectID = 0);

	bool CleanUp() override;

	static AkGameObjectID RegisterGameObject(AkGameObjectID id);
	void SetDefaultListener(const AkGameObjectID id);

	static void StopAllAudioEvents();


private:

	bool InitSoundEngine();
	void TerminateSoundEngine();

	void ProcessAudio();

	void InitSoundBanks();
	void TerminateSoundBanks();

	//// We're using the default Low-Level I/O implementation that's part
	//// of the SDK's sample code, with the file package extension
	CAkFilePackageLowLevelIOBlocking g_lowLevelIO;

	AkBankID _initBank;
	AkBankID _sfxBank;
	AkBankID _musicBank;

	static AkGameObjectID defaultListener;
	static AkGameObjectID defaultSource;

	// Temporal
	bool isPlayingBackground = false;

};

