#pragma once
#include "Layer.h"
#include "MeshRenderComponent.h"
#include "GameObject.h"
#include "API/HelloBehavior.h"
#include <ctime>

#ifdef _DEBUG
#define DLL_DIRECTORY "HelloAPI/Lib/Deb/HelloAPI.dll"
#else
#define DLL_DIRECTORY "HelloAPI/Lib/Rel/HelloAPI.dll"
#endif

#ifdef _DEBUG
#define CHECK_DIRECTORY "HelloAPI/Source/.checkD"
#else
#define CHECK_DIRECTORY "HelloAPI/Source/.checkR"
#endif

class ScriptComponent;
class AnimationComponent;


struct BehaviorScript
{
	HelloBehavior* script = nullptr;
	bool active = true;
	bool lateStart = false;
	std::string debugName = "";
};

class LayerGame : public Layer
{
public:
	LayerGame();
	~LayerGame();

	void Start() override;

	void PreUpdate() override;
	void Update() override;
	void PostUpdate() override;
	void CleanUp() override;

	static void S_Play();
	static void S_Stop();
	static void S_Pause();
	static void S_OneFrame();

	/// To be called when a new scene is loaded in-game.
	static void StartAllScripts();
	static void RemoveAllScripts();

	static void S_AddScriptComponent(ScriptComponent* component);
	static void S_RemoveScriptComponent(ScriptComponent* component);

	static void S_CreateBehaviorScript(ScriptComponent* component);
	static void S_DestroyBehaviorScript(ScriptComponent* component);

	static bool S_CreateBehaviorScriptByName(const std::string& className, ScriptComponent* component);

	static void S_AddAnimationComponent(AnimationComponent* component);
	static void S_RemoveAnimationComponent(AnimationComponent* component);
	static void S_RemoveAllAnimationComponents();

	static void S_HotReload();
	static void S_RequestHotReload();
	static void S_RequestDLLCompile();

	static void S_DisableCreatingBehaviors();
	static bool S_IsCreatingBehaviorsEnabled();

	static bool S_IsPlaying() { return _isPlaying; }
	static bool S_IsPause() { return _paused; }

	static bool detectInput;

	static void UpdateBehaviorScripts();

private:
	static std::map<uint, BehaviorScript> _behaviorScripts;
	static std::vector<ScriptComponent*> _scriptComponents;
	static std::vector<AnimationComponent*> _animationComponent;
	static bool _isPlaying;
	static bool _paused;
	static bool _update;
	static bool _oneFrame;
	static bool _needsReload;
	static HMODULE _dllFile;
	static time_t _dllChangeTime;
	static std::string currentScene;
	static bool _centerMouse;
	static bool _canCreateBehaviors; // Boolean that gets put in false when a compilation has to be done in the DLL. For when automatic compilation is off.
	static bool _compileDLL;

	friend class LayerEditor;
	friend class ScriptComponent;
	friend class ImWindowGame;
};

