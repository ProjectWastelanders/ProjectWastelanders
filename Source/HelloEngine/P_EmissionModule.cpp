#include "Headers.h"
#include "P_EmissionModule.h"
#include "ParticleSystemComponent.h"

P_EmissionModule::P_EmissionModule()
{
	type = P_ModuleType::EMISSION;
}

P_EmissionModule::~P_EmissionModule()
{
}

void P_EmissionModule::OnEditor()
{
	if (ImGui::CollapsingHeader("Particle Emission Module"))
	{
		ImGui::Checkbox("Enable", &component->ParticleEmitter.enableEmissionModule);

		ImGui::DragInt("Particles per second", &component->ParticleEmitter.ParticlesPerSecond);

		if (component->ParticleEmitter.ParticlesPerSecond < 0) {
			component->ParticleEmitter.ParticlesPerSecond = 0;
		}
		
	}

}