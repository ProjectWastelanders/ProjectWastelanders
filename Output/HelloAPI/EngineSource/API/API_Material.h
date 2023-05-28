#pragma once
#include "Globals.h"
#include "API_GameObject.h"

class TextureComponent;
class DragBoxMaterial;

namespace API
{
	class TO_API API_Material
	{
	public:
		API_Material();
		~API_Material();

		API_GameObject GetGameObject();

		void ChangeAlbedoTexture(uint textureUID);

		bool IsTextureNull();

	private:
		TextureComponent* GetComponent();
		void SetComponent(TextureComponent* component);
	private:
		TextureComponent* _material = nullptr;

		friend class API_GameObject;
		friend class DragBoxMaterialComponent;
	};
}

