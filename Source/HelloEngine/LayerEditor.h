#ifndef __MODULE_ENGINE_H__
#define __MODULE_ENGINE_H__

#include "ImWindow.h"
#include "Layer.h"
#include "LayerGame.h"
#include "FileTree.hpp"

class Application;
class ImWindowConfiguration;

enum class ImWindowID
{
	CONFIGURATION,
	PERFORMANCE,
	CONSOLE,
	PROJECT,
	HIERARCHY,
	INSPECTOR,
	QUICKSAVE,
	ABOUT,
	GAME,
	SCENE,
	UI,
	RESOURCES,
	GLSL,
	MAX,
};

struct PopUpMessage
{
	PopUpMessage(std::string message) :message(message) {}

	std::string message = "";
	
	float currentMessageTime = 0.0f;

	bool hovered = false;
};

class LayerEditor :public Layer
{
public:
	LayerEditor();
	~LayerEditor();

	void Start() override;

	void PreUpdate() override;
	void Update() override;
	void PostUpdate() override;

	void CleanUp() override;

	static void S_SetSelectGameObject(GameObject* g);

	//GameObject* GetSelectedGameObject() 
	//{
	//	return selectedGameObject; 
	//}

	static void S_AddPopUpMessage(std::string message);

	static void S_ChangeColors(bool playMode);

	static void S_ShowCompilationError() { _showCompilationError = true; }

	static void S_OpenShader(uint UID);
	static void S_ReimportAllAssets();

	static bool S_GetWindowActive(ImWindowID id);

private:
	void DrawMenuBar();

	// Draw scene open and save popups
	void DrawPopUpLoadScene();
	void DrawPopUpSaveScene();
	void DrawAssetsTree(Directory*& newDir, Directory* node, const bool drawFiles);

	void DrawPopUpMessages();

	void DrawWarningForAutomaticCompilation();

	void DrawCompilationErrorWarning();

public:
	static GameObject* selectedGameObject;
	static GameObject* draggedGameObject;

	static bool detectInput;
private:
	static ImWindow* _imWindows[(uint)ImWindowID::MAX];

	Application* _app = nullptr;


	//Popup message variables
	static std::vector<PopUpMessage> _popUpMessages;
	static float _messageTime;

	// Scene popups
	static bool _openLoadScene;
	static bool _openSaveScene;
	static FileTree* _fileTree;
	static std::string _currentSelectedPath;
	static std::string _savingSceneName;
	static bool _requestUpdateFileTree;
	static bool _cannotCancel;

	// Play/Stop buttons
	static uint _playImageID;
	static uint _stopImageID;
	static uint _pauseImageID;
	static uint _nextImageID;

	// Compilation warning
	static bool _showCompilationWarning;
	static bool _showCompilationError;
	friend class ImWindowHierarchy;
	friend class ComponentUI;
	friend class ComponentUISlider;
	friend class ComponentUIImage;
	friend class ComponentUIInput;
};

#endif // !__MODULE_ENGINE_H__