#include "Headers.h"
#include "P_ConeShape.h"
#include "ParticleSystemComponent.h"
#include "GameObject.h"

P_ConeShape::P_ConeShape()
{
	type = P_ModuleType::SHAPE;
	shapeType = ShapeType::CONE;

	radius = 10;
	height = 20;
	
	dir = 2;
	invert = false;
}

P_ConeShape::~P_ConeShape()
{
}

void P_ConeShape::OnEditor()
{
	if (ImGui::CollapsingHeader("Cone Shape Emitter"))
	{
		ImGui::Text("Cone Direction");
		ImGui::SameLine();
		ImGui::Spacing();
		ImGui::Checkbox("Invert", &invert);
		if (ImGui::Button(" X ")) {
			dir = 1;
		}
		ImGui::SameLine();
		if (ImGui::Button(" Y ")) {
				dir = 2;
		}
		ImGui::SameLine();
		if (ImGui::Button(" Z ")) {
				dir = 3;
		}
		ImGui::DragFloat("Radius", &radius);
		ImGui::DragFloat("Height", &height);
	}
}

float3 P_ConeShape::GetRandomPos()
{
	center = component->GetGameObject()->transform->GetGlobalPosition();

	LCG random;

	float h = height * random.Float(0.0f, 1.0f);
	float r = (radius / height) * h * sqrt(random.Float(0.0f, 1.0f));
	float angle = 2 * math::pi * random.Float(0.0f, 1.0f);


	float x = r * cos(angle);
	float y;
	if (invert) {
		y = -h;
	}
	else {
		y = h;
	}
	float z = r * sin(angle);
	
	//x += center.x;
	//y += center.y;
	//z += center.z;

	switch (dir)
	{
	case 1:
	{
		x += center.y;
		y += center.x;
		z += center.z;
		
		return float3(y, x, z);
		
	}		
		break;
	case 2:
	{
		x += center.x;
		y += center.y;
		z += center.z;
		
		return float3(x, y, z);
	}		
		break;
	case 3:
	{
		x += center.x;
		y += center.z;
		z += center.y;

		return float3(x, z, y);
	}		
		break;
	}

	//return float3(x, y, z);
}

bool P_ConeShape::IsInside(float3 position)
{
	//center = component->GetGameObject()->transform->GetGlobalPosition();

	/*float limitX = center.x + c1;
	float limitY = center.x + c1;
	float limitZ = center.z + c2;

	if (position.x <= limitX && position.y <= limitY && position.z <= limitZ)
	{
		return true;
	}*/

	return true;
}