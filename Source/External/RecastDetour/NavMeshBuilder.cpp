#include "NavMeshBuilder.h"
#include "InputGeom.h"

#include "Detour/DetourNavMesh.h"
#include "Detour/DetourNavMeshQuery.h"
#include "Detour/DetourNavMeshBuilder.h"
#include "DebugUtils/SampleInterfaces.h"
#include "DebugUtils/DetourDebugDraw.h"
#include "DebugUtils/RecastDebugDraw.h"

#include "ImGui/imgui.h"

#include "Globals.h"
#include "Application.h"

#include "ModuleRenderer3D.h"
#include "ModuleNavMesh.h"

#include "Console.h"

using CSL = Engine::Console;

inline unsigned int nextPow2(unsigned int v)
{
	v--;
	v |= v >> 1;
	v |= v >> 2;
	v |= v >> 4;
	v |= v >> 8;
	v |= v >> 16;
	v++;
	return v;
}

inline unsigned int ilog2(unsigned int v)
{
	unsigned int r;
	unsigned int shift;
	r = (v > 0xffff) << 4; v >>= r;
	shift = (v > 0xff) << 3; v >>= shift; r |= shift;
	shift = (v > 0xf) << 2; v >>= shift; r |= shift;
	shift = (v > 0x3) << 1; v >>= shift; r |= shift;
	r |= (v >> 1);
	return r;
}

NavMeshBuilder::NavMeshBuilder() :
	m_geom(nullptr),
	m_navMesh(nullptr),
	m_navQuery(nullptr),
	m_keepInterResults(false),
	m_buildAll(true),
	m_totalBuildTimeMs(0),
	m_triareas(nullptr),
	m_solid(nullptr),
	m_chf(nullptr),
	m_cset(nullptr),
	m_pmesh(nullptr),
	m_dmesh(nullptr),
	m_maxTiles(0),
	m_maxPolysPerTile(0),
	m_navMeshDrawFlags(DU_DRAWNAVMESH_OFFMESHCONS | DU_DRAWNAVMESH_CLOSEDLIST),
	m_tileBuildTime(0.0f),
	m_tileMemUsage(0.0f),
	m_tileTriCount(0)
{
	m_navQuery = dtAllocNavMeshQuery();
	memset(m_lastBuiltTileBmin, 0, sizeof(m_lastBuiltTileBmin));
	memset(m_lastBuiltTileBmax, 0, sizeof(m_lastBuiltTileBmax));
}

NavMeshBuilder::~NavMeshBuilder()
{
	CleanUp();
	dtFreeNavMesh(m_navMesh);
	m_navMesh = nullptr;
	dtFreeNavMeshQuery(m_navQuery);
	m_navQuery = nullptr;
}

void NavMeshBuilder::CleanUp()
{
	RELEASE_ARRAY(m_triareas);
	rcFreeHeightField(m_solid);
	m_solid = nullptr;
	rcFreeCompactHeightfield(m_chf);
	m_chf = nullptr;
	rcFreeContourSet(m_cset);
	m_cset = nullptr;
	rcFreePolyMesh(m_pmesh);
	m_pmesh = nullptr;
	rcFreePolyMeshDetail(m_dmesh);
	m_dmesh = nullptr;
}

