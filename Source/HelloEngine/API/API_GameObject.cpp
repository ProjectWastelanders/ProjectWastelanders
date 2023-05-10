#include "Headers.h"
#include "API_GameObject.h"
#include "GameObject.h"
#include "Console.h"
#include "ModuleLayers.h"
#include "ScriptComponent.h"
#include "API_Transform.h"
#include "API_MeshRenderer.h"
#include "API/API_RigidBody.h"
#include "API/API_Material.h"
#include "API/API_ParticleSystem.h"
#include "API/API_AnimationPlayer.h"
#include "API/API_Agent.h"
#include "PhysicsComponent.h"
#include "TextureComponent.h"
#include "ParticleSystemComponent.h"
#include "MeshRenderComponent.h"
#include "API/API_SkinnedMeshRenderer.h"
#include "SkinnedMeshRenderComponent.h"
#include "AnimationComponent.h"
#include "NavAgentComponent.h"
#include "API_UIButton.h"
#include "ComponentUIButton.h"

API::API_GameObject::API_GameObject()
{

}

API::API_GameObject::~API_GameObject()
{
    if (_gameObject != nullptr)
    {
        ModuleLayers::apiGameObjects.erase(_gameObject->GetID());
    }
}

uint API::API_GameObject::GetUID()
{
    if (_gameObject == nullptr)
    {
        Console::S_Log("Trying to acces a NULLPTR GameObject. GetUID()");
        return false;
    }
    return _gameObject->GetID();
}

const char* API::API_GameObject::GetName()
{
    if (_gameObject == nullptr)
    {
        Console::S_Log("Trying to acces a NULLPTR GameObject! GetName()");
        return "NULL";
    }
    return _gameObject->name.c_str();
}

const char* API::API_GameObject::GetTag()
{
    if (_gameObject == nullptr)
    {
        Console::S_Log("Trying to acces a NULLPTR GameObject! GetTag()");
        return "NULL";
    }
    return _gameObject->tag.c_str();
}

void API::API_GameObject::SetTag(const char* tag)
{
    if (_gameObject == nullptr)
    {
        Console::S_Log("Trying to acces a NULLPTR GameObject! GetTag()");
        return;
    }
    _gameObject->tag = tag;
}

void API::API_GameObject::SetName(const char name)
{
    if (_gameObject == nullptr)
    {
        Console::S_Log("Trying to acces a NULLPTR GameObject! GetTag()");
        return;
    }
    _gameObject->name = name;
}

void API::API_GameObject::GetChildren(API_GameObject* buffer, int count)
{
    if (_gameObject == nullptr)
    {
        Console::S_Log("Trying to acces a NULLPTR GameObject! AddScript()");
        return;
    }
    std::vector<GameObject*>* children = _gameObject->GetChildren();
    int currentCount = 0;

    for (int i = 0; i < children->size(); ++i)
    {
        API_GameObject child;
        child.SetGameObject(children->at(i));
        *buffer = child;
        ++buffer;
        ++currentCount;
        if (currentCount == count)
            return;
    }
}

HelloBehavior* API::API_GameObject::AddScript(const char* className)
{
    if (_gameObject == nullptr)
    {
        Console::S_Log("Trying to acces a NULLPTR GameObject! AddScript()");
        return nullptr;
    }
    ScriptComponent* scriptComponent = _gameObject->AddComponent<ScriptComponent>();
    scriptComponent->AddScript(className);
    return scriptComponent->GetScript();
}

HelloBehavior* API::API_GameObject::GetScript(const char* className)
{
    if (_gameObject == nullptr)
    {
        Console::S_Log("Trying to acces a NULLPTR GameObject! GetScript()" + std::string(className));
        return nullptr;
    }
    std::vector<ScriptComponent*> scripts;
    std::vector<Component*> components = _gameObject->GetComponents();
    std::string name(className); // Needs a string to compare to another string.

    for (int i = 0; i < components.size(); ++i)
    {
        if (components[i]->GetType() == Component::Type::SCRIPT)
            scripts.push_back((ScriptComponent*)components[i]);
    }

    for (int i = 0; i < scripts.size(); ++i)
    {
        if (scripts[i]->GetScriptName() == className)
        {
            return scripts[i]->GetScript();
        }
    }

    return nullptr;
}

API::API_MeshRenderer API::API_GameObject::AddMeshRenderer()
{
    if (_gameObject == nullptr)
    {
        Console::S_Log("Trying to acces a NULLPTR GameObject! AddMeshRenderer()");
        return API_MeshRenderer();
    }
    MeshRenderComponent* component = (MeshRenderComponent*)_gameObject->AddComponentOfType(Component::Type::MESH_RENDERER);
    if (component == nullptr)
    {
        Console::S_Log("Trying to add a second MeshRender to a GameObject. Only one MeshRender for game object is allowed. AddMeshRenderer()");
        return API_MeshRenderer();
    }
    API_MeshRenderer ret;
    ret.SetComponent(component);
    return ret;
}

