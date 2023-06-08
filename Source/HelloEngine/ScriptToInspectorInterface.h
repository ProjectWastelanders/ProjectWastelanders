#pragma once
#include "Globals.h"
#include <string>
#include <vector>
#include "API/API_GameObject.h"
#include "API/API_String.h"

#ifndef HELLO_ENGINE_EXPORTS
#include "json.hpp"
using json = nlohmann::json;
#endif

class ScriptInspectorField
{
public:
	ScriptInspectorField();
	void* value = nullptr;
	std::string valueName = "";
	std::string className = "";
	uint UID = 0;
	virtual void OnEditor() = 0;
#ifndef HELLO_ENGINE_EXPORTS
	virtual void OnSerialize(json& j) = 0;
	virtual void OnDeserialize(json& j) = 0;
#endif
};

class DragFieldFloat : public ScriptInspectorField
{
	void OnEditor() override;
#ifndef HELLO_ENGINE_EXPORTS
	void OnSerialize(json& j) override;
	void OnDeserialize(json& j) override;
#endif
};

class DragFieldInt : public ScriptInspectorField
{
	void OnEditor() override;
#ifndef HELLO_ENGINE_EXPORTS
	void OnSerialize(json& j) override;
	void OnDeserialize(json& j) override;
#endif
};

class CheckBoxField : public ScriptInspectorField
{
	void OnEditor() override;
#ifndef HELLO_ENGINE_EXPORTS
	void OnSerialize(json& j) override;
	void OnDeserialize(json& j) override;
#endif
};

class InputBoxField : public ScriptInspectorField
{
	void OnEditor() override;
#ifndef HELLO_ENGINE_EXPORTS
	void OnSerialize(json& j) override;
	void OnDeserialize(json& j) override;
#endif
};

class DragBoxGameObject : public ScriptInspectorField
{
	void OnEditor() override;
#ifndef HELLO_ENGINE_EXPORTS
	void OnSerialize(json& j) override;
	void OnDeserialize(json& j) override;
#endif
};

class DragBoxTransform : public ScriptInspectorField
{
	void OnEditor() override;
#ifndef HELLO_ENGINE_EXPORTS
	void OnSerialize(json& j) override;
	void OnDeserialize(json& j) override;
#endif
};

class DragBoxMeshRenderer : public ScriptInspectorField
{
	void OnEditor() override;
#ifndef HELLO_ENGINE_EXPORTS
	void OnSerialize(json& j) override;
	void OnDeserialize(json& j) override;
#endif
};

class DragBoxCamera : public ScriptInspectorField
{
	void OnEditor() override;
#ifndef HELLO_ENGINE_EXPORTS
	void OnSerialize(json& j) override;
	void OnDeserialize(json& j) override;
#endif
};

class DragBoxRigidBody : public ScriptInspectorField
{
	void OnEditor() override;
#ifndef HELLO_ENGINE_EXPORTS
	void OnSerialize(json& j) override;
	void OnDeserialize(json& j) override;
#endif
};

class DragBoxAnimationPlayer : public ScriptInspectorField
{
	void OnEditor() override;
#ifndef HELLO_ENGINE_EXPORTS
	void OnSerialize(json& j) override;
	void OnDeserialize(json& j) override;
#endif
};

class DragBoxAnimationResource : public ScriptInspectorField
{
	void OnEditor() override;
#ifndef HELLO_ENGINE_EXPORTS
	void OnSerialize(json& j) override;
	void OnDeserialize(json& j) override;
#endif
};

class DragBoxMeshResource : public ScriptInspectorField
{
	void OnEditor() override;
#ifndef HELLO_ENGINE_EXPORTS
	void OnSerialize(json& j) override;
	void OnDeserialize(json& j) override;
#endif
};

class DragBoxTextureResource : public ScriptInspectorField
{
	void OnEditor() override;
#ifndef HELLO_ENGINE_EXPORTS
	void OnSerialize(json& j) override;
	void OnDeserialize(json& j) override;
#endif
};

class DragBoxMaterialComponent : public ScriptInspectorField
{
	void OnEditor() override;
#ifndef HELLO_ENGINE_EXPORTS
	void OnSerialize(json& j) override;
	void OnDeserialize(json& j) override;
#endif
};

class DragBoxParticleSystem : public ScriptInspectorField
{
	void OnEditor() override;
#ifndef HELLO_ENGINE_EXPORTS
	void OnSerialize(json& j) override;
	void OnDeserialize(json& j) override;
#endif
};

class DragBoxUIButton : public ScriptInspectorField
{
	void OnEditor() override;
#ifndef HELLO_ENGINE_EXPORTS
	void OnSerialize(json& j) override;
	void OnDeserialize(json& j) override;
#endif
};

