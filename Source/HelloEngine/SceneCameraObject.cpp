#include "Headers.h"
#include "SceneCameraObject.h"
#include "GameObject.h"
#include "ModuleLayers.h"
#include "LayerEditor.h"
#include "ModuleWindow.h"
#include "ImGuizmo/ImGuizmo.h"

SceneCameraObject::SceneCameraObject() : CameraObject()
{
	Position = float3(0.0f, 0.0f, 5.0f);
	type = CameraType::SCENE;
}

SceneCameraObject::~SceneCameraObject()
{
}

void SceneCameraObject::UpdateInput()
{
	// Mouse motion ----------------
	GameObject* selectedGO = LayerEditor::selectedGameObject;

	float dx = -ModuleInput::S_GetMouseXMotion();

	float dy = -ModuleInput::S_GetMouseYMotion();

	float Sensitivity = 0.75f;

	float3 empty = { 0,0,0 };

	Quat lookingDir = Quat::identity;

	cameraFrustum.WorldMatrix().Decompose(empty, lookingDir, empty);

	if (ModuleInput::S_GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		float3 newPos(0, 0, 0);
		float speed = cameraSpeed * app->fps;

		if (ModuleInput::S_GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
			speed = cameraSpeed * app->fps * 10.0f;

		if (ModuleInput::S_GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
			newPos += cameraFrustum.front * speed;

		if (ModuleInput::S_GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
			newPos -= cameraFrustum.front * speed;

		if (ModuleInput::S_GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
			newPos -= cameraFrustum.WorldRight() * speed;

		if (ModuleInput::S_GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
			newPos += cameraFrustum.WorldRight() * speed;

		if (ModuleInput::S_GetKey(SDL_SCANCODE_Q) == KEY_REPEAT)
			newPos += cameraFrustum.up * speed;

		if (ModuleInput::S_GetKey(SDL_SCANCODE_E) == KEY_REPEAT)
			newPos -= cameraFrustum.up * speed;

		cameraFrustum.pos += newPos;

		if (dy != 0)
		{
			float DeltaY = math::DegToRad(dy * Sensitivity * 0.75f);

			math::Quat rotation = Quat::identity;

			rotation.SetFromAxisAngle(float3(1, 0, 0), DeltaY);

			lookingDir = lookingDir * rotation;
		}

		if (dx != 0)
		{
			float DeltaX = math::DegToRad(dx * Sensitivity * 0.75f);

			math::Quat rotation = Quat::identity;

			rotation.SetFromAxisAngle({ 0.0f, 1.0f, 0.0f }, DeltaX);

			lookingDir = rotation * lookingDir;
		}

		float4x4 newWorldMatrix = cameraFrustum.WorldMatrix();
		newWorldMatrix.SetRotatePart(lookingDir.Normalized());
		cameraFrustum.SetWorldMatrix(newWorldMatrix.Float3x4Part());
	}

	// Orbital rotation
	if (ModuleInput::S_GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT && ModuleInput::S_GetMouseButton(1) == KEY_REPEAT && !ImGuizmo::IsUsing())
	{
		float3 rotationCenter = { 0.0f,0.0f,0.0f };

		if (selectedGO != nullptr) rotationCenter = selectedGO->transform->GetGlobalMatrix().TranslatePart();

		float distFromCenter = cameraFrustum.pos.Distance(rotationCenter); // Get current distance from center

		if (dx != 0)
		{
			float DeltaX = math::DegToRad(dx * Sensitivity * 0.75f);

			Quat rotation = Quat::identity;

			rotation = rotation.RotateY(DeltaX);

			lookingDir = rotation * lookingDir;
		}

		if (dy != 0)
		{
			float DeltaY = math::DegToRad(dy * Sensitivity * 0.75f);
			float cosAngle = math::Dot(float3(0, 1, 0), cameraFrustum.front);

			if (math::Abs(cosAngle) > 0.99f)
			{
				if (cosAngle > 0 && DeltaY > 0)
					DeltaY = 0;

				if (cosAngle < 0 && DeltaY < 0)
					DeltaY = 0;
			}

			Quat rotation = Quat::identity;

			rotation = rotation.RotateX(DeltaY);

			lookingDir = lookingDir * rotation;
		}

		float4x4 newWorldMatrix = cameraFrustum.WorldMatrix();

		newWorldMatrix.SetRotatePart(lookingDir.Normalized());

		cameraFrustum.SetWorldMatrix(newWorldMatrix.Float3x4Part());

		cameraFrustum.pos = rotationCenter + (cameraFrustum.front * -distFromCenter);
	}

	int mouseZ = ModuleInput::S_GetMouseZ();

	if (mouseZ != 0)
	{
		float speed = _wheelSpeed * ImGui::GetIO().Framerate;
		if (mouseZ > 0)
			cameraFrustum.pos += cameraFrustum.front * speed;
		if (mouseZ < 0)
			cameraFrustum.pos -= cameraFrustum.front * speed;
	}

	// Camera panning
	if (ModuleInput::S_GetMouseButton(2))
	{
		float panningSensitivity = 0.05f;

		float4x4 cameraMat = cameraFrustum.WorldMatrix();
		float3 movementX = cameraMat.RotatePart().Col(0) * dx * panningSensitivity;
		float3 movementY = cameraMat.RotatePart().Col(1) * -dy * panningSensitivity;

		float3 movement = movementX + movementY;

		cameraFrustum.pos += movement;
	}

	if (ModuleInput::S_GetKey(SDL_SCANCODE_F) == KEY_DOWN)
	{
		if (selectedGO != nullptr)
			Focus(selectedGO);
	}
}

bool SceneCameraObject::SetWheelSpeed(float speed)
{
	if (speed < 0)
		return false;

	_wheelSpeed = speed;
	return true;
}

void SceneCameraObject::Focus(const float3& focusPoint, float distance)
{
	float3 newPos = focusPoint;
	newPos -= (cameraFrustum.front * distance);
	cameraFrustum.pos = newPos;
	LookAt(focusPoint);
}

void SceneCameraObject::Focus(GameObject* gameObject)
{
	float3 focusPoint = gameObject->transform->GetGlobalMatrix().TranslatePart();
	float3 newPos = focusPoint;

	float distance = 4.0f;
	float distanceOffset = 1.5f;

	if (gameObject->HasComponent<MeshRenderComponent>())
	{
		Mesh& mesh = gameObject->GetComponent<MeshRenderComponent>()->GetMesh();
		AABB& meshAABB = mesh.globalAABB;
		distance = meshAABB.Size().Length() * distanceOffset; // Uses square root, but its just once and on an event, should be fine.
	}

	newPos -= (cameraFrustum.front * distance);
	cameraFrustum.pos = newPos;
	LookAt(focusPoint);
}
