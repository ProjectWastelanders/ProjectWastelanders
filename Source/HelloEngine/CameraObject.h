#pragma once

#include "FrameBuffer.h"
#include "Shader.h"

enum class CameraType
{
	SCENE,
	GAME,
};

class ResourceShader;

class CameraObject
{
public:
	CameraObject();
	virtual ~CameraObject();

	void Look(const float3& Position, const float3& Reference, bool RotateAroundReference = false);
	void LookAt(const float3& Spot);
	void Move(const float3& Movement);
	
	void DrawFrustum();
	void SetUpBuffers();

	float* GetViewMatrix();
	float* GetProjectionMatrix();

	float* GetViewMatrixNoTransp();
	float* GetProjectionMatrixNoTransp();

	float3 GetFrustumPosition();
	float3 GetPosition();

	void ChangeToOrthograpic();
	void ChangeToPerspective();

	void ChangeOrthographicSize(float size);
	float GetOrthographicSize();

	void RecalculateProjection();

	void RegenerateFrameBuffer(int width, int height);

	void ChangeAspectRatio(float aspectRatio);

	float GetAspectRatio() { return aspectRatio; }

	void SetFOV(float fov);
	float GetFOV() { return FOV; };

	bool IsInsideFrustum(AABB& globalAABB);

private:
	bool IsInsideFrustumPerspective(AABB& globalAABB);
	bool IsInsideFrustumOrthographic(AABB& globalAABB);

public:
	float3 X = { 0,0,0 }, Y = { 0,0,0 }, Z = { 0,0,0 }, Position = { 0,0,0 }, Reference = { 0,0,0 };
	bool active = true;
	FrameBuffer frameBuffer;
	CameraType type = CameraType::GAME;
	Frustum cameraFrustum; // Should not be public, but we need ModuleCamera3D to access it.
	bool currentlyDisplaying = false;
	bool isCullingActive = true;


private:
	float4x4 ViewMatrix = float4x4::identity, ViewMatrixInverse = float4x4::identity, ProjectionMatrix = float4x4::identity;

	float3 offset = { 0,0,0 };
	float aspectRatio = 0.0f;
	float FOV = 60.0f;
	float _orthographicSize = 10.0f; // We use a Size instead of Width and height to maintain correct Aspect ratio at all times.

	std::vector<uint> boxIndices;

	ResourceShader* localLineShader = nullptr;
	uint VAO = 0;
	uint VBO = 0;
	uint IBO = 0;

	friend class ModuleCamera3D;
	friend class CameraComponent;

protected:
	Application* app = nullptr;


};

