#include "Headers.h"
#include "P_CubeShape.h"
#include "ParticleSystemComponent.h"
#include "GameObject.h"

P_CubeShape::P_CubeShape()
{
	type = P_ModuleType::SHAPE;
	shapeType = ShapeType::CUBE;

	c1 = 10;
	c2 = 10;
	c3 = 10;
}

P_CubeShape::~P_CubeShape()
{
}

void P_CubeShape::OnEditor()
{
	if (ImGui::CollapsingHeader("Cube Shape Emitter"))
	{
		ImGui::DragFloat("Side X", &c1);
		ImGui::DragFloat("Side Y", &c2);
		ImGui::DragFloat("Side Z", &c3);
	}
}

float3 P_CubeShape::GetRandomPos()
{
	center = component->GetGameObject()->transform->GetGlobalPosition();
	LCG random;

	float x = random.Float() * c1;
	float y = random.Float() * c2;
	float z = random.Float() * c3;

	x += center.x;
	y += center.y;
	z += center.z;

	return float3(x, y, z);
}

bool P_CubeShape::IsInside(float3 position)
{
	center = component->GetGameObject()->transform->GetGlobalPosition();

	float limitX = center.x + c1;
	float limitY = center.x + c1;
	float limitZ = center.z + c2;

	if (position.x <= limitX && position.y <= limitY && position.z <= limitZ)
	{
		return true;
	}

	return false;
}

void P_CubeShape::Serialization(json& j)
{
	j["ParticleModules"]["ShapeModule"]["C1"] = c1;
	j["ParticleModules"]["ShapeModule"]["C2"] = c2;
	j["ParticleModules"]["ShapeModule"]["C3"] = c3;
}

void P_CubeShape::DeSerialization(json& j)
{
	c1 = j["ParticleModules"]["ShapeModule"]["C1"];
	c2 = j["ParticleModules"]["ShapeModule"]["C2"];
	c3 = j["ParticleModules"]["ShapeModule"]["C3"];
}