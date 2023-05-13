#include "Headers.h"
#include "ModuleLayers.h"
#include "LayerEditor.h"
#include "LayerGame.h"
#include "LayerUI.h"
#include "MeshImporter.h"
#include "TextureImporter.h"
#include "TransformComponent.h"
#include "ModuleResourceManager.h"

#include "ModuleInput.h"
#include "ModuleWindow.h"
#include "ModuleNavMesh.h"

//TEMPORAL
#include "ComponentUI.h"

LayerGame* ModuleLayers::game = nullptr;
LayerUI* ModuleLayers::layerUI = nullptr;
GameObject* ModuleLayers::rootGameObject = nullptr;
LayerEditor* ModuleLayers::editor = nullptr;
std::map<uint, GameObject*> ModuleLayers::gameObjects;
Layer* ModuleLayers::_layers[(uint)LayersID::MAX] = { nullptr };
bool ModuleLayers::_requestScene = false;
std::string ModuleLayers:: _requestScenePath = "null";
std::string ModuleLayers::_sceneBeginPath = "null";
std::vector<GameObject*> ModuleLayers::_deletedGameObjects;
std::map<uint, API::API_GameObject*> ModuleLayers::apiGameObjects;

API::API_Transform* ModuleLayers::emptyAPITransform = nullptr;
API::API_GameObject* ModuleLayers::emptyAPIGameObject = nullptr;


std::vector<std::string> ModuleLayers::allScenesInAssets;
bool ModuleLayers::reimportAllScenes = false;

ModuleLayers::ModuleLayers()
{
}

ModuleLayers::~ModuleLayers()
{
}

bool ModuleLayers::Start()
{
    // Create empty API components.
    emptyAPITransform = new API::API_Transform();
    emptyAPIGameObject = new API::API_GameObject();

#ifdef STANDALONE
    _layers[(uint)LayersID::EDITOR] = new LayerEditor();
#endif
    _layers[(uint)LayersID::GAME] = new LayerGame();
    _layers[(uint)LayersID::UI] = new LayerUI();

    game = (LayerGame*)_layers[(uint)LayersID::GAME];
    layerUI = (LayerUI*)_layers[(uint)LayersID::UI];
    editor = (LayerEditor*)_layers[(uint)LayersID::EDITOR];

    for (int i = 0; i < (uint)LayersID::MAX; i++)
    {
        if (_layers[i] && _layers[i]->IsEnabled())
            _layers[i]->Start();
    }

    // Create Root GameObject (Scene)
    XMLNode sceneXML = Application::Instance()->xml->GetConfigXML();

    _sceneBeginPath = sceneXML.FindChildBreadth("currentScene").node.attribute("value").as_string();

    if (!ModuleResourceManager::S_DeserializeScene(_sceneBeginPath))
    {
        rootGameObject = new GameObject(nullptr, "Root", "None");
        XMLNode sceneXML = Application::Instance()->xml->GetConfigXML();

        std::string newDir = ASSETS_PATH;

        // Change Title
        newDir += rootGameObject->name + ".HScene";

        std::string scenePath = " -- CurrentScene: " + newDir;

        ModuleWindow::S_AddTitleExtraInfo(scenePath);

        sceneXML.FindChildBreadth("currentScene").node.attribute("value").set_value(newDir.c_str());
        
        ModuleResourceManager::S_SerializeScene(rootGameObject);
    }
    else
        ModuleNavMesh::S_Load();

#ifndef STANDALONE
    LayerGame::S_Play();
#endif

    return true;
}

