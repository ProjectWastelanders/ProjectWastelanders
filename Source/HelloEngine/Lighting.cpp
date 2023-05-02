#include "Headers.h"
#include "Lighting.h"

LightMap Lighting::_lightMap;

Lighting::Lighting()
{
	_active = true;
	ref = -1;
}

Lighting::~Lighting()
{

}

void Lighting::SetDirectionalLight(DirectionalLight lightData)
{
	_lightMap.directionalLight = lightData;
}

uint Lighting::AddPointLight(PointLight lightData)
{
	uint _lightID = 0;

	_lightID = HelloUUID::GenerateUUID();
	_lightMap.pointLight[_lightID] = lightData;

	return _lightID;
}

uint Lighting::AddSpotLight(SpotLight lightData)
{
	uint _lightID = 0;

	_lightID = HelloUUID::GenerateUUID();
	_lightMap.spotLight[_lightID] = lightData;

	return _lightID;
}

void Lighting::RemoveLight(Component::Type type, uint _lightID)
{
	switch (type)
	{
		case Component::Type::DIRECTIONAL_LIGHT:
			//_lightMap.directionalLight = (DirectionalLight) lightData;
			break;
		case Component::Type::POINT_LIGHT:
			_lightMap.pointLight.erase(_lightID);
			break;
		case Component::Type::SPOT_LIGHT:
			_lightMap.spotLight.erase(_lightID);
			break;
	}
}

void Lighting::ClearLights()
{
	_lightMap.pointLight.clear();
	_lightMap.spotLight.clear();
	_lightMap.directionalLight = DirectionalLight();
}

void Lighting::Update()
{
	if (!_active) return;

	//float pos[] = { global.position.x, global.position.y, global.position.z, 1.0f };
	//glLightfv(ref, GL_POSITION, pos);
}