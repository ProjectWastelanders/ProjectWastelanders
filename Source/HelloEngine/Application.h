#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "Timer.hpp"
#include <vector>

class Module;
class ModuleWindow;
class ModuleInput;
class ModuleCamera3D;
class ModuleRenderer3D;
class ModuleLayers;
class ModuleXML;
class ModuleFiles;
class ModuleCommand;
class ModuleResourceManager;
class ModulePhysics;
class ModuleAudio;
class ModuleNavMesh;

#define DEVELOPMENT

class Application
{
public:
	Application();
	~Application();

	bool Init();
	UpdateStatus Update();
	bool CleanUp();

	static Application* Instance();

	void SetFPS(int maxFrames)
	{
		fps = 1.0f / (float)maxFrames;
		frameCap = maxFrames;
	}

	float GetDeltaTime() 
	{
		return _dt;
	}

	bool IsExit()
	{
		return _isExiting;
	}

	void Exit()
	{
		_isExiting = true;
	}

	std::string GetEngineVersion();

private:

	void AddModule(Module* mod);

public:
	ModuleWindow* window = nullptr;
	ModuleInput* input = nullptr;
	ModuleCamera3D* camera = nullptr;
	ModuleRenderer3D* renderer3D = nullptr;
	ModuleLayers* layers = nullptr;
	ModuleXML* xml = nullptr;
	ModuleFiles* file = nullptr;
	ModuleCommand* command = nullptr;
	ModuleResourceManager* resource = nullptr;
	ModulePhysics* physic = nullptr;
	ModuleAudio* audio = nullptr;
	ModuleNavMesh* navMesh = nullptr;

	float fps = 1.0f/60.0f;
	int frameCap = 60;
	Timer	timer;

private:
	float _dt = 0;
	std::vector<Module*> _list_modules;
	static Application* _app;
	bool _isExiting = false;
};
#endif // !__APPLICATION_H__