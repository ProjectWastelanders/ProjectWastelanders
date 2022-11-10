#pragma once
#include "Component.h"
#include "CameraObject.h"

class CameraComponent : public Component
{
public:
	CameraComponent(GameObject* gameObject);
	~CameraComponent();

	void OnPositionUpdate(float3 pos) override;
	void OnRotationUpdate(float3 rotation) override;
	void OnTransformUpdate(float3 pos, float3 scale, float3 rotation) override;

	void OnEditor() override;

	void Enable(bool enabled, bool fromGo = true) override;

public:
	CameraObject* cameraObject = nullptr;

private:
	ModuleCamera3D* moduleCameras = nullptr;


};

