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

void Particle::UpdateTextureCoords(int &cont)
{

	/*switch (cont)
	{
	case 0: {
		particleAnim.textOffsets.x = 0.0f;
		particleAnim.textOffsets.y = 0.0f;
		particleAnim.textOffsets.z = 0.25f;
		particleAnim.textOffsets.w = 0.0f;
	}break;
	case 1: {
		particleAnim.textOffsets.x = 0.25f;
		particleAnim.textOffsets.y = 0.0f;
		particleAnim.textOffsets.z = 0.50f;
		particleAnim.textOffsets.w = 0.0f;
	}break;
	case 2: {
		particleAnim.textOffsets.x = 0.5f;
		particleAnim.textOffsets.y = 0.0f;
		particleAnim.textOffsets.z = 0.75f;
		particleAnim.textOffsets.w = 0.0f;
	}break;
	case 3: {
		particleAnim.textOffsets.x = 0.75f;
		particleAnim.textOffsets.y = 0.0f;
		particleAnim.textOffsets.z = 0.0f;
		particleAnim.textOffsets.w = 0.25f;
	}break;
	case 4: {
		particleAnim.textOffsets.x = 0.0f;
		particleAnim.textOffsets.y = 0.25f;
		particleAnim.textOffsets.z = 0.25f;
		particleAnim.textOffsets.w = 0.25f;
	}break;
	case 5: {
		particleAnim.textOffsets.x = 0.25f;
		particleAnim.textOffsets.y = 0.25f;
		particleAnim.textOffsets.z = 0.5f;
		particleAnim.textOffsets.w = 0.25f;
	}break;
	case 6: {
		particleAnim.textOffsets.x = 0.5f;
		particleAnim.textOffsets.y = 0.25f;
		particleAnim.textOffsets.z = 0.75f;
		particleAnim.textOffsets.w = 0.25f;
	}break;

	default:
		cont = 0;
		break;
	}*/
	
	float particleLife = elapsedTime / Lifetime;
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

	std::cout << particleAnim.textOffsets.x << " TexOffset X ";
	std::cout << particleAnim.textOffsets.y << " TexOffset Y " << "\n";
	std::cout << particleAnim.textOffsets.z << " TexOffset Z ";
	std::cout << particleAnim.textOffsets.w << " TexOffset W " << "\n";
	std::cout << index1 << " / " << " Index 1" << "\n";
	std::cout << index2 << " / " << " Index 2" << "\n";
	std::cout << elapsedTime << "elapsed time ";
	std::cout << atlasProgression << " AtlasProgression" << "\n\n";
	

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

