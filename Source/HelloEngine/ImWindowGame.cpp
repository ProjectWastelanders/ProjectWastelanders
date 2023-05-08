#include "Headers.h"
#include "ImWindowGame.h"
#include "ModuleCamera3D.h"
#include "CameraObject.h"
#include "LayerGame.h"
#include "API/API_Engine.h"

ImWindowGame::ImWindowGame()
{
	windowName = "Game";
}

ImWindowGame::~ImWindowGame()
{
}

void ImWindowGame::Update()
{
	static bool isFocused = ImGui::IsWindowFocused();
	if(ImGui::Begin(windowName.c_str()))
	{
		isActive = true;
		if (Application::Instance()->camera->activeGameCamera != nullptr) 
		{
			ImVec2 gameDimensions = ImGui::GetContentRegionAvail();

			if (gameDimensions.x != gameWidth || gameDimensions.y != gameHeight)
			{
				// If the size of this imgui window is different from the one stored.
				gameWidth = gameDimensions.x;
				gameHeight = gameDimensions.y;
				gamePosX = ImGui::GetWindowPos().x;
				gamePosY = ImGui::GetWindowPos().y;
				Application::Instance()->camera->activeGameCamera->ChangeAspectRatio((float)gameWidth / (float)gameHeight);
			}

			if (Application::Instance()->camera->activeGameCamera->active)
			{
				ImGui::Image((ImTextureID)Application::Instance()->camera->activeGameCamera->frameBuffer.GetTexture(), ImGui::GetContentRegionAvail(), ImVec2(0, 1), ImVec2(1, 0));
			}
		}
	}
	else
	{
		isActive = false;
	}

	LayerGame::detectInput = isFocused;
	if (isFocused != ImGui::IsWindowFocused())
	{
		isFocused = !isFocused;
		if (LayerGame::_isPlaying)
		{
			if (!isFocused)
				API::Engine::UnApplyEngineProperties();
			else
				API::Engine::ApplyEngineProperties();
		}

	}
	if (ModuleInput::S_GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		API::Engine::UnApplyEngineProperties();
	}
	ImGui::End();
}
