#include "Headers.h"
#include "LayerEditor.h"
#include "Application.h"
#include "QuickSave.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleCommand.h"
#include "ModuleFiles.h"

#include "ImGuizmo/ImGuizmo.h"

#include "ImWindowConfiguration.h"
#include "ImWindowAbout.h"
#include "ImWindowConsole.h"
#include "ImWindowProject.h"
#include "ImWindowHierarchy.h"
#include "ImWindowInspector.h"
#include "ImWindowQuickSave.h"
#include "ImWindowScene.h"
#include "ImWindowUI.h"
#include "ImWindowGame.h"
#include "ImWindowPerformanceTest.h"
#include "ImWindowResources.h"
#include "ImWindowNavMesh.h"
#include "ImWindowGLSL.h"

#include "ModuleLayers.h"
#include "LayerGame.h"

#include "TransformComponent.h"
#include "TextureImporter.h"

// Init static variables
GameObject* LayerEditor::selectedGameObject = nullptr;
GameObject* LayerEditor::draggedGameObject = nullptr;
ImWindow* LayerEditor::_imWindows[(uint)ImWindowID::MAX] = {nullptr};
std::vector<PopUpMessage> LayerEditor::_popUpMessages;
float LayerEditor::_messageTime = 3.0f;
bool LayerEditor::_openLoadScene = false;
bool LayerEditor::_openSaveScene = false;
FileTree* LayerEditor::_fileTree = nullptr;
std::string LayerEditor::_currentSelectedPath = "";
std::string LayerEditor::_savingSceneName = "";
bool LayerEditor::_requestUpdateFileTree = false;
uint LayerEditor::_playImageID = 0;
uint LayerEditor::_stopImageID = 0;
uint LayerEditor::_pauseImageID = 0;
uint LayerEditor::_nextImageID = 0;
bool LayerEditor::_showCompilationWarning = false;
bool LayerEditor::_showCompilationError = false;
bool LayerEditor::_cannotCancel = false;
bool LayerEditor::detectInput = true;

LayerEditor::LayerEditor()
{
}

LayerEditor::~LayerEditor()
{
}

void LayerEditor::Start()
{
	// Get application
	_app = Application::Instance();

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGuiContext* imGuiContext = ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;  
	//io.ConfigViewportsNoAutoMerge = true;
	//io.ConfigViewportsNoTaskBarIcon = true;

	ImGuizmo::SetImGuiContext(imGuiContext);
	ImGuizmo::SetOrthographic(false);
	// Setup Dear ImGui style
	
	//ImGui::StyleColorsClassic();
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 10.0f;
		style.Colors[ImGuiCol_WindowBg].w = 0.0f;
		style.Colors[ImGuiCol_Border] = ImVec4(255, 1, 1, 255);
	}

	// Setup ImGui ui style
	S_ChangeColors(false);
	
	// Setup font
	char* buf = nullptr;

	uint bufSize = ModuleFiles::S_Load("Resources/Editor/Fonts/font.ttf", &buf);

	// ImGui will delete buf memory when close application, so no need realease here
	io.Fonts->AddFontFromMemoryTTF(buf, bufSize, 14.0f, NULL);

	// Init OpenGL
	const char* glsl_version = "#version 130";
	ImGui_ImplSDL2_InitForOpenGL(ModuleWindow::window, _app->renderer3D->context);
	ImGui_ImplOpenGL3_Init(glsl_version);

	// Create ImGui editor windows
	{
		_imWindows[(uint)ImWindowID::CONFIGURATION] = new ImWindowConfiguration();
		_imWindows[(uint)ImWindowID::ABOUT] = new ImWindowAbout();
		_imWindows[(uint)ImWindowID::CONSOLE] = new ImWindowConsole();
		_imWindows[(uint)ImWindowID::PROJECT] = new ImWindowProject();
		_imWindows[(uint)ImWindowID::QUICKSAVE] = new ImWindowQuickSave();
		_imWindows[(uint)ImWindowID::INSPECTOR] = new ImWindowInspector();
		_imWindows[(uint)ImWindowID::HIERARCHY] = new ImWindowHierarchy();
		_imWindows[(uint)ImWindowID::SCENE] = new ImWindowScene();
		_imWindows[(uint)ImWindowID::UI] = new ImWindowUI();
		_imWindows[(uint)ImWindowID::GAME] = new ImWindowGame();
		_imWindows[(uint)ImWindowID::PERFORMANCE] = new ImWindowPerformanceTest();
		_imWindows[(uint)ImWindowID::RESOURCES] = new ImWindowResources();
		_imWindows[(uint)ImWindowID::NAVMESH] = new ImWindowNavMesh();
		_imWindows[(uint)ImWindowID::GLSL] = new ImWindowGLSL();
	}

	// Reserve space for popUpMessages
	_popUpMessages.reserve(20);

	// Save and load scene popups variables
	ModuleResourceManager::S_GetFileTree(_fileTree);

	// Load icons
	{
		char* buffer = nullptr;
		int size = ModuleFiles::S_Load("Resources/Editor/Images/play.dds", &buffer);
		_playImageID = TextureImporter::LoadEditorDDS(buffer, size);
		RELEASE(buffer);

		buffer = nullptr;
		size = ModuleFiles::S_Load("Resources/Editor/Images/stop.dds", &buffer);
		_stopImageID = TextureImporter::LoadEditorDDS(buffer, size);
		RELEASE(buffer);

		buffer = nullptr;
		size = ModuleFiles::S_Load("Resources/Editor/Images/pause.dds", &buffer);
		_pauseImageID = TextureImporter::LoadEditorDDS(buffer, size);
		RELEASE(buffer);

		buffer = nullptr;
		size = ModuleFiles::S_Load("Resources/Editor/Images/next.dds", &buffer);
		_nextImageID = TextureImporter::LoadEditorDDS(buffer, size);
		RELEASE(buffer);
	}

	// Check automatic compilation
	_showCompilationWarning = !ModuleFiles::S_IsMSBuildOn();

	EngineTime::StartEngine();
}

