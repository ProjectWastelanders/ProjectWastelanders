#include "Headers.h"
#include "ScriptComponent.h"
#include "ModuleLayers.h"
#include "LayerGame.h"
#include "LayerEditor.h"

ScriptComponent::ScriptComponent(GameObject* go) : Component(go)
{
	Application::Instance()->layers->game->AddScriptComponent(this);
}

ScriptComponent::~ScriptComponent()
{
	Application::Instance()->layers->game->RemoveScriptComponent(this);
}

void ScriptComponent::OnEditor()
{
	if (ImGui::CollapsingHeader("Script", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (scriptResource == nullptr)
		{
			ImGui::TextColored(ImVec4(1,1,0,1), "Drag a .cpp or an .h file with a correct HelloBehavior children class.");
			ImGuiDragScript();
		}
		else
		{
			bool auxiliaryBool = _isEnabled;
			if (ImGui::Checkbox("Active##Script", &auxiliaryBool))
				auxiliaryBool ? Enable() : Disable();

			ImGui::TextWrapped("Loaded script: "); ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), scriptResource->className.c_str());
			ImGuiDragScript();
		}
	}
}

void ScriptComponent::Serialization(json& j)
{
}

void ScriptComponent::DeSerialization(json& j)
{
}

void ScriptComponent::ImGuiDragScript()
{
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Script"))
		{
			const uint* drop = (uint*)payload->Data;

			scriptResource = (ResourceScript*)Application::Instance()->resource->S_LoadResource(*drop);

			std::string popUpmessage = "Loaded Script: " + scriptResource->className;
			Application::Instance()->layers->editor->AddPopUpMessage(popUpmessage);

		}
		ImGui::EndDragDropTarget();
	}

}
