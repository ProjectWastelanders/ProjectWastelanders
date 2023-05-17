#pragma once
#include "Globals.h"
#include "API/API_GameObject.h"

class ComponentUISlider;

namespace API
{
	class TO_API API_UISlider
	{
	public:
		API_UISlider();
		~API_UISlider();

		API_GameObject GetGameObject();

		bool OnPress();

		bool OnHold();

		bool OnHovered();

		int GetValue();

	private:
		ComponentUISlider* GetComponent();
		void SetComponent(ComponentUISlider* component);

		ComponentUISlider* _UISlider = nullptr;

		friend class API_GameObject;
		friend class DragBoxUISlider;
	};
}