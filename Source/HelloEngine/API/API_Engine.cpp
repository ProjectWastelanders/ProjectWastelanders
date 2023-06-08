#include "Headers.h"
#include "API_Engine.h"
#include "ModuleWindow.h"
#include "LayerGame.h"
#include "ModuleRenderer3D.h"

bool API::Engine::hideMouse = false;
bool API::Engine::centerMouse = false;

void API::Engine::ApplyEngineProperties()
{
	// Mouse hide
	if (hideMouse)
		SDL_ShowCursor(0);
	else
		SDL_ShowCursor(1);
	SDL_SetRelativeMouseMode((SDL_bool)centerMouse);
}

void API::Engine::SetVSync(bool active)
{
	Application::Instance()->renderer3D->ToggleVSync(active);
}

void API::Engine::EnginePropertiesUpdate()
{

}

void API::Engine::UnApplyEngineProperties()
{
	// Mouse hide
	SDL_ShowCursor(1);
	SDL_SetRelativeMouseMode(SDL_bool::SDL_FALSE);
}
