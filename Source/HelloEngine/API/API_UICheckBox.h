#pragma once
#include "Globals.h"
#include "API/API_GameObject.h"

class ComponentUICheckbox;

namespace API
{
	class TO_API API_UICheckBox
	{
	public:
		API_UICheckBox();
		~API_UICheckBox();

		API_GameObject GetGameObject();

	private:
		ComponentUICheckbox* GetComponent();
		void SetComponent(ComponentUICheckbox* component);

		ComponentUICheckbox* _UICheckBox = nullptr;

		friend class API_GameObject;
		friend class DragBoxUICheckBox;
	};
}