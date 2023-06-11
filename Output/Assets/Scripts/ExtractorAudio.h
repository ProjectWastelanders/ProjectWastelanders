#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class SmokeExtractorTrap;

class ExtractorAudio : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;
	void OnCollisionEnter(API::API_RigidBody other) override;
	void OnCollisionExit(API::API_RigidBody other) override;

	API_GameObject extractorGO;
	SmokeExtractorTrap* smokeExtractor;

	bool activeAudio, playSmoke, playFire;
};