API::API_MeshRenderer API::API_GameObject::AddMeshRenderer(API_MeshRenderer& copy)
{
    if (_gameObject == nullptr)
    {
        Console::S_Log("Trying to acces a NULLPTR GameObject! AddMeshRenderer()");
        return API_MeshRenderer();
    }
    if (copy._meshRenderer == nullptr)
    {
        Console::S_Log("Trying to acces a NULLPTR MeshRenderComponent! AddMeshRenderer()");
        return API_MeshRenderer();
    }
    MeshRenderComponent* component = (MeshRenderComponent*)_gameObject->AddComponentOfType(Component::Type::MESH_RENDERER, *(Component*)copy._meshRenderer);

    if (component == nullptr)
    {
        Console::S_Log("Trying to add a second MeshRender to a GameObject. Only one MeshRender for game object is allowed. AddMeshRenderer()");
        return API_MeshRenderer();
    }

    API_MeshRenderer ret;
    ret.SetComponent(component);
    return ret;
}

API::API_ParticleSystem API::API_GameObject::AddParticleSystem()
{
    if (_gameObject == nullptr)
    {
        Console::S_Log("Trying to acces a NULLPTR GameObject! AddMeshRenderer()");
        return API::API_ParticleSystem();
    }
    ParticleSystemComponent* component = (ParticleSystemComponent*)_gameObject->AddComponentOfType(Component::Type::PARTICLE_SYSTEM);
    if (component == nullptr)
    {
        Console::S_Log("Trying to add a second MeshRender to a GameObject. Only one MeshRender for game object is allowed. AddMeshRenderer()");
        return API::API_ParticleSystem();
    }
    API_ParticleSystem ret;
    ret.SetComponent(component);
    return ret;
}

API::API_ParticleSystem API::API_GameObject::AddParticleSystem(API_ParticleSystem& copy)
{
    if (_gameObject == nullptr)
    {
        Console::S_Log("Trying to acces a NULLPTR GameObject! AddParticleSystem()");
        return API::API_ParticleSystem();
    }
    ParticleSystemComponent* copyComponent = copy.GetComponent();
    if (copyComponent == nullptr)
    {
        Console::S_Log("Trying to use a Nullptr particle system as a copy for a new one. AddParticleSystem()");
        return API::API_ParticleSystem();
    }
    ParticleSystemComponent* component = (ParticleSystemComponent*)_gameObject->AddComponentOfType(Component::Type::PARTICLE_SYSTEM, *(Component*)copy.GetComponent());
    if (component == nullptr)
    {
        Console::S_Log("Trying to add a second particle system to a GameObject. Only one MeshRender for game object is allowed. AddParticleSystem()");
        return API::API_ParticleSystem();
    }
    API_ParticleSystem ret;
    ret.SetComponent(component);
    return ret;
}

API::API_Material API::API_GameObject::AddMaterial()
{
    if (_gameObject == nullptr)
    {
        Console::S_Log("Trying to acces a NULLPTR GameObject! AddMaterial()");
        return API_Material();
    }
    TextureComponent* component = (TextureComponent*)_gameObject->AddComponent<TextureComponent>();

    if (component == nullptr)
    {
        Console::S_Log("Trying to add a second Material to a GameObject. Only one Material for game object is allowed. AddMaterial()");
        return API_Material();
    }

    API_Material ret;
    ret.SetComponent(component);
    return ret;
}

API::API_MeshRenderer API::API_GameObject::GetMeshRenderer()
{
    if (_gameObject == nullptr)
    {
        Console::S_Log("Trying to acces a NULLPTR GameObject! GetMeshRenderer()");
        return API_MeshRenderer();
    }
    API_MeshRenderer ret;
    ret.SetComponent(_gameObject->GetComponent<MeshRenderComponent>());
    return ret;
}

API::API_Material API::API_GameObject::GetMaterialCompoennt()
{
    if (_gameObject == nullptr)
    {
        Console::S_Log("Trying to acces a NULLPTR GameObject! GetMaterialCompoennt()");
        return API_Material();
    }
    API_Material ret;
    ret.SetComponent(_gameObject->GetComponent<TextureComponent>());
    return ret;
}