bool NavMeshBuilder::HandleBuild()
{
	if (!m_geom || !m_geom->getMesh())
	{
		CSL::S_Log("buildTiledNavigation: No vertices and triangles.", LogType::ERR);
		return false;
	}

	dtFreeNavMesh(m_navMesh);

	m_navMesh = dtAllocNavMesh();
	if (!m_navMesh)
	{
		CSL::S_Log("buildTiledNavigation: Could not allocate navmesh.", LogType::ERR);
		return false;
	}

	dtNavMeshParams params;
	rcVcopy(params.orig, m_geom->getMeshBoundsMin());
	params.tileWidth = buildSettings->tileSize * buildSettings->cellSize;
	params.tileHeight = buildSettings->tileSize * buildSettings->cellSize;
	params.maxTiles = m_maxTiles;
	params.maxPolys = m_maxPolysPerTile;

	dtStatus status;

	status = m_navMesh->init(&params);
	if (dtStatusFailed(status))
	{
		CSL::S_Log("buildTiledNavigation: Could not init navmesh.", LogType::ERR);
		return false;
	}

	status = m_navQuery->init(m_navMesh, 2048);
	if (dtStatusFailed(status))
	{
		CSL::S_Log("buildTiledNavigation: Could not init Detour navmesh query", LogType::ERR);
		return false;
	}

	if (m_buildAll)
		BuildAllTiles();

	return true;
}

bool NavMeshBuilder::BuildAllTiles()
{
	if (!m_geom)
		return false;
	if (!m_navMesh)
		return false;

	const float* bmin = m_geom->getMeshBoundsMin();
	const float* bmax = m_geom->getMeshBoundsMax();
	int gw = 0, gh = 0;
	rcCalcGridSize(bmin, bmax, buildSettings->cellSize, &gw, &gh);
	const int ts = (int)buildSettings->tileSize;
	const int tw = (gw + ts - 1) / ts;
	const int th = (gh + ts - 1) / ts;
	const float tcs = buildSettings->tileSize * buildSettings->cellSize;

	for (int y = 0; y < th; ++y)
	{
		for (int x = 0; x < tw; ++x)
		{
			m_lastBuiltTileBmin[0] = bmin[0] + x * tcs;
			m_lastBuiltTileBmin[1] = bmin[1];
			m_lastBuiltTileBmin[2] = bmin[2] + y * tcs;

			m_lastBuiltTileBmax[0] = bmin[0] + (x + 1) * tcs;
			m_lastBuiltTileBmax[1] = bmax[1];
			m_lastBuiltTileBmax[2] = bmin[2] + (y + 1) * tcs;

			int dataSize = 0;
			unsigned char* data = BuildTile(x, y, m_lastBuiltTileBmin, m_lastBuiltTileBmax, dataSize);
			if (data)
			{
				// Remove any previous data (navmesh owns and deletes the data).
				m_navMesh->removeTile(m_navMesh->getTileRefAt(x, y, 0), 0, 0);
				// Let the navmesh own the data.
				dtStatus status = m_navMesh->addTile(data, dataSize, DT_TILE_FREE_DATA, 0, 0);
				if (dtStatusFailed(status))
					dtFree(data);
			}
		}
	}

	return true;
}

