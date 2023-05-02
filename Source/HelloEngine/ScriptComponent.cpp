#include "Headers.h"
#include "ScriptComponent.h"
#include "ModuleLayers.h"
#include "LayerGame.h"
#include "LayerEditor.h"
#include "ModulePhysics.h"
#include "PhysBody3D.h"
#include "PhysicsComponent.h"

ScriptComponent::ScriptComponent(GameObject* go) : Component(go)
{
	_type = Component::Type::SCRIPT;
	LayerGame::S_AddScriptComponent(this);
	uniqueUID = HelloUUID::GenerateUUID();
	headerName = "Script#" + std::to_string(uniqueUID);
}

ScriptComponent::~ScriptComponent()
{
	LayerGame::S_RemoveScriptComponent(this);
	if (scriptResource != nullptr)
		scriptResource->Dereference();
	DestroyInspectorFields();
}

#ifdef STANDALONE

void ScriptComponent::OnEditor()
{
	bool created = true;
	if (ImGui::CollapsingHeader(headerName.c_str(), &created, ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_SpanAvailWidth))
	{
		if (scriptResource == nullptr && addedScript == "None")
		{
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "Drag a .cpp or an .h file with a correct HelloBehavior children class.");
			ImGuiDragScript();
		}
		else
		{
			bool auxiliaryBool = _isEnabled;
			if (ImGui::Checkbox(("Active##Script" + std::to_string(scriptUID)).c_str(), &auxiliaryBool))
				auxiliaryBool ? Enable() : Disable();

			ImGui::TextWrapped("Loaded script: "); ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), scriptResource == nullptr ? addedScript.c_str() : scriptResource->className.c_str());
			ImGuiDragScript();
			// Show script inspector variables.
			for (int i = 0; i < inspectorFields.size(); ++i)
			{
				inspectorFields[i]->OnEditor();
			}
		}
	}
	if (!created)
		_gameObject->DestroyComponent(this);
}
#endif
void ScriptComponent::OnEnable()
{
	if (scriptUID != 0)
		LayerGame::_behaviorScripts[scriptUID].active = true;
}

void ScriptComponent::OnDisable()
{
	if (scriptUID != 0)
		LayerGame::_behaviorScripts[scriptUID].active = false;
}

void ScriptComponent::OnCollisionEnter(PhysBody3D* other)
{
	HelloBehavior* script = GetScript();
	if (script != nullptr)
	{
		GameObject* otherGO = ModuleLayers::gameObjects[other->gameObjectUID];
		PhysicsComponent* otherComponent = otherGO->GetComponent<PhysicsComponent>();
		API::API_RigidBody apiRB;
		apiRB.SetComponent(otherComponent);
		script->OnCollisionEnter(apiRB);
	}
}

void ScriptComponent::OnCollisionStay(PhysBody3D* other)
{
	HelloBehavior* script = GetScript();
	if (script != nullptr)
	{
		GameObject* otherGO = ModuleLayers::gameObjects[other->gameObjectUID];
		PhysicsComponent* otherComponent = otherGO->GetComponent<PhysicsComponent>();
		API::API_RigidBody apiRB;
		apiRB.SetComponent(otherComponent);
		script->OnCollisionStay(apiRB);
	}
}

void ScriptComponent::OnCollisionExit(PhysBody3D* other)
{
	HelloBehavior* script = GetScript();
	if (script != nullptr)
	{
		GameObject* otherGO = ModuleLayers::gameObjects[other->gameObjectUID];
		PhysicsComponent* otherComponent = otherGO->GetComponent<PhysicsComponent>();
		API::API_RigidBody apiRB;
		apiRB.SetComponent(otherComponent);
		script->OnCollisionExit(apiRB);
	}
}

void ScriptComponent::Serialization(json& j)
{
	json _j;

	_j["Type"] = _type;

	_j["Script Resource"] = scriptResource ? scriptResource->UID : 0;

	// Serialize every inspector field
	SaveInspectorFields(&_j);

	_j["Enabled"] = _isEnabled;

	j["Components"].push_back(_j);
}

