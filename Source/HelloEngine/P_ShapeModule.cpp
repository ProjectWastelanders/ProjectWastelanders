#include "Headers.h"
#include "P_ShapeModule.h"
#include "ParticleSystemComponent.h"

P_ShapeModule::P_ShapeModule()
{
    type = P_ModuleType::SHAPE;
    shapeType = ShapeType::NONE;
}

P_ShapeModule::~P_ShapeModule()
{
}

void P_ShapeModule::OnEditor()
{
    if (ImGui::CollapsingHeader("Emitter Shape Module")) 
    {
        if (ImGui::BeginCombo("Change Shape", "Select"))
        {
            for (int n = 0; n < SHAPE_NUM; n++)
            {
                int selectedItem = n;
                if (ImGui::Selectable(_shapeValues[n].c_str(), false))
                {
                    switch (n)
                    {
                    case 0:
                        if (component->GetCurrentShape() != nullptr)
                            component->DeleteCurrentShape();
                        break;
                    case 1:
                        if (component->GetCurrentShape() == nullptr) {
                            component->CreateCurrentShape(ShapeType::CIRCLE);
                        }
                        else {
                            if (component->GetCurrentShape()->shapeType != ShapeType::CIRCLE)
                            {
                                component->DeleteCurrentShape();
                                component->CreateCurrentShape(ShapeType::CIRCLE);
                            }
                        }
                        break;
                    }
                }
            }
            ImGui::EndCombo();
        }
    }
    
}

float3 P_ShapeModule::GetRandomPos()
{
    return float3(0, 0, 0);
}

bool P_ShapeModule::IsInside(float3 position)
{
    return false;
}