void LayerEditor::PreUpdate()
{
	if (_requestUpdateFileTree)
	{
		ModuleResourceManager::S_UpdateFileTree();
		_requestUpdateFileTree = false;
		ImWindowProject* projectWindow = (ImWindowProject*)_imWindows[(uint)ImWindowID::PROJECT];
		projectWindow->UpdateFileNodes();
	}

	//Update Engine Time
	EngineTime::UpdateEngineTime();
}

void LayerEditor::Update()
{
	if (!detectInput)
		return;

	if(ModuleInput::S_GetKey(SDL_SCANCODE_DELETE) == KEY_DOWN)
		ModuleCommand::S_DeleteGameObject(selectedGameObject);

	if (ModuleInput::S_GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT && ModuleInput::S_GetKey(SDL_SCANCODE_S) == KEY_DOWN)
	{
		if (ModuleInput::S_GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
		{
			// Restart selectedPath
			Directory* rootDir = nullptr;
			_fileTree->GetRootDir(rootDir);
			_currentSelectedPath = rootDir->path;

			_openSaveScene = true;
		}
		else
		{
			XMLNode sceneXML = Application::Instance()->xml->GetConfigXML();
			std::string configScene = sceneXML.FindChildBreadth("currentScene").node.attribute("value").as_string();

			if (configScene == "null")
				_openSaveScene = true;
			else
				ModuleResourceManager::S_SerializeScene(ModuleLayers::rootGameObject);
			
			S_AddPopUpMessage("Saved scene: " + ModuleFiles::S_GetFileName(configScene));
		}
	}
}

void LayerEditor::PostUpdate()
{
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	// ImGuizmo
	ImGuizmo::BeginFrame();

	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

	if (_showCompilationWarning)
		DrawWarningForAutomaticCompilation();

	if (_showCompilationError)
		DrawCompilationErrorWarning();

	DrawMenuBar();

	DrawPopUpMessages();

	// Draw Every windows
    for (int i = 0; i < (uint)ImWindowID::MAX; i++)
    {
        if (_imWindows[i]->isEnabled) 
			_imWindows[i]->Update();
    }

#ifdef STANDALONE
	Application::Instance()->renderer3D->renderManager.OnEditor();
#endif

	if (_openLoadScene)
		DrawPopUpLoadScene();

	if (_openSaveScene)
		DrawPopUpSaveScene();

	if (LayerGame::_compileDLL)
	{
		ImGui::OpenPopup("Currently Compiling");
		if (ImGui::BeginPopupModal("Currently Compiling", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("The ScriptingSLN is being compiled, please wait.");
			ImGui::EndPopup();
		}
	}

	if (LayerGame::_needsReload && !LayerGame::_isPlaying)
	{
		ImGui::OpenPopup("Currently Hot Reloading");
		if (ImGui::BeginPopupModal("Currently Hot Reloading", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Hot Reloading. Please wait.");
			ImGui::EndPopup();
		}
	}

	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// Update and Render additional Platform Windows
	// (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
	//  For this specific demo app we could also call SDL_GL_MakeCurrent(window, gl_context) directly)
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
		SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
	}
}

void LayerEditor::CleanUp()
{
	for (int i = 0; i < (uint)ImWindowID::MAX; i++)
	{
		RELEASE(_imWindows[i]);
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}

void LayerEditor::S_SetSelectGameObject(GameObject* g)
{
	if (selectedGameObject)
		selectedGameObject->isSelected = false;

	selectedGameObject = g;
	
	if (selectedGameObject)
		selectedGameObject->isSelected = true;

#ifdef STANDALONE

	ImWindowInspector* inspector = (ImWindowInspector*)_imWindows[(uint)ImWindowID::INSPECTOR];

	inspector->SelectGameObject(g);
#endif
}

void LayerEditor::S_AddPopUpMessage(std::string message)
{
	_popUpMessages.emplace_back(message);
}

void LayerEditor::S_ChangeColors(bool playMode)
{
	ImGuiStyle& style = ImGui::GetStyle();

	float redIncrement = playMode ? 0.1f : 0.0f;

	style.Colors[ImGuiCol_ScrollbarBg] = style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.1f + redIncrement, 0.1f, 0.1f, 1);

	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.15f + redIncrement, 0.15f, 0.15f, 1);

	style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.2f + redIncrement, 0.2f, 0.2f, 1);

	style.Colors[ImGuiCol_FrameBg] = style.Colors[ImGuiCol_Button] = style.Colors[ImGuiCol_TitleBg] = style.Colors[ImGuiCol_Header] =
		style.Colors[ImGuiCol_TitleBgActive] = style.Colors[ImGuiCol_TabUnfocused] = style.Colors[ImGuiCol_Tab] =
		ImVec4(0.25f + redIncrement, 0.25f, 0.25f, 1);

	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.3f + redIncrement, 0.3f, 0.3f, 1);

	style.Colors[ImGuiCol_ButtonHovered] = style.Colors[ImGuiCol_HeaderHovered] = style.Colors[ImGuiCol_FrameBgActive] =
		style.Colors[ImGuiCol_DockingPreview] = style.Colors[ImGuiCol_TabActive] = style.Colors[ImGuiCol_SliderGrab] =
		ImVec4(0.4f + redIncrement, 0.4f, 0.4f, 1);

	style.Colors[ImGuiCol_ButtonActive] = style.Colors[ImGuiCol_TitleBgCollapsed] = style.Colors[ImGuiCol_SliderGrabActive] =
		style.Colors[ImGuiCol_HeaderActive] = style.Colors[ImGuiCol_TabHovered] = style.Colors[ImGuiCol_CheckMark] =
		ImVec4(0.65f + redIncrement, 0.65f, 0.65f, 1);

	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.1f + redIncrement, 0.1f, 0.1f, 1);
}

