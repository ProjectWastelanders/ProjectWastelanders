#pragma once

#include "Module.h"
#include "Recast/Recast.h"
#include "Detour/DetourNavMesh.h"
#include "Detour/DetourNavMeshQuery.h"
#include <stack>

#define MAX_POLYS 256
#define MAX_SMOOTH 2048
#define MAX_ERROR 0.1f

class GameObject;
class InputGeom;
class NavMeshBuilder;
class ComponentAgent;
class btRigidBody;
class PhysicsComponent;

enum class PathType
{
	SMOOTH,
	STRAIGHT
};

enum class AgentType
{
	ENEMY,
	CHARACTER_1,
};

struct NavAgent
{
	NavAgent();

	float radius = 0.0f;
	float height = 0.0f;
	float maxClimb = 0.0f;
	int maxSlope = 0;

	float speed = 0.0f;
	float angularSpeed = 0.0f;
	float acceleration = 0.0f;
	float stoppingDistance = 0;

	std::vector<float3> path;
	PathType pathType = PathType::STRAIGHT;

	//Path Calculations Variables
	dtPolyRef m_startRef = 0;
	dtPolyRef m_endRef = 0;

	int m_npolys = 0;
	dtPolyRef m_polys[MAX_POLYS];
	dtPolyRef m_parent[MAX_POLYS];

	int m_nsmoothPath = 0;
	float m_smoothPath[MAX_SMOOTH * 3];

	int m_nstraightPath = 0;
	float m_straightPath[MAX_POLYS * 3];
	unsigned char m_straightPathFlags[MAX_POLYS];
	dtPolyRef m_straightPathPolys[MAX_POLYS];
};

class Pathfinder : public Module
{
public:
	Pathfinder();
	~Pathfinder();

	void Init(NavMeshBuilder* builder);
	std::vector<float3> CalculatePath(ComponentAgent* agent, float3 destination);
	void RenderPath(ComponentAgent* agent);
	bool MovePath(std::pair<ComponentAgent*, PhysicsComponent*> agentPair);

private:
	bool MoveTo(PhysicsComponent* agent, const NavAgent* const destination);
	bool LookAt(btRigidBody* rigidBody, float2 direction2D, float2 origin2D);
	bool SmoothLookAt(btRigidBody* rigidBody, float2 direction2D, float2 origin2D, float speed);

public:
	dtNavMesh* m_navMesh = nullptr;
	dtNavMeshQuery* m_navQuery = nullptr;
	dtQueryFilter m_filter;
	NavMeshBuilder* m_navMeshBuilder = nullptr;

	float3 hitPosition = float3::zero;
	float3 rayCast[2] = { float3::zero, float3::zero };
};

struct BuildSettings
{
	// Cell size in world units
	float cellSize = 0.3f;
	// Cell height in world units
	float cellHeight = 0.2f;
	// Agent height in world units
	float agentHeight = 2.0f;
	// Agent radius in world units
	float agentRadius = 0.5f;
	// Agent max climb in world units
	float agentMaxClimb = 0.9f;
	// Agent max slope in degrees
	float agentMaxSlope = 45.0f;
	// Region minimum size in voxels.
	// regionMinSize = sqrt(regionMinArea)
	float regionMinSize = 8.0f;
	// Region merge size in voxels.
	// regionMergeSize = sqrt(regionMergeArea)
	float regionMergeSize = 20.0f;
	// Edge max length in world units
	float edgeMaxLen = 12.0f;
	// Edge max error in voxels
	float edgeMaxError = 1.3f;	
	float vertsPerPoly;
	// Detail sample distance in voxels
	float detailSampleDist = 6.0f;
	// Detail sample max error in voxel heights.
	float detailSampleMaxError = 1.0f;
	// Partition type, see SamplePartitionType
	int partitionType;
	// Size of the tiles in voxels
	float tileSize = 32;
	// Bounds of the area to mesh
	float navMeshBMin[3];
	float navMeshBMax[3];
};

class ModuleNavMesh : public Module
{
public:
	ModuleNavMesh(bool start_enabled = true);

	~ModuleNavMesh();

	bool Start() override;

	UpdateStatus Update() override;

	static bool S_Save();

	static void S_Load(std::string navMeshPath = "null");

	static void S_CheckNavMeshIntersection(LineSegment raycast, int clickedMouseButton);

	static void S_ResetNavMeshes();

	static bool S_IsWalkable(float x, float z, float3& hitPoint);

	bool CleanUp() override;

	static void S_BakeNavMesh();

	static void S_AddGameObjectToNavMesh(GameObject* objectToAdd);

	static NavMeshBuilder* S_GetNavMeshBuilder() { return _navMeshBuilder; };
	static InputGeom* S_GetInputGeom() { return _geometry; };
	static BuildSettings* S_GetBuildSettings() { return _buildSettings; };
	static Pathfinder* S_GetPathfinding() { return _pathfinder; };

	static bool availableNavMesh;

private:
	static int AddAgentToList(ComponentAgent* const agent);

	static void RemoveAgentFromList(const int index);

private:
	static NavMeshBuilder* _navMeshBuilder;
	static Pathfinder* _pathfinder;
	static InputGeom* _geometry;
	static BuildSettings* _buildSettings;

	// Agent List manager
	static std::vector<std::pair<ComponentAgent*, PhysicsComponent*>> _agents;
	static std::stack<int> _freeSpace;

	friend class ComponentAgent;
};
