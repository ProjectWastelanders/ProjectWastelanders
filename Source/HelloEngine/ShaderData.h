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

	uint _maxPointLights = 32;
	uint _maxSpotLights = 32;
};