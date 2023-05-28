#include "TpRotateSphere.h"
HELLO_ENGINE_API_C TpRotateSphere* CreateTpRotateSphere(ScriptToInspectorInterface* script)
{
	TpRotateSphere* classInstance = new TpRotateSphere();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragFloat("Multiplier", &classInstance->timeMult);
	script->AddDragFloat("Deformation Seconds", &classInstance->secondsToDeform);
	script->AddDragFloat("Deformation Scale", &classInstance->deformationScale);
	script->AddDragFloat("Sphere Size", &classInstance->sphereSize);
	
	return classInstance;
}

void TpRotateSphere::Start()
{
	timeCount = 0.0f;
	deformationTime = 0.0f;
	deformationTimeMid = 0.0f;
	deformedVer = false;
	inInFirstPart = true;
	partsCount = 0;
}
void TpRotateSphere::Update()
{
	timeCount += Time::GetDeltaTime();
	deformationTime += Time::GetDeltaTime();
	deformationTimeMid += Time::GetDeltaTime();

	float rotY = gameObject.GetTransform().GetLocalRotation().y;
	bool hasChanged = false;
	while (rotY / 360 > 1.0f)
	{
		rotY = rotY - 360;
		hasChanged = true;
	}
	if (hasChanged)
	{
		gameObject.GetTransform().SetRotation(0, rotY, 0);
	}

	float rot = timeCount * timeMult;
	gameObject.GetTransform().Rotate(0, rot, 0);

	rotY = gameObject.GetTransform().GetLocalRotation().y;
	hasChanged = false;
	while (rotY / 360 > 1.0f)
	{
		rotY = rotY - 360;
		hasChanged = true;
	}
	if (hasChanged)
	{
		gameObject.GetTransform().SetRotation(0, rotY, 0);
	}

	if (secondsToDeform == 0)
	{
		return;
	}

	float secondsToDeformMid = secondsToDeform / 2;
	while (deformationTime > secondsToDeformMid)
	{
		deformationTime -= secondsToDeformMid;
		inInFirstPart = !inInFirstPart;
		partsCount++;
	}
	
	while (partsCount >= 2)
	{
		partsCount -= 2;
		deformedVer = !deformedVer;
	}

	float deformRatioMid = deformationTime / secondsToDeformMid;

	if (deformedVer == true)
	{
		if (inInFirstPart == true)
		{
			gameObject.GetTransform().SetScale(sphereSize - deformRatioMid * deformationScale * sphereSize, sphereSize + deformRatioMid * deformationScale * sphereSize, sphereSize - deformRatioMid * deformationScale * sphereSize);
		}
		else
		{
			gameObject.GetTransform().SetScale((sphereSize - deformationScale * sphereSize) + deformRatioMid * deformationScale * sphereSize, (sphereSize + deformationScale * sphereSize) - deformRatioMid * deformationScale * sphereSize, (sphereSize - deformationScale * sphereSize) + deformRatioMid * deformationScale * sphereSize);
		}
	}
	else
	{
		if (inInFirstPart == true)
		{
			gameObject.GetTransform().SetScale(sphereSize + deformRatioMid * deformationScale * sphereSize, sphereSize - deformRatioMid * deformationScale * sphereSize, sphereSize + deformRatioMid * deformationScale * sphereSize);
		}
		else
		{
			gameObject.GetTransform().SetScale((sphereSize + deformationScale * sphereSize) - deformRatioMid * deformationScale * sphereSize, (sphereSize - deformationScale * sphereSize) + deformRatioMid * deformationScale * sphereSize, (sphereSize + deformationScale * sphereSize) - deformRatioMid * deformationScale * sphereSize);
		}
	}
}