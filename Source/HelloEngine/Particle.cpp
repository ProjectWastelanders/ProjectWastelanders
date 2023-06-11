#include "Headers.h"
#include "Particle.h"
#include "LayerGame.h"

Particle::Particle():startColor(0.0f, 1.0f, 0.0f, 1.0f), endColor(0.0f,1.0f,0.0f,0.0f)
{
	 
	this->position = { 0.0f,0.0f,0.0f };

	this->scale = { 1.0f,1.0f,1.0f };

	this->Lifetime = 1.0f;

	this->remainingLifetime = 0.0f;

	this->rotation = { 0.0f,0.0f,0.0f };

	this->transformMat = float4x4::identity;

	//this->rotationSpeed = 0.0f;

	this->startSize = {5.0f,5.0f,5.0f};

	this->endSize = { 5.0f,5.0f,5.0f };

	this->speed = { 0.0f,0.0f,0.0f };

	this->acceleration = { 0.0f,0.0f,0.0f };

	this->Active = false;
	
}

Particle::~Particle()
{
}

void Particle::SetTransformMatrix(Quat rot = Quat::identity)
{
	float x = rotation.x * DEGTORAD;
	float y = rotation.y * DEGTORAD;
	float z = rotation.z * DEGTORAD;

	Quat q = rot * Quat::FromEulerXYZ(x, y, z);

	transformMat = float4x4::FromTRS(position, q, scale).Transposed();
}

void Particle::UpdateTextureCoords()
{
	
	float particleLife = elapsedTime / Lifetime;

	if (particleLife > 1.0f)return;

	int numOfText = texture.numOfRows * texture.numOfRows;
	float atlasProgression = particleLife * numOfText;
	int index1 = (int) math::Floor(atlasProgression);
	int index2 = index1 < numOfText - 1 ? index1 + 1 : index1;
	this->blendFactor = FindModule(atlasProgression,1);

	int column = index1 % texture.numOfRows;
	int row = index1 / texture.numOfRows;
	
	particleAnim.textOffsets.x = (float)column / texture.numOfRows;
	particleAnim.textOffsets.y = (float)row / texture.numOfRows;
	int column2 = index2 % texture.numOfRows;
	int row2 = index2 / texture.numOfRows;

	particleAnim.textOffsets.z = (float)column2 / texture.numOfRows;
	particleAnim.textOffsets.w = (float)row2 / texture.numOfRows;

	particleAnim.texInfo.x = texture.numOfRows;
	particleAnim.texInfo.y = blendFactor;

}

float Particle::FindModule(float a, float b)
{
	double mod;
	// Handling negative values
	if (a < 0)
		mod = -a;
	else
		mod = a;
	if (b < 0)
		b = -b;

	// Finding mod by repeated subtraction

	while (mod >= b)
		mod = mod - b;

	// Sign of result typically depends
	// on sign of a.
	if (a < 0)
		return -mod;

	return mod;
}

