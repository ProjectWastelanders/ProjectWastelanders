#include "Headers.h"
#include "GameObject.h"
#include "ModuleLayers.h"
#include "TransformComponent.h"
#include "MeshRenderComponent.h"
#include "TextureComponent.h"
#include "MaterialComponent.h"
#include "CameraComponent.h"
#include "SkinnedMeshRenderComponent.h"
#include "LayerEditor.h"
#include "ImGuizmo/ImGuizmo.h"
#include "ScriptComponent.h"
#include "ComponentUI.h"
#include "ComponentUIButton.h"
#include "ComponentUISlider.h"
#include "ComponentUICheckbox.h"
#include "ComponentUIImage.h"
#include "AnimationComponent.h"
#include "MaterialComponent.h"
#include "ParticleSystemComponent.h"
#include "BillBoardComponent.h"
#include "PhysicsComponent.h"
#include "ComponentUIInput.h"
#include "TextRendererComponent.h"
#include "VideoPlayerComponent.h"
#include "DirectionalLightComponent.h"
#include "PointLightComponent.h"
#include "SpotLightComponent.h"
#include "NavAgentComponent.h"
#include "AudioSourceComponent.h"

GameObject::GameObject(GameObject* parent, std::string name, std::string tag, uint ID) : name(name), tag(tag)
{
	_ID = ModuleLayers::S_AddGameObject(this, ID);
	transform = AddComponent<TransformComponent>();
	if (parent != nullptr)
		parent->AddChild(this);

	_prefabUID = 0;
	_updatePrefab = true;
}

GameObject::GameObject(GameObject* parent, std::string& name, std::string& tag, uint ID) : name(name), tag(tag)
{
	_ID = ModuleLayers::S_AddGameObject(this, ID);
	transform = AddComponent<TransformComponent>();
	if (parent != nullptr)
		parent->AddChild(this);

	_prefabUID = 0;
	_updatePrefab = true;
}

GameObject::~GameObject()
{
	for (int i = 0; i < _components.size(); i++)
	{
		RELEASE(_components[i]);
	}
	

	for (int i = 0; i < _children.size(); i++)
	{
		RELEASE(_children[i]);
	}
	
}

void GameObject::DestroyComponent(Component::Type type)
{
	for (int i = 0; i < _components.size(); i++)
	{
		if (_components[i]->_type == type)
		{
			RELEASE(_components[i]);
			_components.erase(_components.begin() + i);
			break;
		}
	}
}

void GameObject::DestroyComponent(Component* component)
{
	if (component == nullptr)
		return;
	for (int i = 0; i < _components.size(); i++)
	{
		if (_components[i] == component)
		{
			Component* comp = _components[i];
			_components.erase(_components.begin() + i);
			RELEASE(comp);
			break;
		}
	}
}

bool GameObject::AddChild(GameObject* child)
{
	if (!child) return false;
	if (child->_parent == this) return false;

	GameObject* p = _parent;

	while (p)
	{
		if (p == child) return false;

		p = p->_parent;
	}

	_children.push_back(child);

	if (child->_parent)
		child->_parent->RemoveChild(child);

	child->_parent = this;

	transform->ForceUpdate();

	return true;
}

bool GameObject::SetParent(GameObject* parent)
{
	return parent->AddChild(this);
}

void GameObject::SetActive(bool active)
{
	if (_isActive == active)
		return;

	_isActive = active;

	for (auto* component : _components)
	{
		_isActive ? component->EnableFromGameObject() : component->DisableFromGameObject();
	}

	for (auto* child : _children)
	{
		child->SetActive(active);
	}

}

void GameObject::SetIsStatic(bool _isStatic)
{
	this->_isStatic = _isStatic;
}

void GameObject::SetChildrenStatic(bool childStatic)
{
	for (int i = 0; i < _children.size(); ++i)
	{
		_children[i]->SetIsStatic(childStatic);
		_children[i]->SetChildrenStatic(childStatic);
	}
}

std::vector<GameObject*>* GameObject::GetAllChildren()
{
	std::vector<GameObject*>* res = new std::vector<GameObject*>;

	res->insert(res->end(), _children.begin(), _children.end());

	for (int i = 0; i < _children.size(); i++)
	{
		std::vector<GameObject*>* ret = _children[i]->GetAllChildren();

		res->insert(res->end(), ret->begin(), ret->end());
	}

	return res;
}

