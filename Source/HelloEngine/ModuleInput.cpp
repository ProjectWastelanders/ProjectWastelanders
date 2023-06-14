#include "Headers.h"
#include "Application.h"
#include "ModuleRenderer3D.h"

#include "imgui_impl_sdl.h"

#define MAX_KEYS 300

// Init static variables
KEY_STATE* ModuleInput::_keyboard = nullptr;
KEY_STATE ModuleInput::_mouse_buttons[MAX_MOUSE_BUTTONS];
int ModuleInput::_mouse_x = 0;
int ModuleInput::_mouse_y = 0;
int ModuleInput::_mouse_z = 0;
int ModuleInput::_mouse_x_motion = 0;
int ModuleInput::_mouse_y_motion = 0;
std::function<void(std::string)> ModuleInput::_dropEvent = nullptr;

// GamePad variables
int ModuleInput::maxGamePads = 0;
int ModuleInput::gamePadIndex = 0;
KEY_STATE ModuleInput::gamePadButtons[MAX_CONTROLLER_BUTTONS] = { KEY_STATE::KEY_DOWN };

SDL_GameControllerButton ModuleInput::sdlGamePadButtons[MAX_CONTROLLER_BUTTONS] = { SDL_CONTROLLER_BUTTON_A,SDL_CONTROLLER_BUTTON_B,SDL_CONTROLLER_BUTTON_X,SDL_CONTROLLER_BUTTON_Y,
						SDL_CONTROLLER_BUTTON_DPAD_UP, SDL_CONTROLLER_BUTTON_DPAD_DOWN, SDL_CONTROLLER_BUTTON_DPAD_LEFT, SDL_CONTROLLER_BUTTON_DPAD_RIGHT,
						SDL_CONTROLLER_BUTTON_LEFTSHOULDER, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER, SDL_CONTROLLER_BUTTON_START, SDL_CONTROLLER_BUTTON_BACK, SDL_CONTROLLER_BUTTON_GUIDE
	, SDL_CONTROLLER_BUTTON_LEFTSTICK, SDL_CONTROLLER_BUTTON_RIGHTSTICK };

SDL_GameController* ModuleInput::gamePadHandles[MAX_CONTROLLERS];

bool ModuleInput::_usingGamePad = false;

ModuleInput::ModuleInput(bool start_enabled) : Module(start_enabled)
{
	_keyboard = new KEY_STATE[MAX_KEYS];
	memset(_keyboard, KEY_IDLE, sizeof(KEY_STATE) * MAX_KEYS);
	memset(_mouse_buttons, KEY_IDLE, sizeof(KEY_STATE) * MAX_MOUSE_BUTTONS);
}

// Destructor
ModuleInput::~ModuleInput()
{
	RELEASE_ARRAY(_keyboard);
}

// Called before render is available
bool ModuleInput::Init()
{
	LOG("Init SDL input event system");

	bool ret = true;
	
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	
	if (SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER) < 0)
	{
		std::string error = SDL_GetError();
		Console::S_Log("SDL_INIT_GAMECONTROLLER could not initialize! SDL_Error: " + error, LogType::ERR);
		ret = false;
	}
	else
	{
		OpenController();
		SDL_JoystickEventState(SDL_ENABLE);
	}

	return ret;
}