API::API_ParticleSystem API::API_GameObject::GetParticleSystem()
{
    if (_gameObject == nullptr)
    {
        Console::S_Log("Trying to acces a NULLPTR GameObject! GetParticleSystem()");
        return API_ParticleSystem();
    }
    API_ParticleSystem ret;
    ret.SetComponent(_gameObject->GetComponent<ParticleSystemComponent>());
    return ret;
}

API::API_RigidBody API::API_GameObject::GetRigidBody()
{
    if (_gameObject == nullptr)
    {
        Console::S_Log("Trying to acces a NULLPTR GameObject! GetRigidBody()");
        return API_RigidBody();
    }
    API_RigidBody ret;
    ret.SetComponent(_gameObject->GetComponent<PhysicsComponent>());
    return ret;
}

API::API_AnimationPlayer API::API_GameObject::GetAnimationPlayer()
{
    if (_gameObject == nullptr)
    {
        Console::S_Log("Trying to acces a NULLPTR GameObject! GetAnimationPlayer()");
        return API_AnimationPlayer();
    }
    API_AnimationPlayer ret;
    ret.SetComponent(_gameObject->GetComponent<AnimationComponent>());
    return ret;
}

API::API_SkinnedMeshRenderer API::API_GameObject::GetSkinnedMeshRender()
{
    if (_gameObject == nullptr)
    {
        Console::S_Log("Trying to acces a NULLPTR GameObject! GetSkinnedMeshRender()");
        return API_SkinnedMeshRenderer();
    }
    API_SkinnedMeshRenderer ret;
    ret.SetComponent(_gameObject->GetComponent<SkinnedMeshRenderComponent>());
    return ret;
}

API::API_Agent API::API_GameObject::GetAgent()
{
    if (_gameObject == nullptr)
    {
        Console::S_Log("Trying to acces a NULLPTR GameObject! GetAgent()");
        return API_Agent();
    }
    API_Agent ret;
    ret.SetComponent(_gameObject->GetComponent<ComponentAgent>());
    return ret;
}

API::API_UIButton API::API_GameObject::GetUIButton()
{
    if (_gameObject == nullptr)
    {
        Console::S_Log("Trying to acces a NULLPTR GameObject! GetButton()");
        return API::API_UIButton();
    }
    API_UIButton ret;
    ret.SetComponent(_gameObject->GetComponent<ComponentUIButton>());
    return ret;
}

void API::API_GameObject::SetActive(bool active)
{
    if (_gameObject == nullptr)
    {
        Console::S_Log("Trying to acces a NULLPTR GameObject. SetActive()");
        return;
    }
    _gameObject->SetActive(active);
}

bool API::API_GameObject::IsActive()
{
    if (_gameObject == nullptr)
    {
        Console::S_Log("Trying to acces a NULLPTR GameObject. SetActive()");
        return false;
    }
    return _gameObject->IsActive();
}

void API::API_GameObject::Destroy()
{
    if (_gameObject == nullptr)
    {
        Console::S_Log("Trying to acces a NULLPTR GameObject. Destroy()");
        return;
    }
    _gameObject->Destroy();
}

API::API_Transform API::API_GameObject::GetTransform()
{
    if (_gameObject == nullptr)
    {
        Console::S_Log("Trying to acces a NULLPTR GameObject. GetTransform()");
        return *ModuleLayers::emptyAPITransform;
    }
    API_Transform returnTransform;
    returnTransform.SetComponent(_gameObject->transform);
    return returnTransform;
}

API::API_RigidBody API::API_GameObject::CreateRigidBodyBox(API::API_Vector3 pos, API::API_Vector3 rotation, API::API_Vector3 scale, bool isStatic)
{
    if (_gameObject == nullptr)
    {
        Console::S_Log("Trying to acces a NULLPTR GameObject. GetTransform()");
        return API::API_RigidBody();
    }

    PhysicsComponent* physComponent = _gameObject->AddComponent<PhysicsComponent>();
    physComponent->_shapeSelected = ColliderShape::BOX;
    physComponent->CreateCollider();
    physComponent->_physBody->colPos = { pos.x, pos.y, pos.z };
    physComponent->_physBody->colRot = { rotation.x, rotation.y, rotation.z };
    physComponent->_physBody->colScl = { scale.x, scale.y, scale.z };
    physComponent->CallUpdatePos();
    physComponent->CallUpdateRotation();
    physComponent->CallUpdateScale();
    if (isStatic)
    {
        physComponent->_physBody->isStatic = true;
        physComponent->CallUpdateMass();
    }

    API_RigidBody ret;
    ret.SetComponent(physComponent);
    return ret;
}

void API::API_GameObject::SetGameObject(GameObject* gameObject)
{
    _gameObject = gameObject;
    if (_gameObject != nullptr)
        ModuleLayers::apiGameObjects[gameObject->GetID()] = this;
}