unsigned char* NavMeshBuilder::BuildTile(const int tx, const int ty, const float* bmin, const float* bmax, int& dataSize)
{
	if (!m_geom || !m_geom->getMesh() || !m_geom->getChunkyMesh())
	{
		CSL::S_Log("buildNavigation: Input mesh is not specified.", LogType::ERR);
		return 0;
	}

	m_tileMemUsage = 0.0f;
	m_tileBuildTime = 0.0f;

	rcContext m_ctx;

	CleanUp();

	const float* verts = (float*)&m_geom->getMesh()->vertices[0];
	const int nverts = m_geom->getMesh()->vertices.size();
	const int ntris = m_geom->getMesh()->indices.size() / 3;
	const rcChunkyTriMesh* chunkyMesh = m_geom->getChunkyMesh();

	// Init build configuration from GUI
	memset(&m_cfg, 0, sizeof(m_cfg));
	m_cfg.cs = buildSettings->cellSize;
	m_cfg.ch = buildSettings->cellHeight;
	m_cfg.walkableSlopeAngle = buildSettings->agentMaxSlope;
	m_cfg.walkableHeight = (int)ceilf(buildSettings->agentHeight / m_cfg.ch);
	m_cfg.walkableClimb = (int)floorf(buildSettings->agentMaxClimb / m_cfg.ch);
	m_cfg.walkableRadius = (int)ceilf(buildSettings->agentRadius / m_cfg.cs);
	m_cfg.maxEdgeLen = (int)(buildSettings->edgeMaxLen / buildSettings->cellSize);
	m_cfg.maxSimplificationError = buildSettings->edgeMaxError;
	m_cfg.minRegionArea = (int)rcSqr(buildSettings->regionMinSize);		// Note: area = size*size
	m_cfg.mergeRegionArea = (int)rcSqr(buildSettings->regionMergeSize);	// Note: area = size*size
	m_cfg.maxVertsPerPoly = (int)buildSettings->vertsPerPoly;
	m_cfg.tileSize = (int)buildSettings->tileSize;
	m_cfg.borderSize = m_cfg.walkableRadius + 3; // Reserve enough padding.
	m_cfg.width = m_cfg.tileSize + m_cfg.borderSize * 2;
	m_cfg.height = m_cfg.tileSize + m_cfg.borderSize * 2;
	m_cfg.detailSampleDist = buildSettings->detailSampleDist < 0.9f ? 0 : buildSettings->cellSize * buildSettings->detailSampleDist;
	m_cfg.detailSampleMaxError = buildSettings->cellHeight * buildSettings->detailSampleMaxError;

	// Expand the heighfield bounding box by border size to find the extents of geometry we need to build this tile.
	//
	// This is done in order to make sure that the navmesh tiles connect correctly at the borders,
	// and the obstacles close to the border work correctly with the dilation process.
	// No polygons (or contours) will be created on the border area.
	//
	// IMPORTANT!
	//
	//   :''''''''':
	//   : +-----+ :
	//   : |     | :
	//   : |     |<--- tile to build
	//   : |     | :  
	//   : +-----+ :<-- geometry needed
	//   :.........:
	//
	// You should use this bounding box to query your input geometry.
	//
	// For example if you build a navmesh for terrain, and want the navmesh tiles to match the terrain tile size
	// you will need to pass in data from neighbour terrain tiles too! In a simple case, just pass in all the 8 neighbours,
	// or use the bounding box below to only pass in a sliver of each of the 8 neighbours.
	rcVcopy(m_cfg.bmin, bmin);
	rcVcopy(m_cfg.bmax, bmax);
	m_cfg.bmin[0] -= m_cfg.borderSize * m_cfg.cs;
	m_cfg.bmin[2] -= m_cfg.borderSize * m_cfg.cs;
	m_cfg.bmax[0] += m_cfg.borderSize * m_cfg.cs;
	m_cfg.bmax[2] += m_cfg.borderSize * m_cfg.cs;

	// Reset build times gathering.

	// Start the build process.

	//CSL::S_Log("Building navigation:");

	//std::string temp;

	//temp = "-" + std::to_string(m_cfg.width) + "x" + std::to_string(m_cfg.height) + "cells";

	//CSL::S_Log(temp.c_str());

	//temp = "-" + std::to_string(nverts / 1000.0f) + "x" + std::to_string(ntris / 1000.0f) + "cells";

	//CSL::S_Log(" - %.1fK verts, %.1fK tris", nverts / 1000.0f, ntris / 1000.0f);

	// Allocate voxel heightfield where we rasterize our input data to.
	m_solid = rcAllocHeightfield();
	if (!m_solid)
	{
		CSL::S_Log("buildNavigation: Out of memory 'solid'.", LogType::ERR);
		return 0;
	}
	if (!rcCreateHeightfield(&m_ctx, *m_solid, m_cfg.width, m_cfg.height, m_cfg.bmin, m_cfg.bmax, m_cfg.cs, m_cfg.ch))
	{
		CSL::S_Log("buildNavigation: Could not create solid heightfield.", LogType::ERR);
		return 0;
	}

	// Allocate array that can hold triangle flags.
	// If you have multiple meshes you need to process, allocate
	// and array which can hold the max number of triangles you need to process.
	m_triareas = new unsigned char[chunkyMesh->maxTrisPerChunk];
	if (!m_triareas)
	{
		std::string temp;
		temp = "buildNavigation: Out of memory 'm_triareas' (" + std::to_string(chunkyMesh->maxTrisPerChunk) + ").";
		CSL::S_Log(temp.c_str());
		return 0;
	}

	float tbmin[2], tbmax[2];
	tbmin[0] = m_cfg.bmin[0];
	tbmin[1] = m_cfg.bmin[2];
	tbmax[0] = m_cfg.bmax[0];
	tbmax[1] = m_cfg.bmax[2];
	int cid[512];// TODO: Make grow when returning too many items.
	const int ncid = rcGetChunksOverlappingRect(chunkyMesh, tbmin, tbmax, cid, 512);
	if (!ncid)
		return 0;

	m_tileTriCount = 0;

	for (int i = 0; i < ncid; ++i)
	{
		const rcChunkyTriMeshNode& node = chunkyMesh->nodes[cid[i]];
		const int* ctris = &chunkyMesh->tris[node.i * 3];
		const int nctris = node.n;

		m_tileTriCount += nctris;

		memset(m_triareas, 0, nctris * sizeof(unsigned char));
		rcMarkWalkableTriangles(&m_ctx, m_cfg.walkableSlopeAngle,
			verts, nverts, ctris, nctris, m_triareas);

		if (!rcRasterizeTriangles(&m_ctx, verts, nverts, ctris, m_triareas, nctris, *m_solid, m_cfg.walkableClimb))
			return 0;
	}

	RELEASE_ARRAY(m_triareas);


	// Once all geometry is rasterized, we do initial pass of filtering to
	// remove unwanted overhangs caused by the conservative rasterization
	// as well as filter spans where the character cannot possibly stand.
	rcFilterLowHangingWalkableObstacles(&m_ctx, m_cfg.walkableClimb, *m_solid);
	rcFilterLedgeSpans(&m_ctx, m_cfg.walkableHeight, m_cfg.walkableClimb, *m_solid);
	rcFilterWalkableLowHeightSpans(&m_ctx, m_cfg.walkableHeight, *m_solid);

	// Compact the heightfield so that it is faster to handle from now on.
	// This will result more cache coherent data as well as the neighbours
	// between walkable cells will be calculated.
	m_chf = rcAllocCompactHeightfield();
	if (!m_chf)
	{
		CSL::S_Log("buildNavigation: Out of memory 'chf'.", LogType::ERR);
		return 0;
	}
	if (!rcBuildCompactHeightfield(&m_ctx, m_cfg.walkableHeight, m_cfg.walkableClimb, *m_solid, *m_chf))
	{
		CSL::S_Log("buildNavigation: Could not build compact data.", LogType::ERR);
		return 0;
	}


	rcFreeHeightField(m_solid);
	m_solid = 0;


	// Erode the walkable area by agent radius.
	if (!rcErodeWalkableArea(&m_ctx, m_cfg.walkableRadius, *m_chf))
	{
		CSL::S_Log("buildNavigation: Could not erode.", LogType::ERR);
		return 0;
	}

	// (Optional) Mark areas.
	//const ConvexVolume* vols = m_geom->getConvexVolumes();
	//for (int i = 0; i < m_geom->getConvexVolumeCount(); ++i)
	//	rcMarkConvexPolyArea(&m_ctx, vols[i].verts, vols[i].nverts, vols[i].hmin, vols[i].hmax, (unsigned char)vols[i].area, *m_chf);


	// Partition the heightfield so that we can use simple algorithm later to triangulate the walkable areas.
	// There are 3 martitioning methods, each with some pros and cons:
	// 1) Watershed partitioning
	//   - the classic Recast partitioning
	//   - creates the nicest tessellation
	//   - usually slowest
	//   - partitions the heightfield into nice regions without holes or overlaps
	//   - the are some corner cases where this method creates produces holes and overlaps
	//      - holes may appear when a small obstacles is close to large open area (triangulation can handle this)
	//      - overlaps may occur if you have narrow spiral corridors (i.e stairs), this make triangulation to fail
	//   * generally the best choice if you precompute the nacmesh, use this if you have large open areas
	// 2) Monotone partioning
	//   - fastest
	//   - partitions the heightfield into regions without holes and overlaps (guaranteed)
	//   - creates long thin polygons, which sometimes causes paths with detours
	//   * use this if you want fast navmesh generation
	// 3) Layer partitoining
	//   - quite fast
	//   - partitions the heighfield into non-overlapping regions
	//   - relies on the triangulation code to cope with holes (thus slower than monotone partitioning)
	//   - produces better triangles than monotone partitioning
	//   - does not have the corner cases of watershed partitioning
	//   - can be slow and create a bit ugly tessellation (still better than monotone)
	//     if you have large open areas with small obstacles (not a problem if you use tiles)
	//   * good choice to use for tiled navmesh with medium and small sized tiles


	// Prepare for region partitioning, by calculating distance field along the walkable surface.
	//if (!rcBuildDistanceField(&m_ctx, *m_chf))
	//{
	//	LOG(LogType::L_ERROR, "buildNavigation: Could not build distance field.");
	//	return 0;
	//}

	//// Partition the walkable surface into simple regions without holes.
	//if (!rcBuildRegions(&m_ctx, *m_chf, m_cfg.borderSize, m_cfg.minRegionArea, m_cfg.mergeRegionArea))
	//{
	//	LOG(LogType::L_ERROR, "buildNavigation: Could not build watershed regions.");
	//	return 0;
	//}

	if (!rcBuildLayerRegions(&m_ctx, *m_chf, m_cfg.borderSize, m_cfg.minRegionArea))
	{
		CSL::S_Log("buildNavigation: Could not build layer regions.", LogType::ERR);
		return 0;
	}

	// Create contours.
	m_cset = rcAllocContourSet();
	if (!m_cset)
	{
		CSL::S_Log("buildNavigation: Out of memory 'cset'.", LogType::ERR);
		return 0;
	}
	if (!rcBuildContours(&m_ctx, *m_chf, m_cfg.maxSimplificationError, m_cfg.maxEdgeLen, *m_cset))
	{
		CSL::S_Log("buildNavigation: Could not create contours.", LogType::ERR);
		return 0;
	}

	if (m_cset->nconts == 0)
	{
		return 0;
	}

	// Build polygon navmesh from the contours.
	m_pmesh = rcAllocPolyMesh();
	if (!m_pmesh)
	{
		CSL::S_Log("buildNavigation: Out of memory 'pmesh'.", LogType::ERR);
		return 0;
	}
	if (!rcBuildPolyMesh(&m_ctx, *m_cset, m_cfg.maxVertsPerPoly, *m_pmesh))
	{
		CSL::S_Log("buildNavigation: Could not triangulate contours.", LogType::ERR);
		return 0;
	}

	// Build detail mesh.
	m_dmesh = rcAllocPolyMeshDetail();
	if (!m_dmesh)
	{
		CSL::S_Log("buildNavigation: Out of memory 'dmesh'.", LogType::ERR);
		return 0;
	}

	if (!rcBuildPolyMeshDetail(&m_ctx, *m_pmesh, *m_chf,
		m_cfg.detailSampleDist, m_cfg.detailSampleMaxError,
		*m_dmesh))
	{
		CSL::S_Log("buildNavigation: Could build polymesh detail.", LogType::ERR);
		return 0;
	}

	rcFreeCompactHeightfield(m_chf);
	m_chf = 0;
	rcFreeContourSet(m_cset);
	m_cset = 0;

	unsigned char* navData = 0;
	int navDataSize = 0;
	if (m_cfg.maxVertsPerPoly <= DT_VERTS_PER_POLYGON)
	{
		if (m_pmesh->nverts >= 0xffff)
		{
			// The vertex indices are ushorts, and cannot point to more than 0xffff vertices.
			std::string temp;
			temp = "Too many vertices per tile " + std::to_string(m_pmesh->nverts) + "(max: " + std::to_string(0xffff) + ").";
			CSL::S_Log(temp.c_str());
			return 0;
		}

		// Update poly flags from areas.
		for (int i = 0; i < m_pmesh->npolys; ++i)
		{
			if (m_pmesh->areas[i] == RC_WALKABLE_AREA)
				m_pmesh->areas[i] = (int)Walkability::WALKABLE;

			m_pmesh->flags[i] = 1 << m_pmesh->areas[i];
		}

		dtNavMeshCreateParams params;
		memset(&params, 0, sizeof(params));
		params.verts = m_pmesh->verts;
		params.vertCount = m_pmesh->nverts;
		params.polys = m_pmesh->polys;
		params.polyAreas = m_pmesh->areas;
		params.polyFlags = m_pmesh->flags;
		params.polyCount = m_pmesh->npolys;
		params.nvp = m_pmesh->nvp;
		params.detailMeshes = m_dmesh->meshes;
		params.detailVerts = m_dmesh->verts;
		params.detailVertsCount = m_dmesh->nverts;
		params.detailTris = m_dmesh->tris;
		params.detailTriCount = m_dmesh->ntris;
		params.offMeshConVerts = m_geom->getOffMeshConnectionVerts();
		params.offMeshConRad = m_geom->getOffMeshConnectionRads();
		params.offMeshConDir = m_geom->getOffMeshConnectionDirs();
		params.offMeshConAreas = m_geom->getOffMeshConnectionAreas();
		params.offMeshConFlags = m_geom->getOffMeshConnectionFlags();
		params.offMeshConUserID = m_geom->getOffMeshConnectionId();
		params.offMeshConCount = m_geom->getOffMeshConnectionCount();
		params.walkableHeight = buildSettings->agentHeight;
		params.walkableRadius = buildSettings->agentRadius;
		params.walkableClimb = buildSettings->agentMaxClimb;
		params.tileX = tx;
		params.tileY = ty;
		params.tileLayer = 0;
		rcVcopy(params.bmin, m_pmesh->bmin);
		rcVcopy(params.bmax, m_pmesh->bmax);
		params.cs = m_cfg.cs;
		params.ch = m_cfg.ch;
		params.buildBvTree = true;

		if (!dtCreateNavMeshData(&params, &navData, &navDataSize))
		{
			CSL::S_Log("Could not build Detour navmesh.", LogType::ERR);
			return 0;
		}
	}
	m_tileMemUsage = navDataSize / 1024.0f;

	// Show performance stats.
	std::string temp;
	temp = ">> Polymesh: " + std::to_string(m_pmesh->nverts) + " vertices  " + std::to_string(m_pmesh->npolys) + " polygons";
	CSL::S_Log(temp.c_str());

	dataSize = navDataSize;
	return navData;
}

