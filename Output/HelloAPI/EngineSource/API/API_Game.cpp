#include "Headers.h"	
#include "API_Game.h"
#include "ModuleLayers.h"
#include "Console.h"
#include "LayerGame.h"

API::API_GameObject API::Game::FindGameObject(const char* name)
{
	for (auto& gameObject : ModuleLayers::gameObjects)
	{
		if (gameObject.second->name == name)
		{
			API_GameObject apiGO;
			apiGO.SetGameObject(gameObject.second);
			return apiGO;
		}
	}
}

API::API_GameObject API::Game::CreateGameObject(const char* name, const char* tag, API_GameObject* parent)
{
	GameObject* newGameObject = nullptr;

	if (parent == nullptr)
		newGameObject = new GameObject(ModuleLayers::rootGameObject, name, tag);
	else
	{
		GameObject* newParent = parent->GetGameObject();
		if (newParent != nullptr)
			newGameObject = new GameObject(newParent, name, tag);
		else
		{
			Engine::Console::S_Log("Tried to create a new GameObject with a null parent using Game::CreateGameObject. Parent ignored.", LogType::WARNING);
			newGameObject = new GameObject(ModuleLayers::rootGameObject, name, tag);
		}
	}

	API_GameObject newAPIGameObject;
	newAPIGameObject.SetGameObject(newGameObject);

	return newAPIGameObject;
}

TO_API API::API_GameObject API::Game::InstancePrefab(uint uid, API_GameObject parent)
{
	ResourcePrefab* prefabResource = (ResourcePrefab*)ModuleResourceManager::S_LoadResource(uid);
	if (prefabResource && prefabResource->_type != ResourceType::PREFAB)
	{
		return API_GameObject();
	}
	// Get prefab
	if (prefabResource != nullptr)
	{
		// Find path of prefab. Resource path refrences an Assets path.
		std::string prefabPath = prefabResource->resourcePath;
		GameObject* parentGO = ModuleLayers::rootGameObject;
		// Set parent if given game object is valid
		if (parent.IsAlive())
			parentGO = parent.GetGameObject();

		GameObject* newGameObject = ModuleResourceManager::S_DeserializeFromPrefab(prefabPath, parentGO);

		API_GameObject newAPIGameObject;
		newAPIGameObject.SetGameObject(newGameObject);
		return newAPIGameObject;
	}
	return API_GameObject();
}

TO_API void API::Game::ExitApplication()
{
#ifdef STANDALONE
	LayerGame::S_Stop();
#else
	Application::Instance()->Exit();
#endif // STANDALONE

}

TO_API void API::Game::FindGameObjectsWithTag(const char* tag, API_GameObject* buffer, uint count)
{
	uint currentCount = 0;
	for (auto& gameObject : ModuleLayers::gameObjects)
	{
		if (gameObject.second->tag == tag)
		{
			API_GameObject apiGO;
			apiGO.SetGameObject(gameObject.second);
			*buffer = apiGO;
			++buffer;
			++currentCount;
			if (currentCount == count)
				return;
		}
	}
}
