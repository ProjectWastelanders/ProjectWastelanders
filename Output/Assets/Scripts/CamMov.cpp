#include "CamMov.h"
#include <random>

HELLO_ENGINE_API_C CamMov* CreateCamMov(ScriptToInspectorInterface* script)
{
	CamMov* classInstance = new CamMov();
	script->AddDragBoxGameObject("Target", &classInstance->target);
	script->AddDragFloat("Delay", &classInstance->delay);
	script->AddDragFloat("Offset_X", &classInstance->camPos.x);
	script->AddDragFloat("Offset_Y", &classInstance->camPos.y);
	script->AddDragFloat("Offset_Z", &classInstance->camPos.z);
	script->AddDragFloat("Orbital Multiplier", &classInstance->orbitalMult);
	script->AddDragFloat("Earthquake Multiplier", &classInstance->earthquakeMult);
	script->AddDragFloat("Rot_X", &classInstance->camRot.x);
	script->AddDragFloat("Rot_Y", &classInstance->camRot.y);
	script->AddDragFloat("Rot_Z", &classInstance->camRot.z);
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragFloat("SafeZone_Distance", &classInstance->safeZoneDistance);
	script->AddCheckBox("Use safe zone: ", &classInstance->safeZone);
	script->AddCheckBox("Test_Orbital", &classInstance->orbital);
	script->AddDragFloat("Test_VibrationTime", &classInstance->vibrationTime);
	return classInstance;
}

float CamMov::Lerp(float a, float b, float t)
{
	return a + t * (b - a);
}

void CamMov::Start()
{
	gameObject.GetTransform().SetPosition(target.GetTransform().GetGlobalPosition() + camPos);
	gameObject.GetTransform().SetRotation(camRot);
	tempDelay = delay;
	realPos = gameObject.GetTransform().GetGlobalPosition();
}

void CamMov::Update()
{
	gameObject.GetTransform().SetRotation(camRot);
	desiredPosition = target.GetTransform().GetGlobalPosition();
	
	//Zoom & Offset
	if (vibrationTime > 0.0f)
	{
		desiredPosition += camPos * earthquakeMult;
		vibrationTime -= Time::GetDeltaTime();
	}
	else if (orbital)
	{
		desiredPosition += camPos * orbitalMult;
	}
	else
	{
		desiredPosition += camPos;
	}
	
	//Safe Zone
	if (safeZone)
	{
		if (desiredPosition.Distance(realPos) > safeZoneDistance) {
			tempDelay += 0.01;
		}
		else {
			if (tempDelay > delay) {
				tempDelay -= 0.01;
			}
		}
	}

	smoothedPosition.x = Lerp(realPos.x, desiredPosition.x, tempDelay);
	smoothedPosition.y = Lerp(realPos.y, desiredPosition.y, tempDelay);
	smoothedPosition.z = Lerp(realPos.z, desiredPosition.z, tempDelay);
	realPos = smoothedPosition;

	//Vibration
	if (vibrationTime > 0.0f)
	{
		smoothedPosition.x += (float(rand() % 20)/10.0f - 1.0f);
		smoothedPosition.z += (float(rand() % 20)/10.0f - 1.0f);
	}

	gameObject.GetTransform().SetPosition(smoothedPosition.x, smoothedPosition.y, smoothedPosition.z);
}

void CamMov::SetOrbital(bool orbital)
{
	this->orbital = orbital;
}

void CamMov::Vibrate(float time)
{
	vibrationTime = time;
}