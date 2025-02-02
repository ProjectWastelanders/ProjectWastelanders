#pragma once
#include "InstanceRenderer.h"

struct ParticleProperties
{
	//Position of the particle
	float3 position;
	//Speed and SpeedVariation of the particle
	float3 speed, speedVariation, acceleration;
	//Particle Rotation
	float3 rotation;
	//Starting and Ending color of the particle
	float4 startColor, endColor;
	//Starting and Ending size of the particle
	float3 startsize,endsize,sizevariation;
	//Life time of the particle
	float Lifetime = 1.0f;
};

struct ParticleTexture
{

	//Texture id
	uint _textureID = -1;
	//Number of Rows
	int numOfRows = 1;

};


struct Particle
{

	Particle();

	~Particle();
	//Position of the particle
	float3 position;
	//particle's Euler rotation in degrees.
	float3 rotation;
	//Scale of the particle
	float3 scale;
	//Transform Matrix of the particle
	float4x4 transformMat;
	//Speed of the particle
	float3 speed;
	//Acceleration of the particle
	float3 acceleration;
	//Starting color of the particle
	float4 startColor;
	//Ending color of the particle
	float4 endColor;
	//how many seconds remain of the particle to be dead
	float remainingLifetime;
	//Life time of the particle
	float Lifetime;
	//initial size of the particle
	float3 startSize;
	//End size of the particle
	float3 endSize;

	//Elapsed Time
	float elapsedTime = 0.0f;
	//ID of the instance
	uint _instanceID;
	
	bool Active = false;

	//Particle Texture
	ParticleTexture texture;
	//Texture Offsets;
	ParticleAnimInfo particleAnim;

	float blendFactor = 100.0f;

	//Angular velocity of the particle
	//float rotationSpeed;

	//Calculate the Transform Matrix of the particle
	void SetTransformMatrix(Quat rot);

	void UpdateTextureCoords();

	float FindModule(float a, float b);

};

