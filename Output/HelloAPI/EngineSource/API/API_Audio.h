#pragma once
#include "Globals.h"

namespace API
{
	static class TO_API Audio
	{
	public:
		static uint Event(const char* eventName);
		static uint Event(uint eventID);
		static void StopEvent(uint playID);
		static void SetGlobalGameParameter(const char* parameterName, float value);
	};
}