void NavMeshBuilder::HandleMeshChanged(InputGeom* geom, BuildSettings* settings)
{
	//Normal sample settings 
	m_geom = geom;

	//Copy Settings
	buildSettings = settings;

	CleanUp();

	dtFreeNavMesh(m_navMesh);
	m_navMesh = nullptr;
}

void NavMeshBuilder::HandleSettings()
{
	if (m_geom)
	{
		char text[64];
		int gw = 0, gh = 0;
		const float* bmin = m_geom->getMeshBoundsMin();
		const float* bmax = m_geom->getMeshBoundsMax();
		rcCalcGridSize(bmin, bmax, buildSettings->cellSize, &gw, &gh);
		const int ts = (int)buildSettings->tileSize;
		const int tw = (gw + ts - 1) / ts;
		const int th = (gh + ts - 1) / ts;
		snprintf(text, 64, "Tiles  %d x %d", tw, th);
		//imguiValue(text);

		// Max tiles and max polys affect how the tile IDs are caculated.
		// There are 22 bits available for identifying a tile and a polygon.
		int tileBits = rcMin((int)ilog2(nextPow2(tw * th)), 14);
		if (tileBits > 14) tileBits = 14;
		int polyBits = 22 - tileBits;
		m_maxTiles = 1 << tileBits;
		m_maxPolysPerTile = 1 << polyBits;
		snprintf(text, 64, "Max Tiles  %d", m_maxTiles);
		//imguiValue(text);
		snprintf(text, 64, "Max Polys  %d", m_maxPolysPerTile);
		//imguiValue(text);
	}
	else
	{
		m_maxTiles = 0;
		m_maxPolysPerTile = 0;
	}
}

