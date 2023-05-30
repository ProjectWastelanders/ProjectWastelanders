#include "Headers.h"
#include "CameraObject.h"
#include "ModuleWindow.h"
#include "ModuleCamera3D.h"

#include "ModuleResourceManager.h"

CameraObject::CameraObject()
{
	app = Application::Instance();

	X = float3(1.0f, 0.0f, 0.0f);
	Y = float3(0.0f, 1.0f, 0.0f);
	Z = float3(0.0f, 0.0f, 1.0f);

	Position = float3(0.0f, 0.0f, 0.0f);
	Reference = float3(0.0f, 0.0f, 0.0f);

	cameraFrustum.type = math::FrustumType::PerspectiveFrustum;
	FOV = 60;
	cameraFrustum.verticalFov = math::DegToRad(FOV);
	aspectRatio = 1.7f;
	cameraFrustum.horizontalFov = 2.0f * atanf(tanf(cameraFrustum.verticalFov / 2.0f) * aspectRatio);

	cameraFrustum.nearPlaneDistance = 0.01f;
	cameraFrustum.farPlaneDistance = 1000.0f;

	cameraFrustum.pos = Position;
	cameraFrustum.front = Z;
	cameraFrustum.up = Y;

	localLineShader = ModuleResourceManager::S_CreateResourceShader("Resource/shaders/localLines.shader", 101, "Local Lines");
	SetUpBuffers();
}

CameraObject::~CameraObject()
{
	// TODO: Could be done more clean if we had an event system to commmunicate this kind of things.
	Application::Instance()->camera->EraseGameCamera(this);
	//RELEASE(localLineShader);
	//Cleaned along with shader
	localLineShader = nullptr;
}

void CameraObject::Look(const float3& Position, const float3& Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;

	Z = (Position - Reference).Normalized();
	X = (float3(0.0f, 1.0f, 0.0f).Cross(Z)).Normalized();
	Y = Cross(Z, X);

	if (!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}
}

void CameraObject::LookAt(const float3& Spot)
{
	cameraFrustum.front = (Spot - cameraFrustum.pos).Normalized();
	float3 X = float3(0, 1, 0).Cross(cameraFrustum.front).Normalized();
	cameraFrustum.up = cameraFrustum.front.Cross(X);
}

void CameraObject::Move(const float3& Movement)
{
	cameraFrustum.pos += Movement;
}

void CameraObject::DrawFrustum()
{
	float3 frustumPoints[8];

	cameraFrustum.GetCornerPoints(frustumPoints);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	void* ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	memcpy(ptr, &frustumPoints[0], 8 * sizeof(float3));
	glUnmapBuffer(GL_ARRAY_BUFFER);

	localLineShader->shader.Bind();
	localLineShader->shader.SetMatFloat4v("view", Application::Instance()->camera->currentDrawingCamera->GetViewMatrix());
	localLineShader->shader.SetMatFloat4v("projection", Application::Instance()->camera->currentDrawingCamera->GetProjectionMatrix());
	localLineShader->shader.SetFloat4("lineColor", 0.0f, 1.0f, 1.0f, 1.0f);

	glDrawElements(GL_LINES, boxIndices.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void CameraObject::SetUpBuffers()
{
	boxIndices.push_back(0);    // 1
	boxIndices.push_back(1);    // 2
	boxIndices.push_back(0);    // 3
	boxIndices.push_back(2);    // 4
	boxIndices.push_back(2);    // 5
	boxIndices.push_back(3);    // 6
	boxIndices.push_back(1);    // 7
	boxIndices.push_back(3);    // 8
	boxIndices.push_back(0);    // 9
	boxIndices.push_back(4);    // 10
	boxIndices.push_back(4);    // 11
	boxIndices.push_back(5);    // 12
	boxIndices.push_back(4);    // 13
	boxIndices.push_back(6);    // 14
	boxIndices.push_back(6);    // 15
	boxIndices.push_back(7);    // 16
	boxIndices.push_back(7);    // 17
	boxIndices.push_back(5);    // 18
	boxIndices.push_back(1);    // 19
	boxIndices.push_back(5);    // 20
	boxIndices.push_back(3);    // 21
	boxIndices.push_back(7);    // 22
	boxIndices.push_back(2);    // 23
	boxIndices.push_back(6);    // 24

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * boxIndices.size(), &boxIndices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float3) * 8, nullptr, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float3), (void*)0);

	glBindVertexArray(0);
}

float* CameraObject::GetViewMatrix()
{
	ViewMatrix = cameraFrustum.ViewMatrix();
	ViewMatrix.Transpose();
    return &ViewMatrix.v[0][0];
}

float* CameraObject::GetProjectionMatrix()
{
	ProjectionMatrix = cameraFrustum.ProjectionMatrix().Transposed();
	return &ProjectionMatrix.v[0][0];
}

