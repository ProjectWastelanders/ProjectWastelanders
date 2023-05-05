#include "Headers.h"
#include "ParticleManager.h"
#include "Emitter.h"
#include "Particle.h"
#include "ParticleSystemComponent.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "LayerGame.h"
#include "BillBoardComponent.h"

ParticleManager::ParticleManager()
{

}

ParticleManager::~ParticleManager()
{
}

void ParticleManager::Init()
{
}

void ParticleManager::Start()
{
	for (Emitter* emitter : EmitterList)
	{
		emitter->Start();
	}
}

void ParticleManager::Draw()
{

	for (Emitter* emitter : EmitterList)
	{


		if (emitter->playOnAwake && LayerGame::S_IsPlaying()) {
			
			emitter->component->SetPlayOnGame(true);
		}

		if ((LayerGame::S_IsPlaying() && emitter->component->GetPlayOnGame()) || emitter->component->GetPlayOnScene())
		{
			//UPDATE EACH EMITTER
			if (!emitter->component->StopEmittingOnGame)
			{
				if (LayerGame::S_IsPlaying())
				{
					if (emitter->StartDelay <= 0) {
						if (emitter->Duration > 0 && !emitter->loop) {
							emitter->Duration -= EngineTime::GameDeltaTime();
						}
					}
					else {
						emitter->StartDelay -= EngineTime::GameDeltaTime();
					}
				}
				else
				{
					if (emitter->StartDelay <= 0) {
						if (emitter->Duration > 0 && !emitter->loop) {
							emitter->Duration -= EngineTime::EngineTimeDeltaTime();
						}
					}
					else {
						emitter->StartDelay -= EngineTime::EngineTimeDeltaTime();
					}
				}
			}

			if (!emitter->component->StopEmittingOnGame)
			{
				if (!emitter->loop) {
					if (emitter->Duration <= 0) {
						emitter->stop = true;
					}
					else {
						emitter->stop = false;
					}
				}
				else {
					emitter->stop = false;
				}
			}
			else
			{
				emitter->stop = true;
			}

			if (emitter->component != nullptr)
				emitter->component->particleProps.position = emitter->component->_gameObject->transform->GetGlobalPosition();

			if (emitter->enableEmissionModule)
			{
				if (!emitter->stop) {
					double particlesToEmit = 0;
					if (LayerGame::S_IsPlaying()) {
						//canviar game delta time per float
						emitter->accumulator += EngineTime::GameDeltaTime();

					}
					else {
						emitter->accumulator += EngineTime::EngineTimeDeltaTime();
					}

					while (emitter->accumulator > 1.0 / emitter->ParticlesPerSecond) {
						emitter->EmitParticles(emitter->component->particleProps);
						emitter->accumulator -= 1.0 / emitter->ParticlesPerSecond;
					}
				}

			}
			else{
				if (!emitter->stop) {
					emitter->EmitParticles(emitter->component->particleProps);
				}
			}

			emitter->UpdateParticles();

			// DRAW EACH EMITTER
			
			//var->Draw();
			
		}

	}
	
}

void ParticleManager::RemoveEmitterInList(Emitter* emitter)
{

	for (int i = 0; i < EmitterList.size(); i++)
	{
		if (EmitterList[i] == emitter)
		{
			EmitterList.erase(EmitterList.begin() + i);
		}
	}

}

void ParticleManager::RemoveAllEmitters()
{
	EmitterList.clear();
}


