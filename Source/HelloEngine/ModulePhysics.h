#pragma once
#include "Module.h"
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

class Primitive;
class PhysBody3D;

#define WORLD_TO_RENDER_RELATION 0.5f;

enum class ColliderShape
{
	BOX,
	SPHERE,
	CYLINDER,
	Count,
	NONE,

};

class RayCast
{
public:
	RayCast(btCollisionWorld::AllHitsRayResultCallback& raycast) : btRaycast(raycast) {}
	~RayCast() {};
	btCollisionWorld::AllHitsRayResultCallback btRaycast;
};

class ModulePhysics : public Module
{
public:

	ModulePhysics();

	virtual ~ModulePhysics();

	bool Init() override;

	bool Start() override;

	UpdateStatus PreUpdate() override;

	UpdateStatus Update() override;

	UpdateStatus PostUpdate() override;

	bool CleanUp() override;

	PhysBody3D* CreatePhysBody(const Primitive* primitive, float mass = 1.0f);
	
	static void S_RemovePhysBody(PhysBody3D* physBody);

	static void UpdatePhysBodyPos(PhysBody3D* physBody);
	static void UpdatePhysBodyRotation(PhysBody3D* physBody);
	static void UpdatePhysBodyScaleBox(PhysBody3D* physBody);
	static void UpdatePhysBodyScaleSphere(PhysBody3D* physBody, float radius);
	static void UpdatePhysBodyScaleCylinder(PhysBody3D* physBody, float radius, float height);

	//TODO: this is temporaly here
	static std::vector <PhysBody3D*> physBodies;

	static void SetGlobalGravity(float3 grav);
	static void SetGlobalGravityAtFirst(float3 grav);

	float3 GetGlobalGravity();
	bool hasToChangeGravity;
	float3 gravityToChange;

	void PrepareNewGravityAtLast(float3 grav);
	void SetNewGravityAtLast();

	static btCollisionWorld::AllHitsRayResultCallback RayCastLine(const float3 start, const float3 end);

private:

	btDefaultCollisionConfiguration* collision_conf = nullptr;
	btCollisionDispatcher* dispatcher = nullptr;
	btBroadphaseInterface* broad_phase = nullptr;
	btSequentialImpulseConstraintSolver* solver = nullptr;
	static btDiscreteDynamicsWorld* world;

	bool debugDraw = false;

	std::vector <btCollisionShape*> shapes;
	std::vector <btDefaultMotionState*> motions;
	std::vector <btTypedConstraint*> constraints;

	bool hasToSetRenderBuffers = false;
};