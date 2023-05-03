#pragma once

#include <string>

class ImWindow
{
public:
	ImWindow() {};

	virtual ~ImWindow() {};

	virtual void Update() = 0;

	bool isEnabled = true;
	bool isActive = false;

	std::string windowName = "DefaultWindow";
};