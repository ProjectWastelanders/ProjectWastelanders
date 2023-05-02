#include "Headers.h"
#include "ImWindowNavMesh.h"
#include "ModuleNavMesh.h"
#include "InputGeom.h"
#include "NavMeshBuilder.h"
#include "ModuleRenderer3D.h"

ImWindowNavMesh::ImWindowNavMesh()
{
	buildSettings = ModuleNavMesh::S_GetBuildSettings();
}

ImWindowNavMesh::~ImWindowNavMesh()
{
}

void ImWindowNavMesh::Update()
{
    ImGui::Begin(" Navigator", &isEnabled);
    ImGui::Dummy({ 0,10 });
    ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x - 180) / 2);
    if (ImGui::Button("Bake", ImVec2(180.0f, 20.0f)))
    {
        _bakeRequest = true;
        _bakeCounter = 100;
    }
    ImGui::Dummy({ 0,10 });

    ImGui::Checkbox("DebugDraw", &ModuleRenderer3D::drawNavMesh);

    ImGui::Text("Agent Properties");
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::PushItemWidth(180);
    ImGui::DragFloat("Agent radius", &buildSettings->agentRadius, 0.1f, 0.f);
    ImGui::DragFloat("Agent height", &buildSettings->agentHeight, 0.1f, 0.f);
    ImGui::DragFloat("Agent max climb", &buildSettings->agentMaxClimb, 0.1f, 0.f);
    ImGui::DragFloat("Agent max slope", &buildSettings->agentMaxSlope, 0.1f, 0.f);
    ImGui::PopItemWidth();
    ImGui::Dummy({ 0,10 });

    ImGui::Text("NavMesh Properties");
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::PushItemWidth(180);
    ImGui::DragFloat("Cell size", &buildSettings->cellSize, 0.1f, 0.f);
    ImGui::DragFloat("Region minimum size", &buildSettings->regionMinSize, 0.1f, 0.f);
    ImGui::DragFloat("Region merge size", &buildSettings->regionMergeSize, 0.1f, 0.f);
    ImGui::DragFloat("Edge max lenght", &buildSettings->edgeMaxLen, 0.1f, 0.f);
    ImGui::DragFloat("Edge max error", &buildSettings->edgeMaxError, 0.1f, 0.f);
    ImGui::DragFloat("Detail sample distance", &buildSettings->detailSampleDist, 0.1f, 0.f);
    ImGui::DragFloat("Detail sample max error", &buildSettings->detailSampleMaxError, 0.1f, 0.f);
    ImGui::DragFloat("Tile Size", &buildSettings->tileSize, 0.1f, 0.f);
    ImGui::PopItemWidth();
    ImGui::Dummy({ 0,10 });

    if (ImGui::CollapsingHeader("NavMeshBuilder"))
    {
        mesh = ModuleNavMesh::S_GetInputGeom()->getMesh();
        builder = ModuleNavMesh::S_GetNavMeshBuilder();

        ImGui::Text("NaveMesh Geometry");
        ImGui::Separator();
        ImGui::Spacing();

        if(mesh && builder)
        {
            if (mesh->vertices.size() <= 0 && mesh->indices.size() <= 0)
                ImGui::Text("No Geometry added");
            else
            {
                ImGui::Text("Verts: %d", mesh->vertices.size());
                ImGui::Text("Indices: %d", mesh->indices.size());

                ImGui::Spacing();
                ImGui::Spacing();

                ImGui::Text("Max Tiles: %i", builder->GetMaxTyles());
                ImGui::Text("Max Polys per tiles: %i", builder->GetMaxPolyTile());
                ImGui::Text("Cell size: %.4f", buildSettings->cellSize);
                ImGui::Text("Cell Height: %.4f", buildSettings->cellHeight);

                if (builder->GetPolyMesh())
                {
                    ImGui::Spacing();
                    ImGui::Separator();
                    ImGui::Separator();
                    ImGui::Spacing();
                    ImGui::Text("PolyMesh");

                    ImGui::Text("Verts: %i", builder->GetPolyMesh()->nverts);
                    ImGui::Text("Polys: %i", builder->GetPolyMesh()->npolys);
                }
            }
        }        
    }

    ImGui::End();

    if (_bakeRequest)
    {
        ImGui::OpenPopup("Baking Scene");

        if (ImGui::BeginPopupModal("Baking Scene", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text("Building the NavMesh...");
            ImGui::EndPopup();
        }

        if (_bakeCounter <= 0)
        {
            ModuleNavMesh::S_BakeNavMesh();

            _bakeRequest = false;
        }
        else
        {
            _bakeCounter--;
        }
    }
}
