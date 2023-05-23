#pragma once
#include "Globals.h"
#include <string>
#include "API_Vector3.h"

class GameObject;
class DragBoxGameObject;
class LayerGame;
class HelloBehavior;

namespace API
{
    class API_Transform;
    class API_MeshRenderer;
    class API_Camera;
    class API_RigidBody;
    class API_AnimationPlayer;
    class API_ParticleSystem;
    class API_Material;
    class API_UIButton;
    class API_UICheckBox;
    class API_UISlider;
    class API_UIImage;
    class API_UIInput;
    class API_UIText;
    class API_SkinnedMeshRenderer;
    class API_ShaderComponent;
    class API_Agent;
    class API_AudioSourceComponent;

    class TO_API API_GameObject
    {
    public:
        API_GameObject();
        ~API_GameObject();

        // Any necessary methods from the GameObject class, but with a nullptr check before calling.

        const char* GetName();
        const char* GetTag();

        void SetTag(const char* tag);
        void SetName(const char name);

        /// WARNING: Do not call this function with an array too small!
        /// <param name="buffer">An API_GameObject array to fill with this gameobject's children</param>
        /// <param name="count">Number of children to get. -1 = All children.</param>
        int GetChildren(API_GameObject* buffer, int count = -1);
        API_GameObject GetParent();
        bool SetParent(API_GameObject parent);

        HelloBehavior* AddScript(const char* className);
        HelloBehavior* GetScript(const char* className);

        API_MeshRenderer AddMeshRenderer();
        API_MeshRenderer AddMeshRenderer(API_MeshRenderer& copy);
        API_MeshRenderer GetMeshRenderer();

        API_ParticleSystem AddParticleSystem();
        API_ParticleSystem AddParticleSystem(API_ParticleSystem& copy);
        API_ParticleSystem GetParticleSystem();

        API_Material AddMaterial();
        API_Material GetMaterialCompoennt();

        API_RigidBody GetRigidBody();

        API_AnimationPlayer GetAnimationPlayer();

        API_SkinnedMeshRenderer GetSkinnedMeshRender();

        API_Agent GetAgent();

        API_UIButton GetUIButton();

        API_UIImage GetUIImage();

        uint GetUID();

        void SetActive(bool active);
        bool IsActive();

        // Destroys this GameObject instance from the scene. You can still use the API_GameObject class, but it wont make any effect.
        void Destroy();

        bool IsAlive() { return _gameObject != nullptr; }

        API::API_Transform GetTransform();

        // Create components

        API::API_RigidBody CreateRigidBodyBox(API_Vector3 pos, API_Vector3 rotation, API_Vector3 scale, bool isStatic);

    private:
        GameObject* GetGameObject() { return _gameObject; };
        void SetGameObject(GameObject* gameObject);
    private:
        GameObject* _gameObject = nullptr;
        friend class ModuleLayers;
        friend class DragBoxGameObject;
        friend class API_Transform;
        friend class API_MeshRenderer;
        friend class API_RigidBody;
        friend class API_Camera;
        friend class API_AnimationPlayer;
        friend class API_Material;
        friend class Game;
        friend class LayerGame;
        friend class API_ParticleSystem;
        friend class API_UIButton;
        friend class API_UICheckBox;
        friend class API_UISlider;
        friend class API_UIImage;
        friend class API_UIInput;
        friend class API_UIText;
        friend class API_SkinnedMeshRenderer;
        friend class API_ShaderComponent;
        friend class API_Agent;
        friend class API_RayCast;
        friend class API_AudioSourceComponent;
    };
}