void ScriptComponent::DeSerialization(json& j)
{
	scriptResource = (ResourceScript*)ModuleResourceManager::S_LoadResource(j["Script Resource"]);

	if (scriptResource)
	{
		// Create a new script object instance.
		LayerGame::S_CreateBehaviorScript(this);
		// Deserialize every inspector field
		LoadInspectorFields(&j);
	}

	bool enabled = j["Enabled"];
	if (!enabled)
		Disable();
}

void ScriptComponent::AddScript(std::string scriptName)
{
	addedScript = scriptName;// We need to do this before calling next funciton because the addedScript is needed by the Create function.

	// Create a new script object instance.
	if (!LayerGame::S_CreateBehaviorScriptByName(scriptName, this))
	{
		addedScript = "None";
		return;
	}
}

const std::string& ScriptComponent::GetScriptName()
{
	if (scriptResource != nullptr)
		return scriptResource->className;
	return addedScript;
}

HelloBehavior* ScriptComponent::GetScript()
{
	if (scriptResource == nullptr && addedScript == "None")
		return nullptr;

	return LayerGame::_behaviorScripts[scriptUID].script;
}

void ScriptComponent::AddDragFloat(const char* name, float* value)
{
	DragFieldFloat* dragField = new DragFieldFloat();
	dragField->valueName = name;
	dragField->value = value;
	dragField->className = scriptResource == nullptr ? addedScript : scriptResource->className;

	inspectorFields.push_back(dragField);
}

void ScriptComponent::AddDragInt(const char* name, int* value)
{
	DragFieldInt* dragField = new DragFieldInt();
	dragField->valueName = name;
	dragField->value = value;
	dragField->className = scriptResource == nullptr ? addedScript : scriptResource->className;

	inspectorFields.push_back(dragField);
}

void ScriptComponent::AddCheckBox(const char* name, bool* value)
{
	CheckBoxField* checkBoxField = new CheckBoxField();
	checkBoxField->valueName = name;
	checkBoxField->value = value;
	checkBoxField->className = scriptResource == nullptr ? addedScript : scriptResource->className;

	inspectorFields.push_back(checkBoxField);
}

void ScriptComponent::AddInputBox(const char* name, API::API_String* value)
{
	InputBoxField* inputBoxField = new InputBoxField();
	inputBoxField->valueName = name;
	inputBoxField->value = value;
	inputBoxField->className = scriptResource == nullptr ? addedScript : scriptResource->className;

	inspectorFields.push_back(inputBoxField);
}

void ScriptComponent::AddDragBoxGameObject(const char* name, API::API_GameObject* value)
{
	DragBoxGameObject* dragBoxField = new DragBoxGameObject();
	dragBoxField->valueName = name;
	dragBoxField->value = value;
	dragBoxField->className = scriptResource == nullptr ? addedScript : scriptResource->className;

	inspectorFields.push_back(dragBoxField);
}

void ScriptComponent::AddDragBoxTransform(const char* name, API::API_Transform* value)
{
	DragBoxTransform* dragBoxField = new DragBoxTransform();
	dragBoxField->valueName = name;
	dragBoxField->value = value;
	dragBoxField->className = scriptResource == nullptr ? addedScript : scriptResource->className;

	inspectorFields.push_back(dragBoxField);
}

void ScriptComponent::AddDragBoxMeshRenderer(const char* name, API::API_MeshRenderer* value)
{
	DragBoxMeshRenderer* dragBoxField = new DragBoxMeshRenderer();
	dragBoxField->valueName = name;
	dragBoxField->value = value;
	dragBoxField->className = scriptResource == nullptr ? addedScript : scriptResource->className;

	inspectorFields.push_back(dragBoxField);
}

void ScriptComponent::AddDragBoxCamera(const char* name, API::API_Camera* value)
{
	DragBoxCamera* dragBoxField = new DragBoxCamera();
	dragBoxField->valueName = name;
	dragBoxField->value = value;
	dragBoxField->className = scriptResource == nullptr ? addedScript : scriptResource->className;

	inspectorFields.push_back(dragBoxField);
}

