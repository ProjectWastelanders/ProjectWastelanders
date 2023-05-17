#include "Headers.h"
#include "P_RectangleShape.h"
#include "ParticleSystemComponent.h"
#include "GameObject.h"

P_RectangleShape::P_RectangleShape()
{
	type = P_ModuleType::SHAPE;
	shapeType = ShapeType::RECTANGLE;

	c1 = 10;
	c2 = 10;
}

P_RectangleShape::~P_RectangleShape()
{
}

void P_RectangleShape::OnEditor()
{
	if (ImGui::CollapsingHeader("Rectangle Shape Emitter"))
	{
		ImGui::DragFloat("Side X", &c1);
		ImGui::DragFloat("Side Z", &c2);
	}
}

float3 P_RectangleShape::GetRandomPos()
{
	center = component->GetGameObject()->transform->GetGlobalPosition();
	LCG random;

	float x = random.Float() * c1;
	float y = 0;
	float z = random.Float() * c2;

	x += center.x;
	z += center.z;

	return float3(x, y, z);
}

bool P_RectangleShape::IsInside(float3 position)
{
	center = component->GetGameObject()->transform->GetGlobalPosition();

	float limitX = center.x + c1;
	float limitZ = center.z + c2;

	if (position.x <= limitX && position.z <= limitZ)
	{
		return true;
	}

	return false;
}

void P_RectangleShape::Serialization(json& j)
{
	j["ParticleModules"]["ShapeModule"]["C1"] = c1;
	j["ParticleModules"]["ShapeModule"]["C1"] = c2;
}

void P_RectangleShape::DeSerialization(json& j)
{
	c1 = j["ParticleModules"]["ShapeModule"]["C1"];
	c2 = j["ParticleModules"]["ShapeModule"]["C2"];
}
