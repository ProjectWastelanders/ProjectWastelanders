#include "Headers.h"
#include "Particle.h"

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
	
	elapsedTime += EngineTime::GameDeltaTime();
	float particleLife = elapsedTime / Lifetime;
	int numOfText = texture.numOfRows * texture.numOfRows;
	float atlasProgression = particleLife * numOfText;
	int index1 = (int) math::Floor(atlasProgression);
	int index2 = index1 < numOfText - 1 ? index1 + 1 : index1;
	int x = (int)atlasProgression * 1000;
	int y = x % 1000;
	this->blendFactor = y / 1000;

	int column = index1 % texture.numOfRows;
	int row = index1 / texture.numOfRows;
	textureOffset1.x = column / texture.numOfRows;
	textureOffset1.y = row / texture.numOfRows;
	column = index2 % texture.numOfRows;
	row = index2 / texture.numOfRows;
	textureOffset2.x = column / texture.numOfRows;
	textureOffset2.y = row / texture.numOfRows;

	textureInfo.x = texture.numOfRows;
	textureInfo.y = blendFactor;

}

