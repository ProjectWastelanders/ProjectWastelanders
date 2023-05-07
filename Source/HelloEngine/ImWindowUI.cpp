#include "Headers.h"
#include "ImWindowUI.h"
#include "ModuleCamera3D.h"
#include "ModuleResourceManager.h"
#include "ModuleLayers.h"
#include "LayerEditor.h"
#include "ModuleRenderer3D.h"
#include "MeshImporter.h"
#include "ModuleCommand.h"

ImWindowUI::ImWindowUI()
{
	windowName = "Canvas";

	isEnabled = true;

	_moduleCamera = Application::Instance()->camera;
	_UICamera = Application::Instance()->camera->UICamera;
}

ImWindowUI::~ImWindowUI()
{
}

void ImWindowUI::Update()
{
	if (ImGui::Begin(windowName.c_str(), 0, ImGuiWindowFlags_MenuBar))
	{
		isActive = true;
		if (ImGui::BeginMenuBar())
		{
			std::string option = "Change to ";
			option += _imMode == ImGuizmo::MODE::LOCAL ? "GLOBAL" : "LOCAL";
			if (ImGui::MenuItem(option.c_str()))
			{
				_imMode = _imMode == ImGuizmo::MODE::LOCAL ? ImGuizmo::MODE::WORLD : ImGuizmo::MODE::LOCAL;
			}
			ImGui::EndMenuBar();
		}

		ImGui::BeginChild("DropArea");
		{
			_UICamera->active = true;

			_moduleCamera->updateSceneCamera = ImGui::IsWindowHovered();

			ImVec2 sceneDimensions = ImGui::GetContentRegionAvail();

			if (sceneDimensions.x != _sceneWidth || sceneDimensions.y != _sceneHeight)
			{
				// If the size of this imgui window is different from the one stored.
				_sceneWidth = sceneDimensions.x;
				_sceneHeight = sceneDimensions.y;
				_UICamera->ChangeAspectRatio((float)_sceneWidth / (float)_sceneHeight);
			}
			//ImGuizmo::DrawGrid(Application::Instance()->camera->UICamera.GetViewMatrix(), Application::Instance()->camera->UICamera.GetProjectionMatrix(), &identity.v[0][0], 100);

			DetectImGuizmoInput();

			ImGui::Image((ImTextureID)_UICamera->frameBuffer.GetTexture(), ImGui::GetContentRegionAvail(), ImVec2(0, 1), ImVec2(1, 0));

			GameObject* selected = LayerEditor::selectedGameObject;

			if (selected != nullptr)
			{
				ImGuizmo::Enable(true);
				float4x4 auxiliarMatrix = selected->transform->GetGlobalMatrix();
				auxiliarMatrix.Transpose();

				// Could be done only when one of the 4 variables changes.
				ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowWidth(), ImGui::GetWindowHeight());
				ImGuizmo::SetDrawlist();

				static bool firstClick = false;
				static bool manipulated = false;
				static float4x4 currentGlobal;

				if (ImGuizmo::Manipulate(_UICamera->GetViewMatrix(), _UICamera->GetProjectionMatrix(), _imOperation, _imMode, &auxiliarMatrix.v[0][0]))
				{
					manipulated = true;
					if (!firstClick && ImGui::IsWindowHovered())
					{
						currentGlobal = selected->transform->GetGlobalMatrix();
						firstClick = true;
					}
					auxiliarMatrix.Transpose();
					selected->transform->SetLocalFromGlobal(auxiliarMatrix, _imOperation == ImGuizmo::OPERATION::SCALE);
				}

				if (ModuleInput::S_GetMouseButton(1) == KEY_UP && ImGui::IsWindowHovered() && manipulated)
				{
					manipulated = false;
					// If this auxiliar matrix has not been transposed, transpose ir before giving it to the changetransform command
					if (auxiliarMatrix.Col(3).xyz() == float3(0,0,0) && auxiliarMatrix.Col(3).w == 1)
						auxiliarMatrix.Transpose();

					/*ModuleCommand::S_ChangeTransform(auxiliarMatrix, currentGlobal, _imOperation == ImGuizmo::OPERATION::SCALE,
						std::bind(&TransformComponent::SetLocalFromGlobal, selected->transform, std::placeholders::_1, std::placeholders::_2));
					firstClick = false;*/
				}

			}

			if (!ImGuizmo::IsUsing())
				DetectClick();

			ImGui::SameLine(); ImGui::Button("Change to LOCAL");
		}
		ImGui::EndChild();

		// Create Droped mesh
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Model"))
			{
				//Drop asset from Asset window to scene window
				const uint* drop = (uint*)payload->Data;

				ResourceModel* resource = (ResourceModel*)ModuleResourceManager::S_LoadResource(*drop);

				MeshImporter::LoadModelIntoScene(resource);

				std::string popUpmessage = "Loaded Mesh: ";
				LayerEditor::S_AddPopUpMessage(popUpmessage);

			}
			else if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Mesh"))
			{
				const uint* drop = (uint*)payload->Data;

				ResourceMesh* resource = (ResourceMesh*)ModuleResourceManager::resources[*drop];

				GameObject* newGameObject = new GameObject(ModuleLayers::rootGameObject, resource->debugName);
				MeshRenderComponent* meshRender = newGameObject->AddComponent<MeshRenderComponent>();
				meshRender->CreateMesh(*drop);

				std::string popUpmessage = "Loaded Mesh: " + resource->debugName;
				LayerEditor::S_AddPopUpMessage(popUpmessage);

			}
			else if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Scene"))
			{
				const std::string drop = *(std::string*)payload->Data;

				ModuleLayers::S_RequestLoadScene(drop);

				std::string popUpmessage = "Loaded Scene: " + ModuleFiles::S_GetFileName(drop, false);
				LayerEditor::S_AddPopUpMessage(popUpmessage);
			}
			ImGui::EndDragDropTarget();
		}
	}
	else
	{
		_UICamera->active = false;
		isActive = false;
	}
	
	ImGui::End();
}

