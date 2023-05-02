#pragma once
#include "Globals.h"
#include "API/API_GameObject.h"

class ComponentUIImage;

namespace API
{
	class TO_API API_UIImage
	{
	public:
		API_UIImage();
		~API_UIImage();

		API_GameObject GetGameObject();
		/// Value gets clamped to 0->1 range.
		void FillImage(float _GetFillImage);
		void SetOpacity(float opacity);

	private:
		ComponentUIImage* GetComponent();
		void SetComponent(ComponentUIImage* component);
	private:
		ComponentUIImage* _UIImage = nullptr;

		friend class API_GameObject;
		friend class DragBoxUIImage;
	};
}
