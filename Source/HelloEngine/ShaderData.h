#pragma once

#include <string>

struct ShaderData
{
public:
	uint ID = 0;
	std::string name;
	
	std::string vertexCode;
	std::string fragmentCode;

	bool isIstanced = false;
	bool isBoned = false;
	bool hasEngineLight = false;
	bool hasUpdatedLights = false;
	bool hasUpdatedCamera = false;

	uint _maxPointLights = 0;
	uint _maxSpotLights = 0;
};