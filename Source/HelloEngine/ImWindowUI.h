#pragma once
#include "ImWindow.h"
#include "SceneCameraObject.h"
#include "ImGuizmo/ImGuizmo.h"

class ModuleCamera3D;

class ImWindowUI : public ImWindow
{
public:
	ImWindowUI();

	~ImWindowUI();

	void Update() override;

private:
	/// <summary>
	/// This reads necessary Input for the ImGuizmo Manipulate.
	/// </summary>
	void DetectImGuizmoInput();

	void DetectClick();

private:
	ModuleCamera3D* _moduleCamera = nullptr;

	SceneCameraObject* _UICamera = nullptr;

	int _sceneWidth = 1;

	float4x4 _identity = float4x4::identity;

	ImGuizmo::MODE _imMode = ImGuizmo::MODE::WORLD;

	ImGuizmo::OPERATION _imOperation = ImGuizmo::OPERATION::TRANSLATE;

	int _sceneHeight = 1;
};