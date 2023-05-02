#pragma once
#include "ImWindow.h"

struct BuildSettings;
struct SimpleMesh;
class NavMeshBuilder;

class ImWindowNavMesh : public ImWindow
{
public:
	ImWindowNavMesh();

	~ImWindowNavMesh();

	void Update() override;
private:
	// Variables for reimport 
	bool _bakeRequest = false;

	uint _bakeCounter = 100;

	BuildSettings* buildSettings = nullptr;
	const SimpleMesh* mesh = nullptr;
	const NavMeshBuilder* builder = nullptr;
};