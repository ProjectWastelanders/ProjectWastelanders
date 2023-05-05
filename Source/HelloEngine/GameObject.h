#pragma once
#include "Component.h"
#include "TransformComponent.h"

#define COMPONENT_NUM 13

struct PhysBody3D;

class GameObject
{
public:
	GameObject(GameObject* parent, std::string name = "Default", std::string tag = "Default", uint ID = 0);
	GameObject(GameObject* parent, std::string& name, std::string& tag, uint ID = 0);
	~GameObject();

	template <class T>
	T* AddComponent()
	{
		T* newComponent = new T(this);
		_components.push_back(newComponent);

		return newComponent;
	}

	template <class T>
	T* AddComponentCopied(const T& copy)
	{
		T* newComponent = new T(this, copy);
		_components.push_back(newComponent);

		return newComponent;
	}

	Component* AddComponentOfType(Component::Type type);
	Component* AddComponentOfType(Component::Type type, const Component& copy);
	void AddComponentSerialized(Component::Type type, json& jsonFile);

	template<class T>
	T* GetComponent(uint index = 0)
	{
		uint counter = 0;
		for (const auto& component : _components)
		{
			if (component == nullptr)
				continue;

			if (typeid(*component) != typeid(T)) continue;

			if (counter == index) return (T*)component;
			else counter++;
		}
		return nullptr;
	}

	std::vector<Component*> GetComponents() { return _components; }

	void DestroyComponent(Component::Type type);
	void DestroyComponent(Component* component);

	GameObject* GetParent() { return _parent; }

	bool AddChild(GameObject* child);

	// Only to be called when a GameObject is destroyed! no Game Object should be without a parent.
	void RemoveChild(GameObject* child);

	bool SetParent(GameObject* parent);

	void SetActive(bool active);

	bool IsActive() { return _isActive; }

	bool IsPendingToDelete() { return _isPendingToDelete; }

	bool IsStatic() { return _isStatic; }

	void SetIsStatic(bool _isStatic);

	void SetChildrenStatic(bool childStatic);

	std::vector<GameObject*>* GetChildren() { return &_children; }

	std::vector<GameObject*>* GetAllChildren();

	uint GetID() { return _ID; }

	std::string GetName() { return name; }

	std::string GetTag() { return tag; }

	uint GetPrefabUID() { return _prefabUID; }
	void SetPrefabUID(uint prefabUID) { _prefabUID = prefabUID; }
	void SetAllChildsPrefabUID(uint prefabUID);

	void OnCollisionEnter(PhysBody3D* other);
	void OnCollisionStay(PhysBody3D* other);
	void OnCollisionExit(PhysBody3D* other);

	bool GetBonesUpdate() { return _updateTransformWithBones; }
	void SetUpdateWithBones(bool active) { _updateTransformWithBones = active; }

#ifdef STANDALONE
	void OnEditor();
	bool MarkAsDead();

	bool MarkAsAlive();
#endif
	void Destroy();

	template <class T>
	bool HasComponent()
	{
		for (const auto& component : _components)
		{
			if (component == nullptr)
				continue;

			if (typeid(*component) == typeid(T))
				return true;
		}
		return false;
	}

	bool HasComponent(Component::Type type);

public:
	std::string name = "";
	std::string tag = "";
	TransformComponent* transform = nullptr;

	bool isSelected = false;

private:
	std::vector<Component*> _components;
	std::vector<GameObject*> _children;

	GameObject* _parent = nullptr;

	bool _isActive = true;
	bool _isDestroyed = false;
	bool _isStatic = false;

	uint _ID = 0; // ID =  0 is an invalid ID. First ID is 1.

	uint _prefabUID;
	bool _updatePrefab;

	bool _updateTransformWithBones = false;

	// On Editor variables
	std::string _comboValues[COMPONENT_NUM] = { "Mesh Renderer", "Texture", "Camera", 
        "Script", "Physics", "Particle System", "Skinned Mesh Renderer", "Animation Player", 
        "Material", "Directional Light", "Point Light", "Spot Light", "Navmesh Agent" };
	
    bool _isPendingToDelete = false;
    std::vector<int> _childrenDeletedIndex;
    friend class TransformComponent;
    friend class ModuleRenderer3D;
    friend class ModuleResourceManager;
    friend class CommandSetParentGameObject;
    friend class ImWindowHierarchy;
    friend class ImWindowInspector;

};