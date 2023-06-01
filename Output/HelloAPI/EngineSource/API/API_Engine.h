#pragma once
#include "Globals.h"

class ImWindowGame;

namespace API
{
	static class TO_API Engine
	{
	public:
		static bool hideMouse;
		static bool centerMouse;

		static void SetVSync(bool active);

	private:
		static void EnginePropertiesUpdate();
		static void ApplyEngineProperties();
		static void UnApplyEngineProperties();

		friend class LayerGame;
		friend class ImWindowGame;
	};
}