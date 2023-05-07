#pragma once

// ----------------------------- C++ -----------------------------

#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <string>
#include <set>
#include <list>
#include <fstream>

// ----------------------------- External -----------------------------

#include "glew.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include "MathGeoLib.h"

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"

#include "SDL.h"

#include "pugixml.hpp"

#include "json.hpp"
#include "physfs.h"

#ifdef STANDALONE
#include "optick.h"
#else
#define OPTICK_EVENT()
#endif

// ----------------------------- Engine -----------------------------
#include "Globals.h"
#include "Application.h"

#include "QuickSave.h" 
#include "Console.h"
#include "ImGuiUtils.h"
#include "UUID.h"
#include "EngineTime.h"

//TODO: Add every Module in here once Modules have been tested and won't be changed anymore.

#include "ModuleInput.h"
#include "ModuleWindow.h"

using namespace Engine;