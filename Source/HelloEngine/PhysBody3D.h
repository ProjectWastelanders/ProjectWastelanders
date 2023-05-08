#pragma once

#include "MathGeoLib.h"
#include "btBulletDynamicsCommon.h"
#include "ModulePhysics.h"

struct PhysBody3D
{
public:
	PhysBody3D(btRigidBody* body);
	~PhysBody3D();

	void Push(float x, float y, float z);
	void SetVelocity(float x, float y, float z);
	void GetTransform(float* matrix) const;
	//float4x4 GetColTransform(float4x4 matrix);
	void SetTransform(const float* matrix) const;
	//void SetColTransform(float4x4 matrix);
	void SetPos(float x, float y, float z);
	void SetRotation(float x, float y, float z);
	void SetScale(float x, float y, float z);
	void SetMass();
	float3 GetPos();
	float3 GetRotation();
	float3 GetVelocity();
	void Update();
	void SetGravity(float3 grav);

	void SetShape(ColliderShape shape);
	void RenderCollider();
	void RenderRayCast();

public:
	btRigidBody* body = nullptr;
	float4x4 globalTrans;
	float4x4 transformMat;
	bool isRenderingCol;
	uint gameObjectUID = 0;
	float mass = 0;

	//Raycast
	float3 raycastPos1;
	float3 raycastPos2;
	bool isRenderingRayCast;

	bool isStatic;
	bool isKinematic;
	bool isTrigger;

	float3 colPos = { 0,0,0 };
	float3 colRot = { 0,0,0 };
	Quat colRotationOffset = Quat::identity;
	float3 colScl = { 1,1,1 };

	ColliderShape colShape;

	std::map<uint, PhysBody3D*> lastFrameCollidingBodies;
	std::map<uint, PhysBody3D*> thisFrameCollidingBodies;
};