void LayerEditor::S_OpenShader(uint UID)
{
	ImWindowGLSL* win = (ImWindowGLSL*)_imWindows[(uint)ImWindowID::GLSL];

	win->SetShader(UID);
}

void LayerEditor::DrawPopUpLoadScene()
{
	ImGui::OpenPopup("Load Scene");
	if (ImGui::BeginPopupModal("Load Scene", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Select a scene to load.\n\n");
		ImGui::Separator();
		Directory* newDir = nullptr;

		Directory* rootDir = nullptr;
		_fileTree->GetRootDir(rootDir);

		DrawAssetsTree(newDir, rootDir, true);

		if (ImGui::Button("Open"))
		{
			ModuleLayers::S_RequestLoadScene(_currentSelectedPath);
			_openLoadScene = false;
		}

		ImGui::SameLine();

		if (ImGui::Button("Close"))
			_openLoadScene = false;

		ImGui::EndPopup();
	}
}

void LayerEditor::DrawPopUpSaveScene()
{
	ImGui::OpenPopup("Save Scene");
	if (ImGui::BeginPopupModal("Save Scene", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Save your current scene.\n\n");
		ImGui::Separator();
		Directory* newDir = nullptr;

		Directory* rootDir = nullptr;
		_fileTree->GetRootDir(rootDir);

		DrawAssetsTree(newDir, rootDir, false);

		ImGui::Text("Name: "); ImGui::SameLine();
		ImGui::InputText("##inputTextSceneSave", &_savingSceneName);

		if (ImGui::Button("Save"))
		{
			std::string scenePath = _currentSelectedPath + _savingSceneName + ".HScene";

			XMLNode sceneXML = Application::Instance()->xml->GetConfigXML();

			sceneXML.FindChildBreadth("currentScene").node.attribute("value").set_value(scenePath.c_str());

			ModuleResourceManager::S_SerializeScene(ModuleLayers::rootGameObject);

			_requestUpdateFileTree = true;

			_openSaveScene = false;
			_cannotCancel = false;

			S_AddPopUpMessage("Saved scene: " + _savingSceneName);
		}

		ImGui::SameLine();

		if (!_cannotCancel)
		{
			if (ImGui::Button("Close"))
				_openSaveScene = false;
		}
		ImGui::EndPopup();
	}
}

void LayerEditor::DrawAssetsTree(Directory*& newDir, Directory* node, const bool isLoading)
{
	ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_None | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;

	bool hasScenesInside = false;

	for (int i = 0; i < node->files.size(); i++)
	{
		if (ModuleFiles::S_GetFileExtension(node->files[i].path) == "hscene")
		{
			hasScenesInside = true;
			break;
		}
	}

	if (isLoading && node == _fileTree->_currentDir)
		node_flags |= ImGuiTreeNodeFlags_Selected;
	else if(node->path == _currentSelectedPath)
		node_flags |= ImGuiTreeNodeFlags_Selected;

	if (node->directories.empty() && !hasScenesInside)
	{
		if (isLoading)
			return; // We dont want to show a folder that has no scenes in, and doesn't have any other folders.
		
		node_flags |= ImGuiTreeNodeFlags_Leaf;
	}

	if (ImGui::TreeNodeEx(node->name.c_str(), node_flags))
	{
		// Slect node
		if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
		{
			newDir = node;

			if (!isLoading)
				_currentSelectedPath = node->path;
		}

		// Recursive functions
		// Directories
		for (int i = 0; i < node->directories.size(); i++)
			DrawAssetsTree(newDir, node->directories[i], isLoading);

		// Files
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 0, 1));
		for (int i = 0; i < node->files.size(); i++)
		{
			ImGuiTreeNodeFlags leafFlags = ImGuiTreeNodeFlags_Leaf;
			if (ModuleFiles::S_GetFileExtension(node->files[i].path) == "hscene")
			{
				if (_currentSelectedPath == node->files[i].path)
					leafFlags |= ImGuiTreeNodeFlags_Selected;

				ImGui::TreeNodeEx(node->files[i].name.c_str(), leafFlags);

				if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
				{
					if (!isLoading)
					{
						_savingSceneName = ModuleFiles::S_GetFileName(node->files[i].name, false);
						_currentSelectedPath = node->files[i].parent->path;
					}
					else
						_currentSelectedPath = node->files[i].path;
				}
				ImGui::TreePop();
			}
		}
		ImGui::PopStyleColor();

		ImGui::TreePop();
	}
}