void ScriptComponent::AddDragBoxRigidBody(const char* name, API::API_RigidBody* value)
{
	DragBoxRigidBody* dragBoxField = new DragBoxRigidBody();
	dragBoxField->valueName = name;
	dragBoxField->value = value;
	dragBoxField->className = scriptResource == nullptr ? addedScript : scriptResource->className;

	inspectorFields.push_back(dragBoxField);
}

void ScriptComponent::AddDragBoxAnimationPlayer(const char* name, API::API_AnimationPlayer* value)
{
	DragBoxAnimationPlayer* dragBoxField = new DragBoxAnimationPlayer();
	dragBoxField->valueName = name;
	dragBoxField->value = value;
	dragBoxField->className = scriptResource == nullptr ? addedScript : scriptResource->className;

	inspectorFields.push_back(dragBoxField);
}

void ScriptComponent::AddDragBoxAnimationResource(const char* name, uint* value)
{
	DragBoxAnimationResource* dragBoxField = new DragBoxAnimationResource();
	dragBoxField->valueName = name;
	dragBoxField->value = value;
	dragBoxField->className = scriptResource == nullptr ? addedScript : scriptResource->className;

	inspectorFields.push_back(dragBoxField);
}

void ScriptComponent::AddDragBoxMeshResource(const char* name, uint* value)
{
	DragBoxMeshResource* dragBoxField = new DragBoxMeshResource();
	dragBoxField->valueName = name;
	dragBoxField->value = value;
	dragBoxField->className = scriptResource == nullptr ? addedScript : scriptResource->className;

	inspectorFields.push_back(dragBoxField);
}

void ScriptComponent::AddDragBoxTextureResource(const char* name, uint* value)
{
	DragBoxTextureResource* dragBoxField = new DragBoxTextureResource();
	dragBoxField->valueName = name;
	dragBoxField->value = value;
	dragBoxField->className = scriptResource == nullptr ? addedScript : scriptResource->className;

	inspectorFields.push_back(dragBoxField);
}

void ScriptComponent::AddDragBoxMaterialComponent(const char* name, API::API_Material* value)
{
	DragBoxMaterialComponent* dragBoxField = new DragBoxMaterialComponent();
	dragBoxField->valueName = name;
	dragBoxField->value = value;
	dragBoxField->className = scriptResource == nullptr ? addedScript : scriptResource->className;

	inspectorFields.push_back(dragBoxField);
}

void ScriptComponent::AddDragBoxParticleSystem(const char* name, API::API_ParticleSystem* value)
{
    DragBoxParticleSystem* dragBoxField = new DragBoxParticleSystem();
    dragBoxField->valueName = name;
    dragBoxField->value = value;
    dragBoxField->className = scriptResource == nullptr ? addedScript : scriptResource->className;

    inspectorFields.push_back(dragBoxField);
}

void ScriptComponent::AddDragBoxUIButton(const char* name, API::API_UIButton* value)
{
	DragBoxUIButton* dragBoxField = new DragBoxUIButton();
	dragBoxField->valueName = name;
	dragBoxField->value = value;
	dragBoxField->className = scriptResource == nullptr ? addedScript : scriptResource->className;

	inspectorFields.push_back(dragBoxField);
}

void ScriptComponent::AddDragBoxUIImage(const char* name, API::API_UIImage* value)
{
	DragBoxUIImage* dragBoxField = new DragBoxUIImage();
	dragBoxField->valueName = name;
	dragBoxField->value = value;
	dragBoxField->className = scriptResource == nullptr ? addedScript : scriptResource->className;

	inspectorFields.push_back(dragBoxField);
}

void ScriptComponent::AddDragBoxUIInput(const char* name, API::API_UIInput* value)
{
	DragBoxUIInput* dragBoxField = new DragBoxUIInput();
	dragBoxField->valueName = name;
	dragBoxField->value = value;
	dragBoxField->className = scriptResource == nullptr ? addedScript : scriptResource->className;

	inspectorFields.push_back(dragBoxField);
}

void ScriptComponent::AddDragBoxUIText(const char* name, API::API_UIText* value)
{
	DragBoxUIText* dragBoxField = new DragBoxUIText();
	dragBoxField->valueName = name;
	dragBoxField->value = value;
	dragBoxField->className = scriptResource == nullptr ? addedScript : scriptResource->className;

	inspectorFields.push_back(dragBoxField);
}

