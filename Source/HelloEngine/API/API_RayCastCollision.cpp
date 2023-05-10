#include "Headers.h"
#include "API_RayCastCollision.h"

API::API_RayCastCollision::API_RayCastCollision()
{
	colFraction = 0;
	colDistance = 0;
	colPoint = API_Vector3(0, 0, 0);
	colNormal = API_Vector3(0, 0, 0);
}

API::API_RayCastCollision::~API_RayCastCollision()
{

}

API::API_Vector3 API::API_RayCastCollision::GetPoint()
{
	return colPoint;
}

API::API_Vector3 API::API_RayCastCollision::GetNormal()
{
	return colNormal;
}

float API::API_RayCastCollision::GetFraction()
{
	return colFraction;
}

float API::API_RayCastCollision::GetDistance()
{
	return colDistance;
}

API::API_GameObject API::API_RayCastCollision::GetGameObject()
{
	return colGameObject;
}