void LayerEditor::S_ReimportAllAssets()
{
	ImWindowProject* proj = (ImWindowProject*)_imWindows[(uint)ImWindowID::PROJECT];
	proj->RefreshAssets();
}

bool LayerEditor::S_GetWindowActive(ImWindowID id)
{
	return _imWindows[(uint)id]->isActive;
}

std::string LayerEditor::S_GetCurrentSceneName()
{
	XMLNode sceneXML = Application::Instance()->xml->GetConfigXML();

	std::string configScene = sceneXML.FindChildBreadth("currentScene").node.attribute("value").as_string();

	return ModuleFiles::S_GetFileName(configScene, false);
}

void LayerEditor::DrawMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.95f, 0.95f, 0.95f, 1));

		ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.05f, 0.05f, 0.05f, 1));

		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New Scene"))
			{
				// Set config scene path to null
				XMLNode sceneXML = Application::Instance()->xml->GetConfigXML();
				sceneXML.FindChildBreadth("currentScene").node.attribute("value").set_value("null");

				if (ModuleLayers::rootGameObject)
					ModuleLayers::rootGameObject->Destroy();

				ModuleLayers::rootGameObject = new GameObject(nullptr, "Root", "None");

				// Restart selectedPath
				Directory* rootDir = nullptr;
				_fileTree->GetRootDir(rootDir);
				_currentSelectedPath = rootDir->path;

				_openSaveScene = true;
				_cannotCancel = true;
			}

			if (ImGui::MenuItem("Load Scene"))
				_openLoadScene = true;

			if (ImGui::MenuItem("Save Scene (Ctrl + S)"))
			{
				XMLNode sceneXML = Application::Instance()->xml->GetConfigXML();
				std::string configScene = sceneXML.FindChildBreadth("currentScene").node.attribute("value").as_string();

				if (configScene == "null")
				{
					Directory* rootDir = nullptr;
					_fileTree->GetRootDir(rootDir);
					_currentSelectedPath = rootDir->path;

					_openSaveScene = true;
				}
				else
					ModuleResourceManager::S_SerializeScene(ModuleLayers::rootGameObject);
				
				S_AddPopUpMessage("Saved scene: " + ModuleFiles::S_GetFileName(configScene));
			}

			if (ImGui::MenuItem("Save Scene As (Ctrl + Alt + S)"))
			{
				// Restart selectedPath
				Directory* rootDir = nullptr;
				_fileTree->GetRootDir(rootDir);
				_currentSelectedPath = rootDir->path;

				_openSaveScene = true;
			}

			if (ImGui::MenuItem("Close Appplication"))
				_app->Exit();

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Windows"))
		{
			for (int i = 0; i < (uint)ImWindowID::MAX; i++)
			{
				ImGui::MenuItem(_imWindows[i]->windowName.c_str(), (const char*)0, &_imWindows[i]->isEnabled);
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Tools"))
		{
			if (ImGui::MenuItem("Reimport all assets"))
			{
				S_ReimportAllAssets();
			}
			if (ImGui::MenuItem("Regenerate metas UIDs"))
			{
				ModuleFiles::S_RegenerateMetasUIDs();
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("About"))
		{
			if (ImGui::MenuItem("Check our Github!"))
				ShellExecute(0, 0, "https://github.com/HelloGamesCompany/HelloEngine", 0, 0, SW_SHOW);

			ImGui::EndMenu();
		}
		ImGui::PopStyleColor(2);

		ImGui::EndMainMenuBar();
	}

	ImGuiViewportP* viewport = (ImGuiViewportP*)(void*)ImGui::GetMainViewport();
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings;
	float height = ImGui::GetFrameHeight();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.85f, 0.85f, 0.85f, 1.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
	if (ImGui::BeginViewportSideBar("##TimeBar", viewport, ImGuiDir_Up, 32, window_flags)) 
	{
		ImVec2 contentRegion = ImGui::GetContentRegionAvail();
		int totalButtonsWidth = 72;
		ImGui::SetCursorPos(ImVec2(contentRegion.x / 2 - totalButtonsWidth, 4));

		if (!LayerGame::_isPlaying)
		{
			if (ImGui::ImageButton("Play", (ImTextureID)_playImageID, ImVec2(24, 24)))
				LayerGame::S_Play();
			ImGui::SameLine();
		}
		else
		{
			if (ImGui::ImageButton("Stop", (ImTextureID)_stopImageID, ImVec2(24, 24)))
				LayerGame::S_Stop();
			ImGui::SameLine();
		}

		if (LayerGame::_paused)
		{
			ImGui::PopStyleColor();
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.5f, 0.5f, 1.0f));
		}

		if (ImGui::ImageButton("Pause", (ImTextureID)_pauseImageID, ImVec2(24, 24)))
			LayerGame::S_Pause();
		ImGui::SameLine();
		if (LayerGame::_paused)
		{
			ImGui::PopStyleColor();
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.85f, 0.85f, 0.85f, 1.0f));
		}

		if (ImGui::ImageButton("Next", (ImTextureID)_nextImageID, ImVec2(24, 24)))
			LayerGame::S_OneFrame();
		
		ImGui::End();
	}
	ImGui::PopStyleVar();
	ImGui::PopStyleColor();
}

