#pragma once
#include "Module.h"
#include "Layer.h"
#include "GameObject.h"

// Need to be inside the .h file, because classes inside a namespace cannot be forward declared ?
#include "API/API_GameObject.h"
#include "API/API_Transform.h"

class LayerEditor;
class LayerGame;
class LayerUI;
class ComponentUI;

enum LayersID
{
	GAME,
	UI,
	DEBUG,
	EDITOR,// Editor should always be the last layer
	MAX
};

class ModuleLayers : public Module
{
public:
	ModuleLayers();
	~ModuleLayers();

	bool Start() override;

	UpdateStatus PreUpdate() override;
	UpdateStatus Update() override;
	UpdateStatus PostUpdate() override;

	bool CleanUp() override;
	
	static void S_DrawLayers();

	static void S_DrawEditor();

	static uint S_AddGameObject(GameObject* go, uint ID = 0);
	static void S_RemoveGameObject(uint ID);

	static void S_RequestLoadScene(const std::string& scenePath);

	static GameObject* S_GetGameObject(uint ID);

	/// Iterate over every Game object and destroy their MeshRenderComponent mesh (the object, not the component itself).
	static void DestroyMeshes();

	static void RequestReimportAllScenes(std::vector<std::string>& scenes);

	static void RemoveRigidBody(uint gameObjectUID);

public:
	static LayerGame* game;
	static LayerEditor* editor;
	static LayerUI* layerUI;

	static GameObject* rootGameObject;

	static std::map<uint, GameObject*> gameObjects;

	static std::map<uint, API::API_GameObject*> apiGameObjects;
	static std::map<uint, API::API_RigidBody*> apiRigidBodies;

	// Empty API_Components to return in case you try to access the component of an unexisting game object.
	static API::API_GameObject* emptyAPIGameObject;
	static API::API_Transform* emptyAPITransform;

	static std::vector<std::string> allScenesInAssets;
	static bool reimportAllScenes;

private:
	static Layer* _layers[(uint)LayersID::MAX];

	static bool _requestScene;

	static std::string _requestScenePath;

	static std::string _sceneBeginPath;

	static std::vector<GameObject*> _deletedGameObjects;

	friend class GameObject;
};