// Called every draw update
UpdateStatus ModuleInput::PreUpdate()
{
	SDL_PumpEvents();

	const Uint8* keys = SDL_GetKeyboardState(NULL);
	
	for(int i = 0; i < MAX_KEYS; ++i)
	{
		if(keys[i] == 1)
		{
			_usingGamePad = false;
			if(_keyboard[i] == KEY_IDLE)
				_keyboard[i] = KEY_DOWN;
			else
				_keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if(_keyboard[i] == KEY_REPEAT || _keyboard[i] == KEY_DOWN)
				_keyboard[i] = KEY_UP;
			else
				_keyboard[i] = KEY_IDLE;
		}
	}

	Uint32 buttons = SDL_GetMouseState(&_mouse_x, &_mouse_y);

	_mouse_x /= SCREEN_SIZE;
	_mouse_y /= SCREEN_SIZE;
	_mouse_z = 0;

	for(int i = 0; i < 5; ++i)
	{
		if(buttons & SDL_BUTTON(i))
		{
			_usingGamePad = false;
			if(_mouse_buttons[i] == KEY_IDLE)
				_mouse_buttons[i] = KEY_DOWN;
			else
				_mouse_buttons[i] = KEY_REPEAT;
		}
		else
		{
			if(_mouse_buttons[i] == KEY_REPEAT || _mouse_buttons[i] == KEY_DOWN)
				_mouse_buttons[i] = KEY_UP;
			else
				_mouse_buttons[i] = KEY_IDLE;
		}
	}

	_mouse_x_motion = _mouse_y_motion = 0;

	if (gamePadHandles[gamePadIndex] != nullptr)
	{
		UpdateControllerInput();
		if (S_GetGamePadAxis(SDL_CONTROLLER_AXIS_LEFTX) > 10000 || S_GetGamePadAxis(SDL_CONTROLLER_AXIS_LEFTY) > 10000 ||
			S_GetGamePadAxis(SDL_CONTROLLER_AXIS_RIGHTX) > 10000 || S_GetGamePadAxis(SDL_CONTROLLER_AXIS_RIGHTY) > 10000) _usingGamePad = true;
	}

	bool quit = false;

	SDL_Event e;

	while(SDL_PollEvent(&e))
	{
#ifdef STANDALONE
		ImGui_ImplSDL2_ProcessEvent(&e);
#endif
		switch(e.type)
		{
			case SDL_MOUSEWHEEL:
			{
				_mouse_z = e.wheel.y;
			}
			break;
			case SDL_MOUSEMOTION:
			{
				_mouse_x = e.motion.x / SCREEN_SIZE;
				_mouse_y = e.motion.y / SCREEN_SIZE;

				_mouse_x_motion = e.motion.xrel / SCREEN_SIZE;
				_mouse_y_motion = e.motion.yrel / SCREEN_SIZE;
			}
			break;
			case SDL_QUIT:
			{
				quit = true;
			}
			break;
			case (SDL_DROPFILE):
			{
				if (_dropEvent) 
					_dropEvent(e.drop.file);
				
				// Free dropped_filedir memory
				SDL_free(e.drop.file);    
				break;
			}
			case SDL_WINDOWEVENT:
			{
				if(e.window.event == SDL_WINDOWEVENT_RESIZED)
					app->renderer3D->OnResize(e.window.data1, e.window.data2);
				break;
			}
			case SDL_CONTROLLERDEVICEADDED:
				OpenController();
				break;
		}
	}

	if(quit == true)
		return UpdateStatus::UPDATE_STOP;

	return UpdateStatus::UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	LOG("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}

void ModuleInput::S_AddOnDropListener(std::function<void(std::string)> func)
{
	_dropEvent = func;
}

void ModuleInput::S_ClearOnDropListener()
{
	_dropEvent = nullptr;
}

void ModuleInput::S_ResetFrameInput()
{
	for (int i = 0; i < MAX_CONTROLLER_BUTTONS; ++i)
	{
		gamePadButtons[i] = KEY_STATE::KEY_IDLE;
	}
}

void ModuleInput::OpenController()
{
	maxGamePads = SDL_NumJoysticks();
	gamePadIndex = 0;
	for (int JoystickIndex = 0; JoystickIndex < maxGamePads; ++JoystickIndex)
	{
		if (!SDL_IsGameController(JoystickIndex))
		{
			continue;
		}
		if (gamePadIndex >= MAX_CONTROLLERS)
		{
			break;
		}
		gamePadHandles[gamePadIndex] = SDL_GameControllerOpen(JoystickIndex);
		_usingGamePad = true;
		break;
	}
}

void ModuleInput::UpdateControllerInput()
{
	if (gamePadHandles[gamePadIndex] == nullptr) return;

	// Temporary button state (only gives if it's pressed or not)
	bool buttons[MAX_CONTROLLER_BUTTONS];

	for (int i = 0; i < MAX_CONTROLLER_BUTTONS; i++)
	{
		// Get Button State
		buttons[i] = SDL_GameControllerGetButton(gamePadHandles[gamePadIndex], sdlGamePadButtons[i]);

		// If button is pressed
		if (buttons[i] == true)
		{
			_usingGamePad = true;
			if (gamePadButtons[i] == KEY_IDLE)
				gamePadButtons[i] = KEY_DOWN; // KEY_DOWN if not pressed on previous frame
			else
				gamePadButtons[i] = KEY_REPEAT;// KEY_REPEAT if pressed on previous frame
		}
		else
		{
			if (gamePadButtons[i] == KEY_REPEAT || gamePadButtons[i] == KEY_DOWN || gamePadButtons[i] == KEY_HANDLED)
				gamePadButtons[i] = KEY_UP; // KEY_UP if pressed on previous frame
			else
				gamePadButtons[i] = KEY_IDLE; // KEY_IDLE if not pressed on previous frame
		}
	}
}
