#include "Headers.h"
#include "ImWindowInspector.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "MeshRenderComponent.h"
#include "LayerEditor.h"

ImWindowInspector::ImWindowInspector()
{
	windowName = "Inspector";

	isEnabled = true;
}

ImWindowInspector::~ImWindowInspector()
{
}

void ImWindowInspector::Update()
{
	if (ImGui::Begin(windowName.c_str(), &isEnabled))
	{
		if (ImGui::IsWindowFocused())
			LayerEditor::detectInput = false;

		if (selectGameobject && selectGameobject->_parent)
		{
			if (ImGui::SmallButton(selectGameobject->IsActive() ? "X" : " "))
				selectGameobject->SetActive(!selectGameobject->IsActive());
			
			ImGui::SameLine();

			ImGui::InputText("Name", &selectGameobject->name);
			ImGui::InputText("Tag", &selectGameobject->tag);

			bool isStatic = selectGameobject->IsStatic();

			if (ImGui::Checkbox("Is Static", &isStatic))
			{
				selectGameobject->SetIsStatic(isStatic);

				if (!selectGameobject->_children.empty())
				{
					if (isStatic)
						ImGui::OpenPopup("Set Children Static?");
					else
						ImGui::OpenPopup("Set Children non Static?");
				}
			}

			if (ImGui::BeginPopupModal("Set Children Static?", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings)) 
			{
				ImGui::Text("Do you want to set all children to Static?");

				if (ImGui::Button("YES"))
				{
					selectGameobject->SetChildrenStatic(true);
					ImGui::CloseCurrentPopup();
				}
				ImGui::SameLine();

				ImGui::SameLine();
				if (ImGui::Button("NO"))
				{
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
			else if (ImGui::BeginPopupModal("Set Children non Static?", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings)) 
			{
				ImGui::Text("Do you want to set all children to non Static?");
				if (ImGui::Button("YES"))
				{
					selectGameobject->SetChildrenStatic(false);
					ImGui::CloseCurrentPopup();
				}
				ImGui::SameLine();
				if (ImGui::Button("NO"))
				{
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}

#ifdef STANDALONE
			selectGameobject->OnEditor();
#endif // STANDALONE
		}
	}
	ImGui::End();
}

void ImWindowInspector::SelectGameObject(GameObject* g)
{
	selectGameobject = g;
}