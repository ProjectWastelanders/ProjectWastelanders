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

	script->AddDragFloat("Rot_X", &classInstance->camRot.x);
	script->AddDragFloat("Rot_Y", &classInstance->camRot.y);
	script->AddDragFloat("Rot_Z", &classInstance->camRot.z);

	script->AddDragFloat("Orbital Multiplier", &classInstance->orbitalMult);
	script->AddDragFloat("Earthquake Multiplier", &classInstance->earthquakeMult);
	script->AddDragFloat("Earthquake Force", &classInstance->earthquakeForce);
	script->AddCheckBox("TEST EARTHQUAKE", &classInstance->testEarthquake); //ERASE THIS

	script->AddDragFloat("SafeZone_Distance", &classInstance->safeZoneDistance);
	script->AddCheckBox("Use safe zone", &classInstance->safeZone);
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
	if (testEarthquake) Earthquake(0.5f);	//ERASE THIS

	gameObject.GetTransform().SetRotation(camRot);
	desiredPosition = target.GetTransform().GetGlobalPosition();
	
	//Zoom & Offset
	if (zoomTime > 0.0f)
	{
		desiredPosition += camPos * earthquakeMult;
		zoomTime -= Time::GetDeltaTime();
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
			tempDelay += 0.01f;
		}
		else {
			if (tempDelay > delay) {
				tempDelay -= 0.01f;
			}
		}
	}

	smoothedPosition.x = Lerp(realPos.x, desiredPosition.x, tempDelay);
	smoothedPosition.y = Lerp(realPos.y, desiredPosition.y, tempDelay);
	smoothedPosition.z = Lerp(realPos.z, desiredPosition.z, tempDelay);
	realPos = smoothedPosition;

	//Vibration
	if (shakingTime > 0.0f)
	{
		float divisor = 1.0f / currentForce;
		smoothedPosition.x += (float(rand() % 20)/(divisor * 10.0f) - currentForce);
		smoothedPosition.z += (float(rand() % 20)/(divisor * 10.0f) - currentForce);
		shakingTime -= Time::GetDeltaTime();
	}

	gameObject.GetTransform().SetPosition(smoothedPosition.x, smoothedPosition.y, smoothedPosition.z);
}

void CamMov::SetOrbital(bool orbital)
{
	this->orbital = orbital;
}

void CamMov::Shake(float time, float force)
{
	shakingTime = time;
	currentForce = force;
}

void CamMov::Zoom(float time)
{
	zoomTime = time;
}

void CamMov::Earthquake(float time)
{
	Zoom(time);
	Shake(time, earthquakeForce);
}