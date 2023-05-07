#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class CamMov : HelloBehavior
{
public:
	float Lerp(float a, float b, float t);
	void Start() override;
	void Update() override;

	void SetOrbital(bool orbital);
	void Vibrate(float time);

	API_GameObject target;
	API_Vector3 realPos;

	API_Vector3 camPos = { 0.0f,25.0f,-20.0f };
	API_Vector3 camRot = { 50.0f, 0.0f, 0.0f };
	float orbitalMult = 2.0f;
	bool orbital = false;

	API_Vector3 desiredPosition;
	API_Vector3 smoothedPosition;

	float safeZoneDistance = 10.0f;
	float delay = 0.01f;
	float tempDelay = 0.0f;
	bool safeZone = true;

	float vibrationTime = 0.0f;
	float earthquakeMult = 0.2f;
};
