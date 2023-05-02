#pragma once

#include "Detour/DetourNavMesh.h"
#include "InputGeom.h"

class dtNavMesh;
struct BuildSettings;

struct NavMeshSetHeader
{
	int numTiles;
	dtNavMeshParams params;
};

struct NavMeshTileHeader
{
	dtTileRef tileRef;
	int dataSize;
};

class NavMeshImporter
{
public:
	static bool SaveNavMesh(const char* assets_path, const dtNavMesh* const navMesh, const BuildSettings* const buildSettings, const InputGeom* const geom);

	static bool LoadNavMesh(const char* assets_path, dtNavMesh* const navMesh, BuildSettings* const buildSettings, InputGeom* const geom);
};