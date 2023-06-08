#pragma once
#include "Component.h"
#include "ModuleResourceManager.h"
#include "ScriptToInspectorInterface.h"

struct PhysBody3D;

class ScriptComponent : public Component, public ScriptToInspectorInterface
{
public:
	ScriptComponent(GameObject* go);
	~ScriptComponent();

#ifdef STANDALONE
	void OnEditor() override;

	void MarkAsDead() override;
	void MarkAsAlive() override;
#endif
	void Serialization(json& j) override;
	void DeSerialization(json& j) override;

	void AddScript(std::string scriptName);

	const std::string& GetScriptName();
	HelloBehavior* GetScript();

	// Add script variables to inspector methods:
	void AddDragFloat(const char* name, float* value) override;
	void AddDragInt(const char* name, int* value) override;
	void AddCheckBox(const char* name, bool* value) override;
	void AddInputBox(const char* name, API::API_String* value) override;
	void AddDragBoxGameObject(const char* name, API::API_GameObject* value) override;
	void AddDragBoxTransform(const char* name, API::API_Transform* value) override;
	void AddDragBoxMeshRenderer(const char* name, API::API_MeshRenderer* value) override;
	void AddDragBoxCamera(const char* name, API::API_Camera* value) override;
	void AddDragBoxRigidBody(const char* name, API::API_RigidBody* value) override;
	void AddDragBoxAnimationPlayer(const char* name, API::API_AnimationPlayer* value) override;
	void AddDragBoxAnimationResource(const char* name, uint* value) override;
	void AddDragBoxMeshResource(const char* name, uint* value) override;
	void AddDragBoxTextureResource(const char* name, uint* value) override;
	void AddDragBoxMaterialComponent(const char* name, API::API_Material* value) override;
	void AddDragBoxParticleSystem(const char* name, API::API_ParticleSystem* value);
	void AddDragBoxUIButton(const char* name, API::API_UIButton* value) override;
	void AddDragBoxUICheckBox(const char* name, API::API_UICheckBox* value) override;
	void AddDragBoxUISlider(const char* name, API::API_UISlider* value) override;
	void AddDragBoxUIImage(const char* name, API::API_UIImage* value) override;
	void AddDragBoxUIInput(const char* name, API::API_UIInput* value) override;
	void AddDragBoxUIText(const char* name, API::API_UIText* value) override;
	void AddDragBoxPrefabResource(const char* name, uint* value) override;
	void AddDragBoxShaderComponent(const char* name, API::API_ShaderComponent* value) override;
	void AddDragBoxAudioSourceComponent(const char* name, API::API_AudioSourceComponent* value) override;
	void AddDragBoxVideoPlayerComponent(const char* name, API::API_VideoPlayer* value) override;

	uint GetResourceUID();
	void DestroyedResource();

	void OnEnable() override;
	void OnDisable() override;

	void OnCollisionEnter(PhysBody3D* other);
	void OnCollisionStay(PhysBody3D* other);
	void OnCollisionExit(PhysBody3D* other);

private:
	void ImGuiDragScript();
	void DestroyInspectorFields();
	void SaveInspectorFields(json* j = nullptr);
	void LoadInspectorFields(json* j = nullptr);

private:
	uint scriptUID = 0;
	uint uniqueUID = 0;
	std::string headerName = "";
	ResourceScript* scriptResource = nullptr;
	json inspectorFieldsJSON;
	std::string addedScript = "None"; // Name of a class that has been added using AddScript() method of API_Gameobject. Not attached to a Resource.

	friend class LayerGame;
};