void LayerEditor::DrawPopUpMessages()
{
	float fadeInFix = 0.4f; // 0 - 0.4 do fadeIn
	float fadeOutFix = 0.8f; // 0.8 - 1.0 do fadeOut

	bool fadeIn = false;
	bool fadeOut = false;
	bool hovering = false; // We need this variable to Push/Pop in the correct moment.

	int width = ModuleWindow::width;
	int height = ModuleWindow::height;

	std::string id = "popUpMessage";

	ImGuiWindowFlags popupFlags =
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_HorizontalScrollbar |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoInputs;

	if (_popUpMessages.empty())
		return;

	for (size_t i = 0; i < _popUpMessages.size(); i++)
	{
		_popUpMessages[i].currentMessageTime += (1 / ImGui::GetIO().Framerate);

		if (_popUpMessages[i].currentMessageTime >= _messageTime)
		{
			_popUpMessages.erase(_popUpMessages.begin() + i--);

			continue;
		}

		fadeIn = _popUpMessages[i].currentMessageTime <= _messageTime * fadeInFix;
		fadeOut = _popUpMessages[i].currentMessageTime >= _messageTime * fadeOutFix;

		if (fadeIn)
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, _popUpMessages[i].currentMessageTime / (_messageTime * fadeInFix));
		else if (fadeOut)
		{
			float result = 1.0f - ((_popUpMessages[i].currentMessageTime - (_messageTime * fadeOutFix)) / (1.0f - fadeOutFix));
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, result > 0 ? result : 0.0f);
		}

		ImGui::SetNextWindowSize(ImVec2(width * 0.3f, height * 0.15f));
		ImGui::SetNextWindowPos(ImVec2(width * 0.35f, (height * 0.425f) - 30.0f * _popUpMessages[i].currentMessageTime));

		id += std::to_string(i);

		if (_popUpMessages[i].hovered)
		{
			if (fadeIn || fadeOut)
				ImGui::PopStyleVar();

			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.25f);
			hovering = true;
		}
		else
			hovering = false;

		if (ImGui::Begin(id.c_str(),0, popupFlags))
		{
			float2 temp = float2(width * 0.3f, height * 0.15f);
			temp += float2(width * 0.35f, (height * 0.425f) - 30.0f * _popUpMessages[i].currentMessageTime);
			ImVec2 bottomRight = ImVec2(temp.x, temp.y);

			if (ImGui::IsMouseHoveringRect(ImVec2(width * 0.35f, (height * 0.425f) - 30.0f * _popUpMessages[i].currentMessageTime),bottomRight))
				_popUpMessages[i].hovered = true;
			else
				_popUpMessages[i].hovered = false;

			ImVec2 textDimensions = ImGui::CalcTextSize(_popUpMessages[i].message.c_str());

			ImGui::SetWindowFontScale(1.0f);
			ImGui::SetCursorPos(ImVec2((width * 0.3f - textDimensions.x) * 0.5f, (height * 0.15f - textDimensions.y) * 0.5f));
			ImGui::Text(_popUpMessages[i].message.c_str());
		}

		if (fadeIn || fadeOut || hovering) 
			ImGui::PopStyleVar();

		ImGui::End();	
	}
}

void LayerEditor::DrawWarningForAutomaticCompilation()
{
	ImGui::OpenPopup("Automatic compilation not enabled");
	if (ImGui::BeginPopupModal("Automatic compilation not enabled", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
	{
		ImGui::Text("WARNING: The Automatic Compilation is not enabled!");
		ImGui::Text("We recommend to enable this feature for a better workflow. Check the README file for instructions on how to set it up.");

		if (ImGui::Button("Close"))
			_showCompilationWarning = false;

		ImGui::EndPopup();
	}
}

void LayerEditor::DrawCompilationErrorWarning()
{
	ImGui::OpenPopup("Compilation failed");
	if (ImGui::BeginPopupModal("Compilation failed", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
	{
		ImGui::TextColored(ImVec4(1,0,0,1), "WARNING: Compilation of the Scripting project failed.");
		ImGui::Text("Last changes won't apply until compilation errors are solved.");
		ImGui::Text("Please check the Scripting solution and fix any compilation errors and try again.");

		if (ImGui::Button("Close"))
			_showCompilationError = false;

		ImGui::EndPopup();
	}
}
