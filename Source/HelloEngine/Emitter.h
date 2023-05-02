#pragma once
#include "Particle.h"
#include "Algorithm/Random/LCG.h"

class Shader;
class Application;
class InstanceRenderer;
class ParticleSystemComponent;
class ParticleManager;

class Emitter
{
public:

	Emitter();

	~Emitter();

	//void Draw(Shader* shader, Quat BBrot);

	void Start();

	void Draw();

	void EmitParticles(ParticleProperties& properties);

	void UpdateParticles();

	void UpdateParticleTransform(int i, const math::Quat& rotation);

	void UpdateParticlesOnScene(int i);

	void UpdateParticlesOnGame(int i);

	void SetParticlePoolSize(uint size);

	void ResetEmitter();

	bool GetPlayOnAwake() { return playOnAwake; }

	void SetPlayOnAwake(bool b);

public:

	int64_t _meshID = -1;
	int _textureID = -1;

private:
	//Position Emitter
	float3 position;
	
	std::vector<Particle> ParticleList;

	Application* app;

	LCG random;

	ParticleSystemComponent* component = nullptr;

	InstanceRenderer* manager;

	int currentparticle;
	bool loop;//
	bool stop;//
	bool playOnAwake;//

	float StartDelay;//
	float StartDelayCpy;

	float Duration;//
	float DurationCpy;

	bool enableEmissionModule;//
	int ParticlesPerSecond;//

	Quat BBRotAroundZ;

	float accumulator = 0.0f;

	friend class ParticleSystemComponent;
	friend class ParticleManager;
	friend class P_Module;
	friend class P_MainModule;
	friend class P_EmissionModule;
	friend class ModuleRenderer3D;

};