UpdateStatus ModuleLayers::PreUpdate()
{
    OPTICK_EVENT();
    if(_requestScene)
    {
        if (!ModuleResourceManager::S_DeserializeScene(_requestScenePath))
            _requestScenePath = "null";
        else
            ModuleNavMesh::S_Load(_requestScenePath);

        _requestScene = false;
    }

    if (reimportAllScenes)
    {
        ModuleResourceManager::S_SerializeScene(rootGameObject);

        if (allScenesInAssets.empty())
        {
            reimportAllScenes = false;
            ModuleFiles::S_EraseOldResources();
            return UpdateStatus::UPDATE_CONTINUE;
        }

        S_RequestLoadScene(allScenesInAssets[0]);
        allScenesInAssets.erase(allScenesInAssets.begin());
    }

    for (int i = 0; i < _deletedGameObjects.size(); i++)
    {
        RELEASE(_deletedGameObjects[i]);
    }
    _deletedGameObjects.clear();

    for (int i = (uint)LayersID::MAX - 1 ; i >= 0; i--)
    {
        if (_layers[i] && _layers[i]->IsEnabled()) 
            _layers[i]->PreUpdate();
    }

    return UpdateStatus::UPDATE_CONTINUE;
}

UpdateStatus ModuleLayers::Update()
{
    for (int i = 0; i < (uint)LayersID::MAX; i++)
    {
        if (_layers[i] && _layers[i]->IsEnabled()) 
            _layers[i]->Update();
    }
    return UpdateStatus::UPDATE_CONTINUE;
}

UpdateStatus ModuleLayers::PostUpdate()
{
    return UpdateStatus::UPDATE_CONTINUE;
}

void ModuleLayers::S_DrawLayers()
{
    OPTICK_EVENT();
    for (int i = 0; i < (uint)LayersID::EDITOR; i++)
    {
        if (_layers[i] && _layers[i]->IsEnabled())
            _layers[i]->PostUpdate();
    }
}

void ModuleLayers::S_DrawEditor()
{
    OPTICK_EVENT();
    if (_layers[(uint)LayersID::EDITOR] && _layers[(uint)LayersID::EDITOR]->IsEnabled()) 
        _layers[(uint)LayersID::EDITOR]->PostUpdate();
}

bool ModuleLayers::CleanUp()
{ 
#ifdef STANDALONE
    XMLNode sceneXML = Application::Instance()->xml->GetConfigXML();

    std::string configScene = sceneXML.FindChildBreadth("currentScene").node.attribute("value").as_string();

    if (configScene == "null")
    {
        std::string newDir = ASSETS_PATH;

        newDir += rootGameObject->name + ".HScene";

        sceneXML.FindChildBreadth("currentScene").node.attribute("value").set_value(newDir.c_str());
    }

    if (!LayerGame::S_IsPlaying())
        ModuleResourceManager::S_SerializeScene(rootGameObject);
#endif

    for (int i = 0; i < (uint)LayersID::MAX; i++)
    {
       if (_layers[i]) 
           _layers[i]->CleanUp();
       
       RELEASE(_layers[i]);
    }
   
    RELEASE(rootGameObject);
    RELEASE(emptyAPITransform);
    RELEASE(emptyAPIGameObject);
    return true;
}

uint ModuleLayers::S_AddGameObject(GameObject* go, uint ID)
{
    ID = ID == 0 ? HelloUUID::GenerateUUID() : ID;
    gameObjects[ID] = go;
    return ID;
}

void ModuleLayers::S_RemoveGameObject(uint ID)
{
    if (apiGameObjects.count(ID) != 0)
        apiGameObjects[ID]->_gameObject = nullptr;

    gameObjects.erase(ID);
}

void ModuleLayers::S_RequestLoadScene(const std::string& scenePath)
{
    _requestScenePath = scenePath;

    _requestScene = true;
}

GameObject* ModuleLayers::S_GetGameObject(uint ID)
{
    if (gameObjects.find(ID) != gameObjects.end())
        return gameObjects[ID];

    return nullptr;
}

void ModuleLayers::DestroyMeshes()
{
    for (auto& gameObject : gameObjects)
    {
        MeshRenderComponent* mesh = gameObject.second->GetComponent<MeshRenderComponent>();
        if (mesh != nullptr)
            mesh->DestroyMesh();  
    }
}

void ModuleLayers::RequestReimportAllScenes(std::vector<std::string>& scenes)
{
    allScenesInAssets = scenes;
    reimportAllScenes = true;
}