void GameObject::OnCollisionEnter(PhysBody3D* other)
{
	for (int i = 0; i < _components.size(); ++i)
	{
		if (_components[i]->_type == Component::Type::SCRIPT)
		{
			// Callback to Scripting
			ScriptComponent* script = (ScriptComponent*)_components[i];
			script->OnCollisionEnter(other);
		}
	}
}

void GameObject::OnCollisionStay(PhysBody3D* other)
{
	for (int i = 0; i < _components.size(); ++i)
	{
		if (_components[i]->_type == Component::Type::SCRIPT)
		{
			// Callback to Scripting
			ScriptComponent* script = (ScriptComponent*)_components[i];
			script->OnCollisionStay(other);
		}
	}
}

void GameObject::OnCollisionExit(PhysBody3D* other)
{
	for (int i = 0; i < _components.size(); ++i)
	{
		if (_components[i]->_type == Component::Type::SCRIPT)
		{
			// Callback to Scripting
			ScriptComponent* script = (ScriptComponent*)_components[i];
			script->OnCollisionExit(other);
		}
	}
}

#ifdef STANDALONE

void GameObject::OnEditor()
{
    if (_isPendingToDelete) return;

	ImGui::Checkbox("Update with bones", &_updateTransformWithBones);

    for (auto* component : _components)
    {
        component->OnEditor();
    }

    ImGui::Separator();

    ImGui::Spacing();
    if (ImGui::BeginCombo("Add Component", "Select"))
    {
        for (int n = 0; n < COMPONENT_NUM; n++)
        {
            int selectedItem = n;
            if (ImGui::Selectable(_comboValues[n].c_str(), false))
            {
                switch (n)
                {
                case 0:
                    if (!HasComponent<MeshRenderComponent>())
                        AddComponent<MeshRenderComponent>();
                    break;
                case 1:
                    if (!HasComponent<TextureComponent>())
                        AddComponent<TextureComponent>();
                    break;
                case 2:
                    if (!HasComponent<CameraComponent>())
                        AddComponent<CameraComponent>();
                    break;
                case 3:
                    AddComponent<ScriptComponent>();
                    break;
                case 4:
                    if (!HasComponent<PhysicsComponent>())
                        AddComponent<PhysicsComponent>();
                    break;
                case 5:
                    if (!HasComponent<ParticleSystemComponent>())
                        AddComponent<ParticleSystemComponent>();
                    break;
                case 6:
                    if (!HasComponent<SkinnedMeshRenderComponent>())
                        AddComponent<SkinnedMeshRenderComponent>();
                    break;
                case 7:
                    if (!HasComponent<AnimationComponent>())
                        AddComponent<AnimationComponent>();
                    break;
                case 8:
                    if (!HasComponent<MaterialComponent>())
                        AddComponent<MaterialComponent>();
                    break;
                case 9:
                    if (!HasComponent<DirectionalLightComponent>())
                        AddComponent<DirectionalLightComponent>();
                    break;
                case 10:
                    if (!HasComponent<PointLightComponent>())
                        AddComponent<PointLightComponent>();
                    break;
                case 11:
                    if (!HasComponent<SpotLightComponent>())
                        AddComponent<SpotLightComponent>();
					break;
				case 12:
					if (!HasComponent<ComponentAgent>())
						AddComponent<ComponentAgent>();
					break;
				case 13:
					if (!HasComponent<AudioSourceComponent>())
						AddComponent<AudioSourceComponent>();
					break;
                }
            }
        }
        ImGui::EndCombo();
        }
    if (_prefabUID != 0)
    {
        ImGui::TextColored(ImVec4(1, 1, 1, 0.5f), "Prefab UID: "); ImGui::SameLine();
        ImGui::TextColored(ImVec4(1, 1, 1, 0.5f), std::to_string(_prefabUID).c_str());

        if (_parent->_prefabUID != 0) return;
        if (ImGui::Button("Override prefab"))
        {
            // Find prefab(asset) with _prefabUID and override it
            ResourcePrefab* aux = nullptr;
            if (ModuleResourceManager::resources.count(_prefabUID) != 0)
                aux = (ResourcePrefab*)ModuleResourceManager::resources[_prefabUID];

            if (aux != nullptr)
            {
                ModuleResourceManager::S_OverridePrefab(this, aux->path, _prefabUID);
            }
        }
        if (ImGui::Button("Revert prefab"))
        {
            // Find prefab(asset) with _prefabUID and override this one with it
            ResourcePrefab* aux = nullptr;
            if (ModuleResourceManager::resources.count(_prefabUID) != 0)
                aux = (ResourcePrefab*)ModuleResourceManager::resources[_prefabUID];

            if (aux != nullptr)
            {
                GameObject* newGameObject = ModuleResourceManager::S_DeserializeFromPrefab(aux->path, this->_parent);
                TransformComponent* t = this->GetComponent<TransformComponent>();
                newGameObject->GetComponent<TransformComponent>()->SetTransform(t->GetGlobalPosition(), t->GetGlobalScale(), t->GetLocalRotation());
                Destroy();
            }
        }
        if (ImGui::Button("Unpack from prefab"))
        {
            _prefabUID = 0;
        }
    }
}

