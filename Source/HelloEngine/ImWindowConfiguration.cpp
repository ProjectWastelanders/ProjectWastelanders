#include "Headers.h"
#include "ImWindowConfiguration.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "ModuleFiles.h"
#include "CycleArray.hpp"
#include "ModuleCamera3D.h"
#include "ModuleAudio.h"

#include "Lighting.h"

ImWindowConfiguration::ImWindowConfiguration() : ImWindow()
{
	windowName = "Configuration";

	isEnabled = true;

	_frames = new CArrayF(60, 0.0f);

	_app = Application::Instance();

	_countCPU = SDL_GetCPUCount();

	_systemRAM = SDL_GetSystemRAM();

	_windowWidth = &ModuleWindow::width;

	_windowHeight = &ModuleWindow::height;

	_windowBrightness = &ModuleWindow::brightness;

	_isVSyncOn = _app->renderer3D->isVSync;

	_frameLimit = _app->frameCap;

	_automaticCompilation = ModuleFiles::S_IsMSBuildOn();

	// Init render configurations
	_moduleRenderer = Application::Instance()->renderer3D;

	_sceneCameraSpeed = &Application::Instance()->camera->sceneCamera->cameraSpeed;

	// Get openGl node with module xml
	XMLNode openGlNode = Application::Instance()->xml->GetConfigXML().FindChildBreadth("openGL");

	// Init setting values with node context
	for (pugi::xml_node n = openGlNode.node.first_child(); !n.empty(); n = n.next_sibling())
	{
		n.name();
		renderConfigs.insert(
			std::make_pair(
				n.name(), // map key -> conifg name
				std::make_pair(	// map value -> pair<value,tag>
					n.attribute("value").as_bool(false),	// config value
					n.attribute("tag").as_int(0))));	// config tag
	}

	XMLNode inputNode = Application::Instance()->xml->GetConfigXML().FindChildBreadth("input");

	std::string nodeName = inputNode.node.name();

	if(nodeName == "input")
	{
		_mouseWheel = inputNode.node.child("mouseWheel").attribute("value").as_float(1.0f);

		_sceneCamera = Application::Instance()->camera->sceneCamera;

		_sceneCamera->SetWheelSpeed(_mouseWheel);

		_mouseWheelDecimal = _mouseWheel - std::floor(_mouseWheel);

		_mouseWheelInteger = std::floor(_mouseWheel);
	}
	else
	{
		_mouseWheelInteger = 0;

		_mouseWheelDecimal = 0.5f;

		_mouseWheel = 0.5f;

		XMLNode config = Application::Instance()->xml->GetConfigXML();

		pugi::xml_node node =  config.node.append_child("input").append_child("mouseWheel");

		node.append_attribute("value").set_value(0.5f);

		config.Save();
	}
}

ImWindowConfiguration::~ImWindowConfiguration()
{
	RELEASE(_frames);

	// Save values to the xml file
	XMLNode openGlNode = Application::Instance()->xml->GetConfigXML().FindChildBreadth("openGL");

	for (pugi::xml_node n = openGlNode.node.first_child(); !n.empty(); n = n.next_sibling())
		n.attribute("value").set_value(renderConfigs[n.name()].first);

	openGlNode.Save();

	// Save input setting
	XMLNode inputNode = Application::Instance()->xml->GetConfigXML().FindChildBreadth("input");

	inputNode.node.child("mouseWheel").attribute("value").set_value(_mouseWheel);

	inputNode.Save();
}

