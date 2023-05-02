#pragma once
#include <vector>
#include "Particle.h"

class Emitter;
class ParticleManager
{
public:

	ParticleManager();
	~ParticleManager();

	void Init();
	void Start();
	void Draw();

	void RemoveEmitterInList(Emitter* emitter);

	void RemoveAllEmitters();

	std::vector<Emitter*> EmitterList;

};

