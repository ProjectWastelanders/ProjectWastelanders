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
                    case 2:
                        if (component->GetCurrentShape() == nullptr) {
                            component->CreateCurrentShape(ShapeType::RECTANGLE);
                        }
                        else {
                            if (component->GetCurrentShape()->shapeType != ShapeType::RECTANGLE)
                            {
                                component->DeleteCurrentShape();
                                component->CreateCurrentShape(ShapeType::RECTANGLE);
                            }
                        }
                        break;
                    case 3:
                        if (component->GetCurrentShape() == nullptr) {
                            component->CreateCurrentShape(ShapeType::SPHERE);
                        }
                        else {
                            if (component->GetCurrentShape()->shapeType != ShapeType::SPHERE)
                            {
                                component->DeleteCurrentShape();
                                component->CreateCurrentShape(ShapeType::SPHERE);
                            }
                        }
                        break;
                    case 4:
                        if (component->GetCurrentShape() == nullptr) {
                            component->CreateCurrentShape(ShapeType::CUBE);
                        }
                        else {
                            if (component->GetCurrentShape()->shapeType != ShapeType::CUBE)
                            {
                                component->DeleteCurrentShape();
                                component->CreateCurrentShape(ShapeType::CUBE);
                            }
                        }
                        break;
                    case 5:
                        if (component->GetCurrentShape() == nullptr) {
                            component->CreateCurrentShape(ShapeType::CONE);
                        }
                        else {
                            if (component->GetCurrentShape()->shapeType != ShapeType::CONE)
                            {
                                component->DeleteCurrentShape();
                                component->CreateCurrentShape(ShapeType::CONE);
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

void P_ShapeModule::Serialization(json& j)
{
}

void P_ShapeModule::DeSerialization(json& j)
{
}
