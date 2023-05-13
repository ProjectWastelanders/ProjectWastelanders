#pragma once
#include "Module.h"
#include "CommandChangeValue.hpp"
#include "CommandArray.hpp"

#define MAX_UNDO 41 // Actually is 21-1 = 20;

typedef Htool::CommandArray<Command*> CommandArray;

class ModuleInput;
class GameObject;

class ModuleCommand :public Module
{
public:
	ModuleCommand();

	~ModuleCommand();

	UpdateStatus Update() override;

	/// <summary>
	/// 
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="variable">: variable you want to change</param>
	/// <param name="beginValue">: value before change</param>
	/// <param name="endValue">: value after change</param>
	template<class T>
	static void S_ChangeValue(T* variable, T beginValue, T endValue)
	{
		_commands->push(new CommandChangeValue<T>(variable, beginValue, endValue));
	}

	/// <summary>
	/// 
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="variable">: variable you want to change</param>
	/// <param name="beginValue">: value before change</param>
	/// <param name="endValue">: value after change</param>
	/// <param name="function">: if you want call some function when undo/redo for refresh values</param>
	template<class T>
	static void S_ChangeValue(T* variable, T beginValue, T endValue, std::function<void()> function)
	{
		_commands->push(new CommandChangeValue<T>(variable, beginValue, endValue, function));
	}

	static void S_ChangeTransform(float4x4& newGlobalMatrix, float4x4& currentGlobalMatrix, bool ignoreRotation, std::function<void(float4x4&, bool, bool)> function);

	static void S_DeleteGameObject(GameObject* gameobject);

	static void S_SetParentGameObject(GameObject* gameobject, GameObject* newParent);

	static void S_CleanCommandQueue();

private:
	bool Undo();

	bool Redo();

private:
	static CommandArray* _commands;
public:
	static bool _canUseCommand;

	friend class LayerGame;
};