#include "Headers.h"
#include "Emitter.h"
#include "ModuleRenderer3D.h"
#include "InstanceRenderer.h"
#include "LayerGame.h"
#include "ParticleSystemComponent.h"
#include "BillBoardComponent.h"

Emitter::Emitter()
{
	app = Application::Instance();
	app->renderer3D->particleManager.EmitterList.emplace_back(this);

	BBRotAroundZ = Quat::identity;

	loop = true;
	stop = false;
	

	StartDelay = 0.0f;
	StartDelayCpy = 0.0f;
	Duration = 0.0f;
	DurationCpy = 0.0f;

	playOnAwake = false;
	enableEmissionModule = true;
	ParticlesPerSecond = 1;
	emitterTexture.numOfRows = 4;
}

Emitter::~Emitter()
{

	app->renderer3D->particleManager.RemoveEmitterInList(this);

}

void Emitter::SetParticlePoolSize(uint size)
{
	currentparticle = size - 1;

	ParticleList.resize(size);
}

void Emitter::ResetEmitter()
{
	for (int i = 0; i < ParticleList.size(); i++)
	{
		Mesh& meshReference = manager->GetMap()[ParticleList[i]._instanceID].mesh;

		meshReference.draw = false;

		ParticleList[i].Active = false;
	}
	currentparticle = ParticleList.size() - 1;
	StartDelay = StartDelayCpy;
	Duration = DurationCpy;
}

void Emitter::SetPlayOnAwake(bool b)
{
	playOnAwake = b;
}

void Emitter::EmitParticles(ParticleProperties& particleProps)
{
	if (currentparticle < 0)
	{
		currentparticle = ParticleList.size() - 1;
	}

	Particle& particle = ParticleList[currentparticle];
	particle.Active = true;
	particle.position = particleProps.position;
	particle.startSize = particleProps.startsize + particleProps.sizevariation * (random.Float() - 0.5f); //Random number between -0.5 / 0.5
	particle.endSize = particleProps.endsize;

	//text = particleProps.texture;
	// Velocity
	particle.speed = particleProps.speed;
	particle.speed.x += particleProps.speedVariation.x * (random.Float() - 0.5f);
	particle.speed.y += particleProps.speedVariation.y * (random.Float() - 0.5f);
	particle.speed.z += particleProps.speedVariation.z * (random.Float() - 0.5f);

	// Acceleration
	particle.acceleration = particleProps.acceleration;

	// Color
	particle.startColor = particleProps.startColor;
	particle.endColor = particleProps.endColor;

	particle.Lifetime = particleProps.Lifetime;
	particle.remainingLifetime = particleProps.Lifetime;

	particle.SetTransformMatrix(BBRotAroundZ);

	particle.elapsedTime = 0.0f;
	particle.blendFactor = 0.0f;

	particle.particleAnim.textOffsets = float4 (0.0f,0.0f,0.0f,0.0f);
	particle.particleAnim.texInfo = float2(emitterTexture.numOfRows, 0.0f);
	
	currentparticle--;

}

void Emitter::Start()
{
	SetParticlePoolSize(component->size);
}

void Emitter::Draw()
{

	//manager->Draw();

}

void Emitter::UpdateParticles()
{
	if (StartDelay < 0)
	{
		StartDelay = 0;
	}
	if (Duration < 0)
	{
		Duration = 0;
	}
	for (int i = 0; i < ParticleList.size(); i++)
	{
		if (!ParticleList[i].Active)
			continue;

		if (ParticleList[i].remainingLifetime <= 0.0f)
		{
			Mesh& meshReference = manager->GetMap()[ParticleList[i]._instanceID].mesh;

			meshReference.draw = false;

			ParticleList[i].Active = false;
			
			//manager->isParticle = false;
			continue;
		}

		if (!LayerGame::S_IsPlaying() && component->GetPlayOnScene())
		{
			UpdateParticlesOnScene(i);
		}
		else if(LayerGame::S_IsPlaying() && component->GetPlayOnGame())
		{
			UpdateParticlesOnGame(i);
		}
		else if (LayerGame::S_IsPlaying() && component->GetPauseOnScene())
		{
			ResetEmitter();
		}

		if(StartDelay <= 0 && !LayerGame::S_IsPlaying())
		{

			UpdateParticleTransform(i,component->GetGameObject()->GetComponent<BillBoardComponent>()->GetBBRotation(ParticleList[i]));

		}
		else if (StartDelay <= 0 && LayerGame::S_IsPlaying() && component->GetPlayOnGame())
		{

			UpdateParticleTransform(i, component->GetGameObject()->GetComponent<BillBoardComponent>()->GetBBRotation(ParticleList[i]));

		}
		else
		{
			continue;
		}
	}
}

void Emitter::UpdateParticleTransform(int i, const math::Quat& rotation)
{
	float life = ParticleList[i].remainingLifetime / ParticleList[i].Lifetime;

	ParticleList[i].scale = Lerp(ParticleList[i].endSize, ParticleList[i].startSize, life);

	BBRotAroundZ = rotation;

	ParticleList[i].SetTransformMatrix(rotation);

	manager = app->renderer3D->renderManager.GetRenderManager(_meshID, 0);

	if (ParticleList[i].texture.numOfRows > 1)
	{
		manager->particleAnimInfos.push_back(ParticleList[i].particleAnim);
	}
	
	Mesh& meshReference = manager->GetMap()[ParticleList[i]._instanceID].mesh;

	meshReference.draw = true;

	meshReference.modelMatrix = ParticleList[i].transformMat;

	meshReference.textureID = emitterTexture._textureID;

	meshReference.CalculateBoundingBoxes();
}

void Emitter::UpdateParticlesOnScene(int i)
{
	
	ParticleList[i].UpdateTextureCoords();
	// Compute all the calculus needed to move the particles

	// Remaining life minus dt
	ParticleList[i].remainingLifetime -= EngineTime::EngineTimeDeltaTime();
	ParticleList[i].elapsedTime += EngineTime::EngineTimeDeltaTime();


	// velocity = acceleration * dt
	ParticleList[i].speed += ParticleList[i].acceleration * EngineTime::EngineTimeDeltaTime();


	// pos += velocity * dt
	ParticleList[i].position += ParticleList[i].speed * EngineTime::EngineTimeDeltaTime();
	
}

void Emitter::UpdateParticlesOnGame(int i)
{
	
	ParticleList[i].UpdateTextureCoords();
	// Compute all the calculus needed to move the particles

	// Remaining life minus dt
	ParticleList[i].remainingLifetime -= EngineTime::GameDeltaTime();
	ParticleList[i].elapsedTime += EngineTime::GameDeltaTime();


	// velocity = acceleration * dt
	ParticleList[i].speed += ParticleList[i].acceleration * EngineTime::GameDeltaTime();


	// pos += velocity * dt
	ParticleList[i].position += ParticleList[i].speed * EngineTime::GameDeltaTime();
	
	
}