void ImWindowUI::DetectClick()
{
	if (ImGui::IsWindowHovered() && ModuleInput::S_GetMouseButton(1) == KEY_DOWN && ModuleInput::S_GetKey(SDL_SCANCODE_LALT) != KEY_REPEAT)
	{
		// Get mouse position normalized inside this window-----------------------------------------------------
		ImVec2 windowPos = ImGui::GetWindowPos();
		ImVec2 windowSize = ImGui::GetWindowSize();

		ImVec2 normalizedPos = { (float)ModuleInput::S_GetMouseX(), (float)ModuleInput::S_GetMouseY() };

		normalizedPos = { normalizedPos.x - windowPos.x,  normalizedPos.y - windowPos.y };

		float halfWindowWidth = (float)windowSize.x / 2;
		float halfWindowHeight = (float)windowSize.y / 2;

		float positionX = normalizedPos.x - halfWindowWidth;
		float positionY = normalizedPos.y - halfWindowHeight;

		positionX /= halfWindowWidth;
		positionY /= halfWindowHeight;

		positionY *= -1.0f;

		//--------------------------------------------------------------------------------------------------------

		LineSegment line = _UICamera->cameraFrustum.UnProjectLineSegment(positionX, positionY);
		GameObject* hitGameObject = Application::Instance()->renderer3D->RaycastFromMousePosition(line, _UICamera);
		LayerEditor::S_SetSelectGameObject(hitGameObject);
	}
}

void ImWindowUI::DetectImGuizmoInput()
{
	if (ModuleInput::S_GetKey(SDL_SCANCODE_W) == KEY_DOWN)
	{
		_imOperation = ImGuizmo::OPERATION::TRANSLATE;
	}
	else if (ModuleInput::S_GetKey(SDL_SCANCODE_E) == KEY_DOWN)
	{
		_imOperation = ImGuizmo::OPERATION::ROTATE;
	}
	else if (ModuleInput::S_GetKey(SDL_SCANCODE_R) == KEY_DOWN)
	{
		_imOperation = ImGuizmo::OPERATION::SCALE;
	}
}