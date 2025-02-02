#pragma once

#include "json.hpp"

class GameObject;
using json = nlohmann::json;

class Component
{
public:
	enum class Type
	{
		NONE,
		TRANSFORM,
		MESH_RENDERER,
		TEXTURE,
		CAMERA,
		SCRIPT,
		PARTICLE_SYSTEM,
		BILLBOARD,
		PHYSICS,
		SKINNING,
		ANIMATION_PLAYER,
		UI,
		UI_BUTTON,
		UI_SLIDER,
		UI_CHECKBOX,
		UI_IMAGE,
		UI_INPUT,
		UI_TEXT,
		MATERIAL,
		LIGHT,
		DIRECTIONAL_LIGHT,
		POINT_LIGHT,
		SPOT_LIGHT,
		AGENT,
		UI_VIDEO,
		AUDIO_SOURCE,
		MAX
	};

	Component(GameObject* gameObject);
	virtual ~Component() { std::cout << "Destroy Component" << std::endl; }

	GameObject* GetGameObject() { return _gameObject; }

	//template<class T>
	//T* GetComponent(uint index = 0);

	//template<class T>
	//bool TryGetComponent(T* result, uint index = 0);

	Type GetType() { return _type; }
	bool IsEnabled(){ return _isEnabled; }

	bool NeedsTransformCallback() { return _needsTransformCallback; }

	virtual void OnTransformCallback(float4x4 worldMatrix) {};

	void Enable();
	void Disable();

	virtual void OnEnable();
	virtual void OnDisable();

	virtual void OnPositionUpdate(float3 pos) {}
	virtual void OnScaleUpdate(float3 scale) {}
	virtual void OnRotationUpdate(float3 rotation) {}
	virtual void OnTransformUpdate(float3 pos, float3 scale, float3 rotation) {}

	virtual void Serialization(json& j) = 0;
	virtual void DeSerialization(json& j) = 0;

#ifdef STANDALONE
	virtual void OnEditor() {}

	virtual void MarkAsDead() {}
	virtual void MarkAsAlive() {}
#endif
	Type _type = Type::NONE;

private:
	void EnableFromGameObject() { if (_isEnabled) OnEnable(); };
	void DisableFromGameObject() { if (_isEnabled) OnDisable(); };

	friend class GameObject;

protected:
	bool _isEnabled = true;
	bool _needsTransformCallback = false;

	GameObject* _gameObject = nullptr;
	
	friend class ModuleNavMesh;
	friend class Pathfinder;
};