bool GameObject::MarkAsDead()
{
	if (!_isPendingToDelete)
	{
		if (LayerEditor::selectedGameObject == this)
		{
			LayerEditor::S_SetSelectGameObject(nullptr);
			ImGuizmo::Enable(false);
		}

		_isPendingToDelete = true;

		for (int i = 0; i < _children.size(); i++)
		{
			if (_children[i]->MarkAsDead())
			{
				_childrenDeletedIndex.push_back(i);
			}
		}

		for (int i = 0; i < _components.size(); i++)
		{
			_components[i]->MarkAsDead();
		}

		return true;
	}

	return false;
}

bool GameObject::MarkAsAlive()
{
	if (_isPendingToDelete)
	{
		_isPendingToDelete = false;

		for (int i = 0; i < _childrenDeletedIndex.size(); i++)
		{
			_children[_childrenDeletedIndex[i]]->MarkAsAlive();
		}

		for (int i = 0; i < _components.size(); i++)
		{
			_components[i]->MarkAsAlive();
		}

		return true;
	}

	return false;
}

#endif
void GameObject::SetAllChildsPrefabUID(uint prefabUID)
{
	for (auto& go : _children)
	{
		go->SetPrefabUID(prefabUID);
		go->SetAllChildsPrefabUID(prefabUID);
	}
}

void GameObject::Destroy()
{
	if (_isDestroyed)
		return;

	if (LayerEditor::selectedGameObject == this)
	{
		LayerEditor::S_SetSelectGameObject(nullptr);
		ImGuizmo::Enable(false);
	}

	_isPendingToDelete = true;
	_isDestroyed = true;

	ModuleLayers::S_RemoveGameObject(_ID);

	ModuleLayers::_deletedGameObjects.push_back(this);

	if (_parent != nullptr)
		_parent->RemoveChild(this);

	while (!_children.empty())
	{
		_children[0]->Destroy();
	}

	// A bit of hardcoding. This is necessary so the reimport system form models (changing resources, saving and loading) works properly.
	// Basically, we need to nlink the meshRender resource from the component before destroying it, because when the components is deleted, the new scene has already been loaded,
	// and that produces the old component to unlink from the new resource, except from the old one.
	// TODO: Could fix this by searching for references at the moment of reimport.
	MeshRenderComponent* meshRender = GetComponent<MeshRenderComponent>();

	if (meshRender != nullptr)
		meshRender->UnlinkResource();

	_children.clear();
}

bool GameObject::HasComponent(Component::Type type)
{
	for (const auto& component : _components)
	{
		if (component == nullptr)
			continue;
		if (component->_type == type)
			return true;
	}
	return false;
}

void GameObject::RemoveChild(GameObject* child)
{
	if (!child)
		return;

	for (int i = 0; i < _children.size(); ++i)
	{
		if (_children[i] == child)
			_children.erase(_children.begin() + i);
	}
	child->_parent = nullptr;
}

