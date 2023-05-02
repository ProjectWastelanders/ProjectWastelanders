#include "Headers.h"
#include "LayerGame.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "MeshImporter.h"
#include "TextureImporter.h"
#include "ScriptComponent.h"
#include "AnimationComponent.h"
#include "ScriptToInspectorInterface.h"
#include "ModuleCommand.h"
#include "ModuleLayers.h"
#include "API/API_Engine.h"
#include "LayerEditor.h"

std::map<uint, BehaviorScript> LayerGame::_behaviorScripts;
std::vector<ScriptComponent*> LayerGame::_scriptComponents;
std::vector<AnimationComponent*> LayerGame::_animationComponent;
bool LayerGame::_isPlaying = false;
bool LayerGame::_paused = false;
bool LayerGame::_update = false;
bool LayerGame::_oneFrame = false;
bool LayerGame::_needsReload = false;
HMODULE LayerGame::_dllFile;
time_t LayerGame::_dllChangeTime;
bool LayerGame::detectInput = true;
std::string LayerGame::currentScene = "";
bool LayerGame::_centerMouse = false;
bool LayerGame::_canCreateBehaviors = true;
bool LayerGame::_compileDLL = false;

typedef void* (*CreateFunc)(ScriptToInspectorInterface*);

LayerGame::LayerGame()
{
#ifdef STANDALONE
	_dllChangeTime = ModuleFiles::S_CheckFileLastModify(CHECK_DIRECTORY);
	CopyFile(TEXT(DLL_DIRECTORY), TEXT("APILibrary/HelloAPI.dll"), FALSE);
#endif
	_dllFile = LoadLibrary("APILibrary/HelloAPI.dll");
}

LayerGame::~LayerGame()
{
}

void LayerGame::Start()
{
}

void LayerGame::PreUpdate()
{
#ifdef STANDALONE
	static int frameWaitCompile = 300;
	if (_compileDLL)
	{
		if (frameWaitCompile-- == 0)
		{
			ModuleFiles::S_CompileDLLProject();
			_compileDLL = false;
			frameWaitCompile = 300;
		}
	}

	// Check if saved time and change time is not equal.
	time_t temp = ModuleFiles::S_CheckFileLastModify(CHECK_DIRECTORY);
	if (temp != _dllChangeTime)
	{
		_dllChangeTime = temp;
		_needsReload = true;
	}

	if (_needsReload && !_isPlaying)
	{
		static int frameWaitHotReload = 100;
		if (frameWaitHotReload-- == 0)
		{
			S_HotReload();
			frameWaitHotReload = 100;
		}
	}
#endif

	if ((!_isPlaying || _paused) && !_oneFrame)
	{
		_update = false;
		return;
	}
	_oneFrame = false;
	_update = true;
	
	// Update time.
	EngineTime::UpdateRealTime();
	EngineTime::UpdateGameTime();
	std::cout << EngineTime::GameDeltaTime() << std::endl;
}

void LayerGame::Update()
{
	if (!_update)
		return;

	for (int i = 0; i < _animationComponent.size(); ++i)
	{
		_animationComponent.at(i)->UpdateAnimation();
	}

	for (auto& behaviorScript : _behaviorScripts)
	{
		if (behaviorScript.second.lateStart && behaviorScript.second.active)
		{
			behaviorScript.second.script->Start();
			behaviorScript.second.lateStart = false;
		}

		if (behaviorScript.second.active && behaviorScript.second.script)
			behaviorScript.second.script->Update();
	}
	API::Engine::EnginePropertiesUpdate();
}

void LayerGame::PostUpdate()
{
	if (!_update)
		return;
}

void LayerGame::S_Play()
{
	if (_needsReload || _compileDLL)
		return;

	if (_isPlaying)
		return;

#ifdef STANDALONE
	LayerEditor::S_ChangeColors(true);
#endif
	ModuleCommand::_canUseCommand = false;
	// TODO: Save scene.
	currentScene = Application::Instance()->xml->GetConfigXML().FindChildBreadth("currentScene").node.attribute("value").as_string();
	ModuleResourceManager::S_SerializeScene(ModuleLayers::rootGameObject);

	EngineTime::Reset();
	EngineTime::Start();
	_isPlaying = true;

	for (auto& behaviorScript : _behaviorScripts)
	{
		behaviorScript.second.script->Init();
		if (behaviorScript.second.active)
			behaviorScript.second.script->Start();
		else
			behaviorScript.second.lateStart = true;
	}
	API::Engine::ApplyEngineProperties();
}

void LayerGame::S_Stop()
{
	LayerEditor::S_ChangeColors(false);

	ModuleCommand::_canUseCommand = true;
	EngineTime::Reset();
	_isPlaying = false;
	_paused = false;
	// TODO: Reload scene.
	ModuleLayers::S_RequestLoadScene(currentScene);
	API::Engine::UnApplyEngineProperties();
}

void LayerGame::S_Pause()
{
	_paused = !_paused;
	if (!_paused)
		EngineTime::Start();
}

void LayerGame::S_OneFrame()
{
	if (!_isPlaying || !_paused) return;
	EngineTime::Start();
	_oneFrame = true;
}

void LayerGame::S_AddAnimationComponent(AnimationComponent* component)
{
	_animationComponent.push_back(component);
}