void NavMeshBuilder::DebugDraw()
{
	if (m_geom)
	{
		const float* bmin = m_geom->getMeshBoundsMin();
		const float* bmax = m_geom->getMeshBoundsMax();
		DrawBoundaries(bmin[0], bmin[1], bmin[2], bmax[0], bmax[1], bmax[2]);
	}

	if (m_navMesh)
	{
		DebugDrawGL dd;

		if (m_navMesh != nullptr)
			duDebugDrawNavMeshWithClosedList(&dd, *m_navMesh, *m_navQuery, m_navMeshDrawFlags);
	}
}

void NavMeshBuilder::DrawBoundaries(float minx, float miny, float minz, float maxx, float maxy, float maxz)
{
	////Top
	//app->renderer3D->AddDebugLines(float3(minx, miny, minz), float3(maxx, miny, minz), float3(1.0f, 1.0f, 1.0f));
	//app->renderer3D->AddDebugLines(float3(maxx, miny, minz), float3(maxx, miny, maxz), float3(1.0f, 1.0f, 1.0f));
	//app->renderer3D->AddDebugLines(float3(maxx, miny, maxz), float3(minx, miny, maxz), float3(1.0f, 1.0f, 1.0f));
	//app->renderer3D->AddDebugLines(float3(minx, miny, maxz), float3(minx, miny, minz), float3(1.0f, 1.0f, 1.0f));
	//
	////Bottom	
	//app->renderer3D->AddDebugLines(float3(minx, maxy, minz), float3(maxx, maxy, minz), float3(1.0f, 1.0f, 1.0f));
	//app->renderer3D->AddDebugLines(float3(maxx, maxy, minz), float3(maxx, maxy, maxz), float3(1.0f, 1.0f, 1.0f));
	//app->renderer3D->AddDebugLines(float3(maxx, maxy, maxz), float3(minx, maxy, maxz), float3(1.0f, 1.0f, 1.0f));
	//app->renderer3D->AddDebugLines(float3(minx, maxy, maxz), float3(minx, maxy, minz), float3(1.0f, 1.0f, 1.0f));
	//
	////Sides		
	//app->renderer3D->AddDebugLines(float3(minx, miny, minz), float3(minx, maxy, minz), float3(1.0f, 1.0f, 1.0f));
	//app->renderer3D->AddDebugLines(float3(maxx, miny, minz), float3(maxx, maxy, minz), float3(1.0f, 1.0f, 1.0f));
	//app->renderer3D->AddDebugLines(float3(maxx, miny, maxz), float3(maxx, maxy, maxz), float3(1.0f, 1.0f, 1.0f));
	//app->renderer3D->AddDebugLines(float3(minx, miny, maxz), float3(minx, maxy, maxz), float3(1.0f, 1.0f, 1.0f));
}

void NavMeshBuilder::SetNavMesh(dtNavMesh* newNavMesh)
{
	if (m_navMesh)
		dtFreeNavMesh(m_navMesh);

	m_navMesh = newNavMesh;
	m_navQuery->init(m_navMesh, 2048);

	m_maxPolysPerTile = m_navMesh->getParams()->maxPolys;
	m_maxTiles = m_navMesh->getParams()->maxTiles;
}