void ImWindowConfiguration::Update()
{
	std::string framerate = "Framerate: " + std::to_string(ImGui::GetIO().Framerate);

	_frames->push_back(ImGui::GetIO().Framerate);

	if (ImGui::Begin(windowName.c_str(), &isEnabled, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize))
	{
		if (ImGui::CollapsingHeader("Application", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::PlotHistogram("##Framerate", _frames->front(), _frames->size(), 0, framerate.c_str(), 0.0f, 160.0f, ImVec2(300, 160));
			if (ImGui::SliderInt("FPS Limit", &_frameLimit, 30, 120))
			{
				_app->SetFPS(_frameLimit);
			}
			ImGui::Separator();
			if (ModuleFiles::S_IsMSBuildOn())
			{
				if (ImGui::Checkbox("Automatic Compilation", &_automaticCompilation))
					ModuleFiles::S_SetAutomaticCompilation(_automaticCompilation);
			}
			ImGui::SliderFloat("Scene camera speed", _sceneCameraSpeed, 0.0f, 1000.0f);
			
			GameObject* defaultListener = ModuleAudio::defaultListenerGameObject;
			std::string listenerName = defaultListener == nullptr ? "None, add a Camera component." : defaultListener->GetName();
			ImGui::Text(("Current listener: " + listenerName).c_str());
		}

		if (ImGui::CollapsingHeader("Window", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::TextWrapped("<---Window Size--->");
			ImGui::TextWrapped("Width:"); ImGui::SameLine();
			ImGui::TextColored(ImVec4(255, 255, 0, 255), std::to_string(*_windowWidth).c_str());

			ImGui::TextWrapped("Height:"); ImGui::SameLine();
			ImGui::TextColored(ImVec4(255, 255, 0, 255), std::to_string(*_windowHeight).c_str()); ImGui::SameLine();

			ImGui::HelpMarker("Shows Window Width and Height");

			if (ImGui::SliderFloat("Brightness", _windowBrightness, 0.2f, 1.0f))
			{
				ModuleWindow::S_SetBrightness(*_windowBrightness);
			}

			ImGui::Checkbox("VSync", &_isVSyncOn);
			_app->renderer3D->ToggleVSync(_isVSyncOn);	
		}

		if (ImGui::CollapsingHeader("Game Time", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::TextWrapped("<---Time variables--->");

			ImGui::TextWrapped("Real time delta time: "); ImGui::SameLine();
			ImGui::TextColored(ImVec4(255, 255, 0, 255), "%.4f", EngineTime::RealTimeDeltaTime()); 
			ImGui::TextWrapped("Real time total time: "); ImGui::SameLine();
			ImGui::TextColored(ImVec4(255, 255, 0, 255), "%.4f", EngineTime::GameTimeInRealTimeCount());

			ImGui::TextWrapped("Game delta time: "); ImGui::SameLine();
			ImGui::TextColored(ImVec4(255, 255, 0, 255), "%.4f", EngineTime::GameDeltaTime());
			ImGui::TextWrapped("Game total time: "); ImGui::SameLine();
			ImGui::TextColored(ImVec4(255, 255, 0, 255), "%.4f", EngineTime::GameTimeCount());
			ImGui::TextWrapped("Game frame count: "); ImGui::SameLine();
			ImGui::TextColored(ImVec4(255, 255, 0, 255), "%d", EngineTime::GameFrameCount());

			float tempTimeScale = EngineTime::GetTimeScale();
			if (ImGui::DragFloat("Time scale", &tempTimeScale, 0.05f))
				EngineTime::SetTimeScale(tempTimeScale);
		}

		if (ImGui::CollapsingHeader("Input", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::TextWrapped("<---Mouse Input--->");

			ImGui::TextWrapped("Mouse Position: x = "); ImGui::SameLine();
			ImGui::TextColored(ImVec4(255, 255, 0, 255), "%d", ModuleInput::S_GetMouseX()); ImGui::SameLine();
			ImGui::TextWrapped(" y = "); ImGui::SameLine();
			ImGui::TextColored(ImVec4(255, 255, 0, 255), "%d", ModuleInput::S_GetMouseY());

			ImGui::Separator();

			ImGui::TextWrapped("Mouse Wheel Speed: "); ImGui::SameLine();
			ImGui::TextColored(ImVec4(255, 255, 0, 255), "%.4f", _mouseWheel);

			if (ImGui::SliderInt("Mouse Wheel Speed Int", &_mouseWheelInteger, 0, 20))
			{
				_mouseWheel = _mouseWheelInteger + _mouseWheelDecimal;

				if (_sceneCamera)
					_sceneCamera->SetWheelSpeed(_mouseWheel);
			}
			if (ImGui::SliderFloat("Mouse Wheel Speed Float", &_mouseWheelDecimal, 0, 1))
			{
				_mouseWheel = _mouseWheelInteger + _mouseWheelDecimal;

				if (_sceneCamera)
					_sceneCamera->SetWheelSpeed(_mouseWheel);
			}

			if (ImGui::CollapsingHeader("GamePad Input"))
			{
				ImGui::TextWrapped("Button A pressed: "); ImGui::SameLine();
				ImGui::TextColored(ImVec4(255, 255, 0, 255), ModuleInput::S_GetGamePadButton(GamePad::BUTTON_A) == KEY_STATE::KEY_ERROR ||
					ModuleInput::S_GetGamePadButton(GamePad::BUTTON_A) == KEY_STATE::KEY_IDLE ? "Released" : "Pressed");

				ImGui::TextWrapped("Button B pressed: "); ImGui::SameLine();
				ImGui::TextColored(ImVec4(255, 255, 0, 255), ModuleInput::S_GetGamePadButton(GamePad::BUTTON_B) == KEY_STATE::KEY_ERROR ||
					ModuleInput::S_GetGamePadButton(GamePad::BUTTON_B) == KEY_STATE::KEY_IDLE ? "Released" : "Pressed");

				ImGui::TextWrapped("Button X pressed: "); ImGui::SameLine();
				ImGui::TextColored(ImVec4(255, 255, 0, 255), ModuleInput::S_GetGamePadButton(GamePad::BUTTON_X) == KEY_STATE::KEY_ERROR ||
					ModuleInput::S_GetGamePadButton(GamePad::BUTTON_X) == KEY_STATE::KEY_IDLE ? "Released" : "Pressed");

				ImGui::TextWrapped("Button Y pressed: "); ImGui::SameLine();
				ImGui::TextColored(ImVec4(255, 255, 0, 255), ModuleInput::S_GetGamePadButton(GamePad::BUTTON_Y) == KEY_STATE::KEY_ERROR ||
					ModuleInput::S_GetGamePadButton(GamePad::BUTTON_Y) == KEY_STATE::KEY_IDLE ? "Released" : "Pressed");

				ImGui::TextWrapped("Button Up pressed: "); ImGui::SameLine();
				ImGui::TextColored(ImVec4(255, 255, 0, 255), ModuleInput::S_GetGamePadButton(GamePad::BUTTON_UP) == KEY_STATE::KEY_ERROR ||
					ModuleInput::S_GetGamePadButton(GamePad::BUTTON_UP) == KEY_STATE::KEY_IDLE ? "Released" : "Pressed");

				ImGui::TextWrapped("Button Down pressed: "); ImGui::SameLine();
				ImGui::TextColored(ImVec4(255, 255, 0, 255), ModuleInput::S_GetGamePadButton(GamePad::BUTTON_DOWN) == KEY_STATE::KEY_ERROR ||
					ModuleInput::S_GetGamePadButton(GamePad::BUTTON_DOWN) == KEY_STATE::KEY_IDLE ? "Released" : "Pressed");

				ImGui::TextWrapped("Button Left pressed: "); ImGui::SameLine();
				ImGui::TextColored(ImVec4(255, 255, 0, 255), ModuleInput::S_GetGamePadButton(GamePad::BUTTON_LEFT) == KEY_STATE::KEY_ERROR ||
					ModuleInput::S_GetGamePadButton(GamePad::BUTTON_LEFT) == KEY_STATE::KEY_IDLE ? "Released" : "Pressed");

				ImGui::TextWrapped("Button Right pressed: "); ImGui::SameLine();
				ImGui::TextColored(ImVec4(255, 255, 0, 255), ModuleInput::S_GetGamePadButton(GamePad::BUTTON_RIGHT) == KEY_STATE::KEY_ERROR ||
					ModuleInput::S_GetGamePadButton(GamePad::BUTTON_RIGHT) == KEY_STATE::KEY_IDLE ? "Released" : "Pressed");

				ImGui::TextWrapped("Button Left Shoulder pressed: "); ImGui::SameLine();
				ImGui::TextColored(ImVec4(255, 255, 0, 255), ModuleInput::S_GetGamePadButton(GamePad::BUTTON_LEFT_SHOULDER) == KEY_STATE::KEY_ERROR ||
					ModuleInput::S_GetGamePadButton(GamePad::BUTTON_LEFT_SHOULDER) == KEY_STATE::KEY_IDLE ? "Released" : "Pressed");

				ImGui::TextWrapped("Button Right Shoulder pressed: "); ImGui::SameLine();
				ImGui::TextColored(ImVec4(255, 255, 0, 255), ModuleInput::S_GetGamePadButton(GamePad::BUTTON_RIGHT_SHOULDER) == KEY_STATE::KEY_ERROR ||
					ModuleInput::S_GetGamePadButton(GamePad::BUTTON_RIGHT_SHOULDER) == KEY_STATE::KEY_IDLE ? "Released" : "Pressed");

				ImGui::TextWrapped("Button Start pressed: "); ImGui::SameLine();
				ImGui::TextColored(ImVec4(255, 255, 0, 255), ModuleInput::S_GetGamePadButton(GamePad::BUTTON_START) == KEY_STATE::KEY_ERROR ||
					ModuleInput::S_GetGamePadButton(GamePad::BUTTON_START) == KEY_STATE::KEY_IDLE ? "Released" : "Pressed");

				ImGui::TextWrapped("Button Back pressed: "); ImGui::SameLine();
				ImGui::TextColored(ImVec4(255, 255, 0, 255), ModuleInput::S_GetGamePadButton(GamePad::BUTTON_BACK) == KEY_STATE::KEY_ERROR ||
					ModuleInput::S_GetGamePadButton(GamePad::BUTTON_BACK) == KEY_STATE::KEY_IDLE ? "Released" : "Pressed");

				ImGui::TextWrapped("Button Select pressed: "); ImGui::SameLine();
				ImGui::TextColored(ImVec4(255, 255, 0, 255), ModuleInput::S_GetGamePadButton(GamePad::BUTTON_SELECT) == KEY_STATE::KEY_ERROR ||
					ModuleInput::S_GetGamePadButton(GamePad::BUTTON_SELECT) == KEY_STATE::KEY_IDLE ? "Released" : "Pressed");

				ImGui::TextWrapped("Button Left Stick pressed: "); ImGui::SameLine();
				ImGui::TextColored(ImVec4(255, 255, 0, 255), ModuleInput::S_GetGamePadButton(GamePad::BUTTON_LEFT_STICK) == KEY_STATE::KEY_ERROR ||
					ModuleInput::S_GetGamePadButton(GamePad::BUTTON_LEFT_STICK) == KEY_STATE::KEY_IDLE ? "Released" : "Pressed");

				ImGui::TextWrapped("Button Right Stick pressed: "); ImGui::SameLine();
				ImGui::TextColored(ImVec4(255, 255, 0, 255), ModuleInput::S_GetGamePadButton(GamePad::BUTTON_RIGHT_STICK) == KEY_STATE::KEY_ERROR ||
					ModuleInput::S_GetGamePadButton(GamePad::BUTTON_RIGHT_STICK) == KEY_STATE::KEY_IDLE ? "Released" : "Pressed");

				ImGui::TextWrapped("LeftJoystick Horizontal Axis: "); ImGui::SameLine();
				ImGui::TextColored(ImVec4(255, 255, 0, 255), "%d", ModuleInput::S_GetGamePadAxis(SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_LEFTX));

				ImGui::TextWrapped("LeftJoystick Vertical Axis: "); ImGui::SameLine();
				ImGui::TextColored(ImVec4(255, 255, 0, 255), "%d", ModuleInput::S_GetGamePadAxis(SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_LEFTY));

				ImGui::TextWrapped("RightJoystick Horizontal Axis: "); ImGui::SameLine();
				ImGui::TextColored(ImVec4(255, 255, 0, 255), "%d", ModuleInput::S_GetGamePadAxis(SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_RIGHTX));

				ImGui::TextWrapped("RightJoystick Vertical Axis: "); ImGui::SameLine();
				ImGui::TextColored(ImVec4(255, 255, 0, 255), "%d", ModuleInput::S_GetGamePadAxis(SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_RIGHTY));

				ImGui::TextWrapped("Left Trigger Axis: "); ImGui::SameLine();
				ImGui::TextColored(ImVec4(255, 255, 0, 255), "%d", ModuleInput::S_GetGamePadAxis(SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_TRIGGERLEFT));

				ImGui::TextWrapped("Right Trigger Axis: "); ImGui::SameLine();
				ImGui::TextColored(ImVec4(255, 255, 0, 255), "%d", ModuleInput::S_GetGamePadAxis(SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_TRIGGERRIGHT));
			}
		}

		if(ImGui::CollapsingHeader("Render", ImGuiTreeNodeFlags_DefaultOpen))
		{
			for (auto& iter : renderConfigs)
			{
				ImGui::Checkbox(iter.first.c_str(), &iter.second.first);
				if (iter.first == "wireframe") 
					_moduleRenderer->ToggleOpenGLWireframe(iter.second.first);
				else if (iter.first == "shadows") 
					_moduleRenderer->SetShadows(iter.second.first);
				else 
					_moduleRenderer->ToggleOpenGLSystem(iter.second.first, iter.second.second);
			}

			ImGui::Checkbox("Max Render Distance On Inspector", &_app->renderer3D->hasMaxRenderDistanceOnInspector);
			ImGui::DragFloat("Max Render Distance", &_app->renderer3D->maxRenderDistance, 1.0f, 0.0f);

			ImGui::Checkbox("Show Colliders", &_app->renderer3D->isRenderingColliders);
		}

		if (ImGui::CollapsingHeader("Hardware", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::TextWrapped("CPU Count: "); ImGui::SameLine();
			ImGui::TextColored(ImVec4(255, 255, 0, 255), std::to_string(_countCPU).c_str());

			ImGui::TextWrapped("RAM: "); ImGui::SameLine();
			ImGui::TextColored(ImVec4(255, 255, 0, 255), std::to_string(_systemRAM).c_str());

			ImGui::Separator();
			ImGui::TextWrapped("Vendor %s", glGetString(GL_VENDOR));
			ImGui::TextWrapped("Renderer: %s", glGetString(GL_RENDERER));
			ImGui::TextWrapped("OpenGL version supported %s", glGetString(GL_VERSION));
			ImGui::TextWrapped("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

		}
	}
	ImGui::End();
}