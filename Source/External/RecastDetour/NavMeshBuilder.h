#pragma once

#include "Recast/Recast.h"
#include "Detour/DetourNavMesh.h"
#include "Detour/DetourNavMeshQuery.h"

#include "InputGeom.h"

class BuildSettings;

enum class Walkability
{
	WALKABLE,
	NOT_WALKABLE
};

class NavMeshBuilder {
public:
	NavMeshBuilder();
	~NavMeshBuilder();

	void CleanUp();

	bool HandleBuild();
	bool BuildAllTiles();
	unsigned char* BuildTile(const int tx, const int ty, const float* bmin, const float* bmax, int& dataSize);
	void HandleMeshChanged(InputGeom* geom, BuildSettings* settings);
	void HandleSettings();

	void DebugDraw();
	void DrawBoundaries(float minx, float miny, float minz, float maxx, float maxy, float maxz);

	inline dtNavMesh* GetNavMesh() { return m_navMesh; };
	inline dtNavMeshQuery* GetNavMeshQuery() { return m_navQuery; };
	inline const InputGeom* GetGeom() const { return m_geom; };
	inline const rcPolyMesh* GetPolyMesh() const { return m_pmesh; };
	inline const int GetMaxTyles() const { return m_maxTiles; };
	inline const int GetMaxPolyTile() const { return m_maxPolysPerTile; };
	void SetNavMesh(dtNavMesh* newNavMesh);

private:
	class InputGeom* m_geom;
	class dtNavMesh* m_navMesh;
	class dtNavMeshQuery* m_navQuery;

	unsigned char m_navMeshDrawFlags;

	bool m_keepInterResults;
	bool m_buildAll;
	float m_totalBuildTimeMs;

	unsigned char* m_triareas;
	rcHeightfield* m_solid;
	rcCompactHeightfield* m_chf;
	rcContourSet* m_cset;
	rcPolyMesh* m_pmesh;
	rcConfig m_cfg;
	rcPolyMeshDetail* m_dmesh;
	BuildSettings* buildSettings;

	//Tile Settings
	int   m_maxTiles;
	int   m_maxPolysPerTile;

	unsigned int m_tileCol;
	float m_lastBuiltTileBmin[3];
	float m_lastBuiltTileBmax[3];
	float m_tileBuildTime;
	float m_tileMemUsage;
	int m_tileTriCount;
};
