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
	void Shake(float time = 0.1f, float force = 0.5f);
	void Zoom(float time);
	void Earthquake(float time);

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

	float shakingTime = 0.0f;
	float zoomTime = 0.0f;
	float earthquakeMult = 0.2f;
	float earthquakeForce = 1.0f;
	float currentForce = 1.0f;

	bool testEarthquake = false;  //ERASE THIS
};