void LayerGame::S_RemoveAnimationComponent(AnimationComponent* component)
{
	for (int i = 0; i < _animationComponent.size(); ++i)
	{
		if (component == _animationComponent[i])
		{
			_animationComponent.erase(_animationComponent.begin() + i);
			return;
		}
	}
}

void LayerGame::StartAllScripts()
{
	for (auto& behaviorScript : _behaviorScripts)
	{
		behaviorScript.second.script->Init();
		if (behaviorScript.second.active)
			behaviorScript.second.script->Start();
		else
			behaviorScript.second.lateStart = true;
	}
}

void LayerGame::RemoveAllScripts()
{
	_behaviorScripts.clear();
	_scriptComponents.clear();
}

void LayerGame::S_AddScriptComponent(ScriptComponent* component)
{
	_scriptComponents.push_back(component);
	S_CreateBehaviorScript(component);
}

void LayerGame::S_RemoveScriptComponent(ScriptComponent* component)
{
	for (int i = 0; i < _scriptComponents.size(); ++i)
	{
		if (component == _scriptComponents[i])
		{
			_scriptComponents.erase(_scriptComponents.begin() + i);
			S_DestroyBehaviorScript(component);
			return;
		}
	}
	LOG("Didn't found component in script components vector in layer game.");
}

void LayerGame::S_HotReload()
{
	if (!ModuleFiles::S_IsMSBuildOn()) //Sleep when msBuild is not available (no automatic compile).
		Sleep(1000);

	// Destroy every HellobEhavior in map
	for (int i = 0; i < _scriptComponents.size(); ++i)
	{
		S_DestroyBehaviorScript(_scriptComponents[i]);
		_scriptComponents[i]->SaveInspectorFields();
		_scriptComponents[i]->DestroyInspectorFields();
	}

	_behaviorScripts.clear();

	// Free DLL
	FreeLibrary(_dllFile);

	// Move new dll into ouptut to use.
	while (CopyFile(TEXT(DLL_DIRECTORY), TEXT("APILibrary/HelloAPI.dll"), FALSE) == FALSE)
	{
		std::cout << "Error: " << GetLastError() << std::endl;
	}

	// Load DLL
	_dllFile = LoadLibrary(TEXT("APILibrary/HelloAPI.dll"));

	_canCreateBehaviors = true;

	// Create every HElloBehavior in map
	for (int i = 0; i < _scriptComponents.size(); ++i)
	{
		S_CreateBehaviorScript(_scriptComponents[i]);
		_scriptComponents[i]->LoadInspectorFields();
	}

	_needsReload = false;
}

void LayerGame::S_RequestHotReload()
{
	_needsReload = true;
	if (_isPlaying)
		LayerEditor::S_AddPopUpMessage("The changes will be applied when pressed Stop.");
}

void LayerGame::S_RequestDLLCompile()
{
	// Only show compiling message when automatic compilation is ON.
	if (ModuleFiles::S_IsAutomaticCompilationOn())
		_compileDLL = true;
	else
		S_DisableCreatingBehaviors();
}

void LayerGame::S_DisableCreatingBehaviors()
{
	_canCreateBehaviors = false;
}

bool LayerGame::S_IsCreatingBehaviorsEnabled()
{
	return _canCreateBehaviors;
}

void LayerGame::CleanUp()
{
	for (int i = 0; i < _scriptComponents.size(); ++i)
	{
		S_DestroyBehaviorScript(_scriptComponents[i]);
	}
	FreeLibrary(_dllFile);
}

void LayerGame::S_CreateBehaviorScript(ScriptComponent* component)
{
	if (component->scriptResource == nullptr)
		return;

	CreateFunc create = (CreateFunc)GetProcAddress(_dllFile, ("Create" + component->scriptResource->className).c_str());

	if (create == nullptr)
	{
		Console::S_Log("Error loading a script. The given script name doesnt exist: " + component->scriptResource->className);
		return;
	}

	uint randomUID = HelloUUID::GenerateUUID();
	_behaviorScripts[randomUID].script = (HelloBehavior*)create(component);
	_behaviorScripts[randomUID].active = component->IsEnabled() && component->_gameObject->IsActive();
	_behaviorScripts[randomUID].script->gameObject.SetGameObject(component->_gameObject);
	_behaviorScripts[randomUID].debugName = component->scriptResource->className;
	component->scriptUID = randomUID;
}

void LayerGame::S_DestroyBehaviorScript(ScriptComponent* component)
{
	if (component->scriptUID != 0)
		_behaviorScripts.erase(component->scriptUID);
}

bool LayerGame::S_CreateBehaviorScriptByName(const std::string& className, ScriptComponent* component)
{
	CreateFunc create = (CreateFunc)GetProcAddress(_dllFile, ("Create" + className).c_str());

	if (create == nullptr)
	{
		Console::S_Log("The given HelloBehavior class name doesnt exist: " + className + ". Script was not added.");
		return false;
	}

	uint randomUID = HelloUUID::GenerateUUID();
	_behaviorScripts[randomUID].script = (HelloBehavior*)create(component);
	_behaviorScripts[randomUID].active = component->IsEnabled();

	_behaviorScripts[randomUID].script->gameObject.SetGameObject(component->_gameObject);

	component->scriptUID = randomUID;

	// We assume this is only called from API, therefor, during game runtime. We all start after creating the Script.
	_behaviorScripts[randomUID].script->Start();

	return true;
}
