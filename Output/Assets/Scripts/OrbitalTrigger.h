#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class CamMov;

class OrbitalTrigger : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;
	void OnCollisionEnter(API::API_RigidBody other) override;
	void OnCollisionExit(API::API_RigidBody other) override;

	API_GameObject cam;
	CamMov* camScript;
};

