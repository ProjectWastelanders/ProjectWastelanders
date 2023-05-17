#ifndef __MODULEINPUT_H__
#define __MODULEINPUT_H__

#include "Module.h"
#include "Globals.h"

#define MAX_MOUSE_BUTTONS 5
#define MAX_CONTROLLERS 1
#define MAX_CONTROLLER_BUTTONS 15

enum KEY_STATE
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP,
	KEY_HANDLED,
	KEY_ERROR
};

enum GamePad
{
	BUTTON_A,
	BUTTON_B,
	BUTTON_X,
	BUTTON_Y,
	BUTTON_UP,
	BUTTON_DOWN,
	BUTTON_LEFT,
	BUTTON_RIGHT,
	BUTTON_LEFT_SHOULDER,
	BUTTON_RIGHT_SHOULDER,
	BUTTON_START,
	BUTTON_BACK,
	BUTTON_SELECT,
	BUTTON_LEFT_STICK,
	BUTTON_RIGHT_STICK
};

class ModuleInput : public Module
{
public:
	ModuleInput(bool start_enabled = true);

	~ModuleInput();

	bool Init();

	UpdateStatus PreUpdate();

	bool CleanUp();

	static KEY_STATE S_GetKey(int id)
	{
		return _keyboard[id];
	}

	static KEY_STATE S_GetMouseButton(int id)
	{
		return _mouse_buttons[id];
	}

	static KEY_STATE S_GetGamePadButton(GamePad id) 
	{
		// Game Controller Input handler
		if (gamePadHandles[gamePadIndex] == nullptr) return KEY_STATE::KEY_ERROR;
		return gamePadButtons[id];
	}

	static void S_HandleMouseButton(int id)
	{
		_mouse_buttons[id] = KEY_STATE::KEY_IDLE;
	}

	static void S_HandleGamePadButton(GamePad id)
	{
		gamePadButtons[id] = KEY_STATE::KEY_HANDLED;
	}

	static int S_GetMouseX()
	{
		return _mouse_x;
	}

	static int S_GetMouseY()
	{
		return _mouse_y;
	}

	static int S_GetMouseZ()
	{
		return _mouse_z;
	}

	static int S_GetMouseXMotion()
	{
		return _mouse_x_motion;
	}

	static int S_GetMouseYMotion()
	{
		return _mouse_y_motion;
	}

	/// Returns a number between -32768 and 32768 for joystics and 0 to 32768 for triggers.
	static int S_GetGamePadAxis(SDL_GameControllerAxis id) 
	{
		if (gamePadHandles[gamePadIndex] != nullptr)
			return SDL_GameControllerGetAxis(gamePadHandles[gamePadIndex], id);
		return 0;
	}

	static void S_AddOnDropListener(std::function<void(std::string)> func);

	static void S_ClearOnDropListener();

	static bool S_IsUsingController() { return _usingGamePad; }

	static void S_ResetFrameInput();

private:

	static void OpenController();

	static void UpdateControllerInput();

	static KEY_STATE* _keyboard;

	static KEY_STATE _mouse_buttons[MAX_MOUSE_BUTTONS];

	static int _mouse_x;

	static int _mouse_y;

	static int _mouse_z;

	static int _mouse_x_motion;

	static int _mouse_y_motion;

	static std::function<void(std::string)> _dropEvent;

	// Joysticks Controller variables
	static KEY_STATE gamePadButtons[MAX_CONTROLLER_BUTTONS];
	static int maxGamePads;
	static int gamePadIndex;

	static SDL_GameControllerButton sdlGamePadButtons[MAX_CONTROLLER_BUTTONS];

	static SDL_GameController* gamePadHandles[MAX_CONTROLLERS];

	static bool _usingGamePad;
};

#endif // !__MODULEINPUT_H__