float* CameraObject::GetViewMatrixNoTransp()
{
	ViewMatrix = cameraFrustum.ViewMatrix();
	return &ViewMatrix.v[0][0];
}

float3 CameraObject::GetFrustumPosition()
{
	return cameraFrustum.pos;
}

float3 CameraObject::GetPosition()
{
	return cameraFrustum.ViewMatrix().TranslatePart();
}

float* CameraObject::GetProjectionMatrixNoTransp()
{
	ProjectionMatrix = cameraFrustum.ProjectionMatrix();
	return &ProjectionMatrix.v[0][0];
}

void CameraObject::ChangeToOrthograpic()
{
	if (cameraFrustum.type != math::FrustumType::OrthographicFrustum)
	{
		cameraFrustum.orthographicWidth = _orthographicSize * aspectRatio;
		cameraFrustum.orthographicHeight = _orthographicSize * aspectRatio;
		cameraFrustum.type = math::FrustumType::OrthographicFrustum;
	}
}

void CameraObject::ChangeToPerspective()
{
	if (cameraFrustum.type != math::FrustumType::PerspectiveFrustum)
		cameraFrustum.type = math::FrustumType::PerspectiveFrustum;
}

void CameraObject::ChangeOrthographicSize(float size)
{
	_orthographicSize = size;
	cameraFrustum.orthographicWidth = _orthographicSize * aspectRatio;
	cameraFrustum.orthographicHeight = _orthographicSize * aspectRatio;
}

float CameraObject::GetOrthographicSize()
{
	return _orthographicSize;
}

void CameraObject::RecalculateProjection()
{
	float aspectRatio = (float)(frameBuffer.width) / (float)(frameBuffer.height);
	cameraFrustum.horizontalFov = 2.f * atanf(tanf(cameraFrustum.verticalFov * 0.5f) * aspectRatio);
}

void CameraObject::RegenerateFrameBuffer(int width, int height)
{
	frameBuffer.SetDimensions(width, height);
	RecalculateProjection();
}

void CameraObject::ChangeAspectRatio(float aspectRatio)
{
	if (cameraFrustum.type == FrustumType::OrthographicFrustum)
	{
		cameraFrustum.orthographicWidth = _orthographicSize * aspectRatio;
		cameraFrustum.orthographicHeight = _orthographicSize * aspectRatio;
		return;
	}
	this->aspectRatio = aspectRatio;
	cameraFrustum.horizontalFov = 2.0f * atanf(tanf(cameraFrustum.verticalFov * 0.5f) * aspectRatio);
	// Resize orthographic
	
}

void CameraObject::SetFOV(float fov)
{
	this->FOV = fov;
	cameraFrustum.verticalFov = math::DegToRad(fov);
	cameraFrustum.horizontalFov = 2.0f * atanf(tanf(cameraFrustum.verticalFov / 2.0f) * aspectRatio);
}

bool CameraObject::IsInsideFrustum(AABB& globalAABB)
{
	if (cameraFrustum.type == FrustumType::PerspectiveFrustum)
		return IsInsideFrustumPerspective(globalAABB);
	else
		return IsInsideFrustumOrthographic(globalAABB);
}

bool CameraObject::IsInsideFrustumPerspective(AABB& globalAABB)
{
	float3 vCorner[8];
	math::Plane planes[6];
	cameraFrustum.GetPlanes(planes);
	int iTotalIn = 0;
	globalAABB.GetCornerPoints(vCorner); // get the corners of the box into the vCorner array
	// test all 8 corners against the 6 sides
	// if all points are behind 1 specific plane, we are out
	// if we are in with all points, then we are fully in
	for (int p = 0; p < 6; ++p) {
		int iInCount = 8;
		int iPtIn = 1;
		for (int i = 0; i < 8; ++i) {
			// test this point against the planes
			if (planes[p].IsOnPositiveSide(vCorner[i])) { //<-- “IsOnPositiveSide?from MathGeoLib
				iPtIn = 0;
				--iInCount;
			}
		}
		// were all the points outside of plane p?
		if(iInCount == 0)
			return false;
		// check if they were all on the right side of the plane
		iTotalIn += iPtIn;
	}
	// so if iTotalIn is 6, then all are inside the view
	/*if (iTotalIn == 6)
		return true;*/
	// we must be partly in then otherwise
	return true;
}

bool CameraObject::IsInsideFrustumOrthographic(AABB& globalAABB)
{
	// TODO: This is temporal. Doesn't work with too small frustums, where all 8 points are outside.
	for (int i = 0; i < 8; ++i)
	{
		if (cameraFrustum.Contains(globalAABB.CornerPoint(i)))
			return true;
	}
	return false;

}
