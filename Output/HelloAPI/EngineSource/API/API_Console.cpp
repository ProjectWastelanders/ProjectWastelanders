#include "Headers.h"
#include "API_Console.h"

//void API::Console::Log(const API::API_Vector2& vec, API::Console::MessageType type)
//{
//	Engine::Console::S_Log(vec, (LogType)type);
//}

void API::Console::Log(const API::API_Vector3& vec, API::Console::MessageType type)
{
#ifdef STANDALONE
	Engine::Console::S_Log(vec, (LogType)type);
#endif
}

void API::Console::Log(const std::string& text, API::Console::MessageType type)
{
#ifdef STANDALONE
	Engine::Console::S_Log(text, (LogType)type);
#endif
}