class DragBoxUIImage : public ScriptInspectorField
{
	void OnEditor() override;
#ifndef HELLO_ENGINE_EXPORTS
	void OnSerialize(json& j) override;
	void OnDeserialize(json& j) override;
#endif
};

class DragBoxUIInput : public ScriptInspectorField
{
	void OnEditor() override;
#ifndef HELLO_ENGINE_EXPORTS
	void OnSerialize(json& j) override;
	void OnDeserialize(json& j) override;
#endif
};

class DragBoxUIText : public ScriptInspectorField
{
	void OnEditor() override;
#ifndef HELLO_ENGINE_EXPORTS
	void OnSerialize(json& j) override;
	void OnDeserialize(json& j) override;
#endif
};

class DragBoxUICheckBox : public ScriptInspectorField
{
	void OnEditor() override;
#ifndef HELLO_ENGINE_EXPORTS
	void OnSerialize(json& j) override;
	void OnDeserialize(json& j) override;
#endif
};

class DragBoxUISlider : public ScriptInspectorField
{
	void OnEditor() override;
#ifndef HELLO_ENGINE_EXPORTS
	void OnSerialize(json& j) override;
	void OnDeserialize(json& j) override;
#endif
};

class DragBoxPrefabResource : public ScriptInspectorField
{
	void OnEditor() override;
#ifndef HELLO_ENGINE_EXPORTS
	void OnSerialize(json& j) override;
	void OnDeserialize(json& j) override;
#endif
};

class DragBoxShaderComponent : public ScriptInspectorField
{
	void OnEditor() override;
#ifndef HELLO_ENGINE_EXPORTS
	void OnSerialize(json& j) override;
	void OnDeserialize(json& j) override;
#endif
};

class DragBoxAudioSourceComponent : public ScriptInspectorField
{
	void OnEditor() override;
#ifndef HELLO_ENGINE_EXPORTS
	void OnSerialize(json& j) override;
	void OnDeserialize(json& j) override;
#endif
};

class DragBoxVideoPlayerComponent : public ScriptInspectorField
{
	void OnEditor() override;
#ifndef HELLO_ENGINE_EXPORTS
	void OnSerialize(json& j) override;
	void OnDeserialize(json& j) override;
#endif
};

class TO_API ScriptToInspectorInterface
{
public:
	virtual void AddDragFloat(const char* name, float* value) = 0;
	virtual void AddDragInt(const char* name, int* value) = 0;
	virtual void AddCheckBox(const char* name, bool* value) = 0;
	virtual void AddInputBox(const char* name, API::API_String* value) = 0;
	virtual void AddDragBoxGameObject(const char* name, API::API_GameObject* value) = 0;
	virtual void AddDragBoxTransform(const char* name, API::API_Transform* value) = 0;
	virtual void AddDragBoxMeshRenderer(const char* name, API::API_MeshRenderer* value) = 0;
	virtual void AddDragBoxCamera(const char* name, API::API_Camera* value) = 0;
	virtual void AddDragBoxRigidBody(const char* name, API::API_RigidBody* value) = 0;
	virtual void AddDragBoxAnimationPlayer(const char* name, API::API_AnimationPlayer* value) = 0;
	virtual void AddDragBoxAnimationResource(const char* name, uint* value) = 0;
	virtual void AddDragBoxMeshResource(const char* name, uint* value) = 0;
	virtual void AddDragBoxTextureResource(const char* name, uint* value) = 0;
	virtual void AddDragBoxMaterialComponent(const char* name, API::API_Material* value) = 0;
	virtual void AddDragBoxParticleSystem(const char* name, API::API_ParticleSystem* value) = 0;
	virtual void AddDragBoxUIButton(const char* name, API::API_UIButton* value) = 0;
	virtual void AddDragBoxUICheckBox(const char* name, API::API_UICheckBox* value) = 0;
	virtual void AddDragBoxUISlider(const char* name, API::API_UISlider* value) = 0;
	virtual void AddDragBoxUIImage(const char* name, API::API_UIImage* value) = 0;
	virtual void AddDragBoxUIInput(const char* name, API::API_UIInput* value) = 0;
	virtual void AddDragBoxUIText(const char* name, API::API_UIText* value) = 0;
	virtual void AddDragBoxPrefabResource(const char* name, uint* value) = 0;
	virtual void AddDragBoxShaderComponent(const char* name, API::API_ShaderComponent* value) = 0;
	virtual void AddDragBoxAudioSourceComponent(const char* name, API::API_AudioSourceComponent* value) = 0;
	virtual void AddDragBoxVideoPlayerComponent(const char* name, API::API_VideoPlayer* value) = 0;


protected:
	std::vector<ScriptInspectorField*> inspectorFields;
};
