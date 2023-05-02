#include "HUD_SHIELD_BROKEN.h"
HELLO_ENGINE_API_C HUD_SHIELD_BROKEN* CreateHUD_SHIELD_BROKEN(ScriptToInspectorInterface* script)
{
	HUD_SHIELD_BROKEN* classInstance = new HUD_SHIELD_BROKEN();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragBoxGameObject("Struggle Game Object", &classInstance->object_to_move);
	script->AddDragFloat("Movment on X", &classInstance->movment_position);
	script->AddDragFloat("Movment Repetitions", &classInstance->movment_repetitions);
	script->AddDragFloat("Movment Speed", &classInstance->movment_speed);
	
	return classInstance;
}

void HUD_SHIELD_BROKEN::Start()
{
	original_position = object_to_move.GetTransform().GetLocalPosition().x;
	original_positionY = object_to_move.GetTransform().GetLocalPosition().y;
	variable_position = object_to_move.GetTransform().GetLocalPosition().x;
	Console::Log(variable_position);
}
void HUD_SHIELD_BROKEN::Update()
{
		BrokenShield_Animation(breack_shield_boss_anim);
		BrokenShield_Animation(breack_shield_Player_anim);
}

void HUD_SHIELD_BROKEN::BrokenShield_Animation(bool isbroken)
{

	if (isbroken == true)
	{
		if (counter <= movment_repetitions)
		{
			//dreta
			if (object_to_move.GetTransform().GetLocalPosition().x < original_position + movment_position && moviment_Dreta == true)
			{
				variable_position = variable_position + movment_speed;
				object_to_move.GetTransform().SetPosition(variable_position, original_positionY, 0);
				if (variable_position >= original_position + movment_position)
				{
					counter++;
					moviment_Dreta = false;
				}
			}
		//esquerra
			else if (object_to_move.GetTransform().GetLocalPosition().x > original_position - movment_position)
			{
				variable_position = variable_position - movment_speed;
				object_to_move.GetTransform().SetPosition(variable_position, original_positionY, 0);
				if (variable_position <= original_position - movment_position)
				{
					counter++;
					moviment_Dreta = true;
				}
			}
		}
		else if (counter > movment_repetitions)
		{
			if (object_to_move.GetTransform().GetLocalPosition().x > original_position && object_to_move.GetTransform().GetLocalPosition().x != original_position)
			{
				object_to_move.GetTransform().SetPosition(variable_position = variable_position - movment_speed, original_positionY, 0);
				if (variable_position > original_position)
				{
					
					counter = 0;
					breack_shield_boss_anim = false;
					breack_shield_Player_anim = false;

			
				}
			}
			else if (object_to_move.GetTransform().GetLocalPosition().x < original_position && object_to_move.GetTransform().GetLocalPosition().x != original_position)
			{
				object_to_move.GetTransform().SetPosition(variable_position = variable_position + movment_speed, original_positionY, 0);
				if (variable_position < original_position)
				{
					
					counter = 0;
					breack_shield_boss_anim = false;
					breack_shield_Player_anim = false;
				}
			}
		
		}

	}
}