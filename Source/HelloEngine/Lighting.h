#pragma once

#include "LightComponent.h"
#include "DirectionalLightComponent.h"
#include "PointLightComponent.h"
#include "SpotLightComponent.h"

struct LightMap
{
	DirectionalLight directionalLight;
	std::map<uint, PointLight> pointLight;
	std::map<uint, SpotLight> spotLight;

	uint shadowMap = 0;
};

class Lighting
{
public:
	Lighting();
	~Lighting();

	void Update();

	//uint AddLight(Light lightData);
	static uint AddSpotLight(SpotLight lightData);
	static uint AddPointLight(PointLight lightData);
	static void SetDirectionalLight(DirectionalLight lightData);
	
	static void RemoveLight(Component::Type type, uint _lightID);
	static void ClearLights();

	static LightMap& GetLightMap() { return _lightMap; }

private:
	bool _active;

	int ref;

	static LightMap _lightMap;
};

