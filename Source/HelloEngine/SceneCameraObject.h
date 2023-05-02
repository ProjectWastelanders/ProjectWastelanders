#pragma once
#include "CameraObject.h"

class LayerEditor;
class GameObject;

class SceneCameraObject : public CameraObject
{
public:
	SceneCameraObject();
	~SceneCameraObject();

	void UpdateInput();

	float cameraSpeed = 100.0f;

	bool SetWheelSpeed(float speed);

private:
	/// <summary>
	/// Focus on a point in space with an offset set by distance
	/// </summary>
	/// <param name="focusPoint">Point to focus on</param>
	/// <param name="distance">Distance between the camera and the focus point</param>
	void Focus(const float3& focusPoint, float distance);

	/// <summary>
	/// Focus on a gameObject. If it has a mesh, will take into account the mesh's AABB
	/// </summary>
	void Focus(GameObject* gameObject);

private:

	float _wheelSpeed = 10.5f;
};

