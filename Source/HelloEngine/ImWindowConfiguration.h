#pragma once

#include <queue>

#include "ImWindow.h"

class Application;
class SceneCameraObject;

namespace Htool
{
	template<class T> class CycleArray;
}

using CArrayF = Htool::CycleArray<float>;

class ImWindowConfiguration :public ImWindow
{
public:

	ImWindowConfiguration();

	~ImWindowConfiguration();

	void Update() override;

public:
	std::map<std::string, std::pair<bool, int>> renderConfigs;

private:
	Application* _app = nullptr;

	// Need more testing, but i think is good
	Htool::CycleArray<float>* _frames = nullptr;

	bool _fullscreen = false;

	int _countCPU = 0;

	float _systemRAM = 0;

	// Mouse wheel
	int _mouseWheelInteger = 1;
	float _mouseWheelDecimal = 0.5f;
	float _mouseWheel = 10.5f;

	int _frameLimit = 60;

	const int* _windowWidth = nullptr;
	const int* _windowHeight = nullptr;

	float* _windowBrightness = nullptr;

	bool _isVSyncOn;

	bool _automaticCompilation = false;

	float* _sceneCameraSpeed = nullptr;

	SceneCameraObject* _sceneCamera = nullptr;

	ModuleRenderer3D* _moduleRenderer = nullptr;
};