Component* GameObject::AddComponentOfType(Component::Type type)
{
	Component* newComponent = nullptr;
	switch (type)
	{
	case Component::Type::TRANSFORM:
		return transform;
		break;
	case Component::Type::MESH_RENDERER:
		newComponent = new MeshRenderComponent(this);
		_components.push_back(newComponent);
		break;
	case Component::Type::SKINNING:
		newComponent = new SkinnedMeshRenderComponent(this);
		_components.push_back(newComponent);
		break;
	case Component::Type::TEXTURE:
		newComponent = new TextureComponent(this);
		_components.push_back(newComponent);
		break;
	case Component::Type::CAMERA:
		newComponent = new CameraComponent(this);
		_components.push_back(newComponent);
		break;
	case Component::Type::SCRIPT:
		newComponent = new ScriptComponent(this);
		_components.push_back(newComponent);
		break;
	case Component::Type::UI:
		newComponent = new ComponentUI(this);
		_components.push_back(newComponent);
		break;
	case Component::Type::UI_BUTTON:
		newComponent = new ComponentUIButton(this);
		_components.push_back(newComponent);
		break;
	case Component::Type::UI_SLIDER:
		newComponent = new ComponentUISlider(this);
		_components.push_back(newComponent);
		break;
	case Component::Type::UI_CHECKBOX:
		newComponent = new ComponentUICheckbox(this);
		_components.push_back(newComponent);
		break;
	case Component::Type::UI_IMAGE:
		newComponent = new ComponentUIImage(this);
		_components.push_back(newComponent);
		break;
	case Component::Type::PARTICLE_SYSTEM:
		newComponent = new ParticleSystemComponent(this);
		_components.push_back(newComponent);
		break;
	case Component::Type::BILLBOARD:
		newComponent = new BillBoardComponent(this);
		_components.push_back(newComponent);
		break;
	case Component::Type::PHYSICS:
		newComponent = new PhysicsComponent(this);
		_components.push_back(newComponent);
		break;
	case Component::Type::ANIMATION_PLAYER:
		newComponent = new AnimationComponent(this);
		_components.push_back(newComponent);
        break;
    case Component::Type::MATERIAL:
        newComponent = new MaterialComponent(this);
        _components.push_back(newComponent);
        break;
    case Component::Type::UI_INPUT:
        newComponent = new ComponentUIInput(this);
        _components.push_back(newComponent);
        break;
    case Component::Type::UI_TEXT:
        newComponent = new TextRendererComponent(this);
        _components.push_back(newComponent);
        break;
    case Component::Type::UI_VIDEO:
        newComponent = new VideoPlayerComponent(this);
        _components.push_back(newComponent);
        break;
    case Component::Type::DIRECTIONAL_LIGHT:
        newComponent = new DirectionalLightComponent(this);
        _components.push_back(newComponent);
        break;
    case Component::Type::POINT_LIGHT:
        newComponent = new PointLightComponent(this);
        _components.push_back(newComponent);
        break;
    case Component::Type::SPOT_LIGHT:
        newComponent = new SpotLightComponent(this);
        _components.push_back(newComponent);
        break;
	case Component::Type::AGENT:
		newComponent = new ComponentAgent(this);
		_components.push_back(newComponent);
		break;
	case Component::Type::AUDIO_SOURCE:
		newComponent = new AudioSourceComponent(this);
		_components.push_back(newComponent);
		break;
	}


	return newComponent;
}

Component* GameObject::AddComponentOfType(Component::Type type, const Component& copy)
{
	Component* newComponent = nullptr;
	switch (type)
	{
	case Component::Type::TRANSFORM:
		return transform;
		break;
	case Component::Type::MESH_RENDERER:
		newComponent = new MeshRenderComponent(this, *(MeshRenderComponent*)&copy);
		_components.push_back(newComponent);
		break;
	case Component::Type::SKINNING:
		newComponent = new SkinnedMeshRenderComponent(this, *(SkinnedMeshRenderComponent*)&copy);
		_components.push_back(newComponent);
		break;
	case Component::Type::TEXTURE:
		newComponent = new TextureComponent(this);
		_components.push_back(newComponent);
		break;
	case Component::Type::CAMERA:
		newComponent = new CameraComponent(this);
		_components.push_back(newComponent);
		break;
	case Component::Type::SCRIPT:
		newComponent = new ScriptComponent(this);
		_components.push_back(newComponent);
		break;
	case Component::Type::PARTICLE_SYSTEM:
		newComponent = new ParticleSystemComponent(this, *(ParticleSystemComponent*)&copy);
		_components.push_back(newComponent);
		break;
	case Component::Type::BILLBOARD:
		newComponent = new BillBoardComponent(this);
		break;
	case Component::Type::PHYSICS:
		newComponent = new PhysicsComponent(this);
		_components.push_back(newComponent);
		break;
    case Component::Type::MATERIAL:
        newComponent = new MaterialComponent(this);
        _components.push_back(newComponent);
        break;
    case Component::Type::DIRECTIONAL_LIGHT:
        newComponent = new DirectionalLightComponent(this);
        _components.push_back(newComponent);
        break;
    case Component::Type::POINT_LIGHT:
        newComponent = new PointLightComponent(this);
        _components.push_back(newComponent);
        break;
    case Component::Type::SPOT_LIGHT:
        newComponent = new SpotLightComponent(this);
        _components.push_back(newComponent);
	}

	return newComponent;
}

void GameObject::AddComponentSerialized(Component::Type type, json& jsonFile)
{
	Component* newComponent = AddComponentOfType(type);
	newComponent->DeSerialization(jsonFile);
}