void ScriptComponent::AddDragBoxUICheckBox(const char* name, API::API_UICheckBox* value)
{
	DragBoxUICheckBox* dragBoxField = new DragBoxUICheckBox();
	dragBoxField->valueName = name;
	dragBoxField->value = value;
	dragBoxField->className = scriptResource == nullptr ? addedScript : scriptResource->className;

	inspectorFields.push_back(dragBoxField);
}

void ScriptComponent::AddDragBoxPrefabResource(const char* name, uint* value)
{
	DragBoxPrefabResource* dragBoxField = new DragBoxPrefabResource();
	dragBoxField->valueName = name;
	dragBoxField->value = value;
	dragBoxField->className = scriptResource == nullptr ? addedScript : scriptResource->className;

	inspectorFields.push_back(dragBoxField);
}

void ScriptComponent::AddDragBoxShaderComponent(const char* name, API::API_ShaderComponent* value)
{
	DragBoxShaderComponent* dragBoxField = new DragBoxShaderComponent();
	dragBoxField->valueName = name;
	dragBoxField->value = value;
	dragBoxField->className = scriptResource == nullptr ? addedScript : scriptResource->className;

	inspectorFields.push_back(dragBoxField);
}

uint ScriptComponent::GetResourceUID()
{
	if (scriptResource != nullptr)
		return scriptResource->UID;
	return 0;
}

void ScriptComponent::DestroyedResource()
{
	if (scriptResource != nullptr)
	{
		LayerGame::S_DestroyBehaviorScript(this);
		scriptResource = nullptr;
	}
}

void ScriptComponent::ImGuiDragScript()
{
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Script"))
		{
			if (!LayerGame::S_IsCreatingBehaviorsEnabled())
			{
				LayerEditor::S_AddPopUpMessage("Cannot add Script file! Must compile DLL Solution first.");
				return;
			}
			const uint* drop = (uint*)payload->Data;

			// If we had another script attached, destroy that script instance first
			if (scriptUID != 0)
			{
				LayerGame::S_DestroyBehaviorScript(this);
				scriptResource->Dereference();
				DestroyInspectorFields();
			}

			scriptResource = (ResourceScript*)ModuleResourceManager::S_LoadResource(*drop);

			// Create a new script object instance.
			LayerGame::S_CreateBehaviorScript(this);

			std::string popUpmessage = "Loaded Script: " + scriptResource->className;
			LayerEditor::S_AddPopUpMessage(popUpmessage);
		}
		ImGui::EndDragDropTarget();
	}

}

void ScriptComponent::DestroyInspectorFields()
{
	for (int i = 0; i < inspectorFields.size(); ++i)
	{
		RELEASE(inspectorFields[i]);
	}
	inspectorFields.clear();
}

void ScriptComponent::SaveInspectorFields(json* j)
{
	// Serialize every inspector field
	for (int i = 0; i < inspectorFields.size(); ++i)
	{
		inspectorFields[i]->OnSerialize(inspectorFieldsJSON["Inspector Fields"]);
	}

	if (j != nullptr)
	{
		json& _j = *j;
		_j["Inspector Fields"] = inspectorFieldsJSON["Inspector Fields"];
	}
}

void ScriptComponent::LoadInspectorFields(json* j)
{
	if (j != nullptr)
	{
		for (int i = 0; i < inspectorFields.size(); ++i)
		{
			json& _j = *j;
			inspectorFields[i]->OnDeserialize(_j["Inspector Fields"]);
		}
	}
	else
	{
		for (int i = 0; i < inspectorFields.size(); ++i)
		{
			inspectorFields[i]->OnDeserialize(inspectorFieldsJSON["Inspector Fields"]);
		}
	}
}
#ifdef STANDALONE
void ScriptComponent::MarkAsDead()
{
	SaveInspectorFields();
	LayerGame::S_RemoveScriptComponent(this);
	if (scriptResource != nullptr)
		scriptResource->Dereference();
	DestroyInspectorFields();
}

void ScriptComponent::MarkAsAlive()
{
	LayerGame::S_AddScriptComponent(this);
	LoadInspectorFields();
}
#endif
