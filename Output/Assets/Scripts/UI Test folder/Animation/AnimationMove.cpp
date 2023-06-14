#include "AnimationMove.h"
HELLO_ENGINE_API_C AnimationMove* CreateAnimationMove(ScriptToInspectorInterface* script)
{
	AnimationMove* classInstance = new AnimationMove();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);

	script->AddDragBoxGameObject("Button Guide", &classInstance->gameObject);

	script->AddDragInt("Move X animation repeat", &classInstance->repeatMoveX);
	script->AddDragInt("Move Y animation repeat", &classInstance->repeatMoveY);
	script->AddCheckBox("Move animation in loop", &classInstance->loopMove);
	script->AddCheckBox("Move animation active", &classInstance->animationMove);

	script->AddDragFloat("Distance move X", &classInstance->distanceX);
	script->AddDragFloat("Speed move X", &classInstance->speedX);
	script->AddDragFloat("Time Stoped X", &classInstance->stopTimeRight);
	script->AddDragFloat("Distance move Y", &classInstance->distanceY);
	script->AddDragFloat("Speed move Y", &classInstance->speedY);
	script->AddDragFloat("Time Stoped Y", &classInstance->stopTimeUP);

	script->AddDragInt("Scale animation repeat", &classInstance->repeatScale);
	script->AddCheckBox("Scale animation in loop", &classInstance->loopScale);
	script->AddCheckBox("Scale animation active", &classInstance->animationScale);

	script->AddDragFloat("Scale max", &classInstance->scale);
	script->AddDragFloat("Speed Scale", &classInstance->speedScale);

	script->AddDragInt("Rotate animation repeat", &classInstance->repeatRotate);
	script->AddCheckBox("Rotate animation in loop", &classInstance->loopRotate);
	script->AddCheckBox("Rotate animation active", &classInstance->animationRotate);

	script->AddDragFloat("Rotate distance", &classInstance->rotate);
	script->AddDragFloat("Speed Rotation", &classInstance->speedRotate);

	return classInstance;
}

void AnimationMove::Start()
{
	startPosX = gameObject.GetTransform().GetLocalPosition().x;
	startPosY = gameObject.GetTransform().GetLocalPosition().y;

	startScalX = gameObject.GetTransform().GetLocalScale().x;
	startScalY = gameObject.GetTransform().GetLocalScale().y;

	startRotate = gameObject.GetTransform().GetLocalRotation().z;
}
void AnimationMove::Update()
{
	if (animationMove)
	{
		if (loopMove)
		{
			MoveAnimationX();
			MoveAnimationY();
		}
		else
		{
			if (actualRepeatMoveX < repeatMoveX * 2)
			{

				if (!stopMovingRight)
				{
					MoveAnimationX();

					if (gameObject.GetTransform().GetLocalPosition().x == startPosX)
					{
						actualRepeatMoveX++;
					}
				}
				else
				{
					stopTime += Time::GetRealTimeDeltaTime();

					if (stopTime >= stopTimeRight)
					{
						stopMovingRight = false;
						stopTime = 0;
					}
				}
			}
			if (actualRepeatMoveY < repeatMoveY * 2)
			{

				if (!stopMovingUP)
				{
					MoveAnimationY();

					if (gameObject.GetTransform().GetLocalPosition().y == startPosY)
					{
						actualRepeatMoveY++;
					}
				}
				else
				{
					stopTime += Time::GetRealTimeDeltaTime();

					if (stopTime >= stopTimeUP)
					{
						stopMovingUP = false;
						stopTime = 0;
					}
				}
				
			}
			if (actualRepeatMoveY == repeatMoveY * 2 && actualRepeatMoveX == repeatMoveX * 2)
			{
				animationMove = false;
			}
		}
		


	}

	if (animationScale)
	{
		if (loopScale)
		{
			ScaleAnimation();
		}
		else
		{
			if (actualRepeatScale < repeatScale * 4)
			{
				ScaleAnimation();

				if (gameObject.GetTransform().GetLocalScale().x <= startScalX + 0.001 && gameObject.GetTransform().GetLocalScale().x >= startScalX - 0.001)
				{
					actualRepeatScale++;
				}
			}
			else
			{
				animationScale = false;
			}
		}
	}

	if (animationRotate)
	{
		if (loopRotate)
		{
			RotateAnimation();
		}
		else
		{
			if (actualRepeatRotate < repeatRotate * 2)
			{
				RotateAnimation();

				if (gameObject.GetTransform().GetLocalRotation().z <= startRotate + 0.001 && gameObject.GetTransform().GetLocalRotation().z >= startRotate - 0.001)
				{
					actualRepeatRotate++;
				}
			}
			else
			{
				animationRotate = false;
			}
		}
	}
}

void AnimationMove::MoveAnimationX()
{
	if (startPosX - gameObject.GetTransform().GetLocalPosition().x >= distanceX)
	{
		stopMovingRight = true;
		movingRight = true;
	}
	else if (startPosX - gameObject.GetTransform().GetLocalPosition().x <= -distanceX)
	{
		stopMovingRight = true;
		movingRight = false;
	}


	if (movingRight)
	{
		gameObject.GetTransform().SetPosition(gameObject.GetTransform().GetLocalPosition().x + (0.001 * speedX), gameObject.GetTransform().GetLocalPosition().y, gameObject.GetTransform().GetLocalPosition().z);
	}
	else
	{
		gameObject.GetTransform().SetPosition(gameObject.GetTransform().GetLocalPosition().x - (0.001 * speedX), gameObject.GetTransform().GetLocalPosition().y, gameObject.GetTransform().GetLocalPosition().z);
	}
}

void AnimationMove::MoveAnimationY()
{
	if (startPosY - gameObject.GetTransform().GetLocalPosition().y >= distanceY)
	{
		stopMovingUP = true;
		movingUP = true;
	}
	else if (startPosY - gameObject.GetTransform().GetLocalPosition().y <= -distanceY)
	{
		stopMovingUP = true;
		movingUP = false;
	}


	if (movingUP)
	{
		gameObject.GetTransform().SetPosition(gameObject.GetTransform().GetLocalPosition().x, gameObject.GetTransform().GetLocalPosition().y + (0.001 * speedY), gameObject.GetTransform().GetLocalPosition().z);
	}
	else
	{
		gameObject.GetTransform().SetPosition(gameObject.GetTransform().GetLocalPosition().x, gameObject.GetTransform().GetLocalPosition().y - (0.001 * speedY), gameObject.GetTransform().GetLocalPosition().z);
	}
}

void AnimationMove::ScaleAnimation()
{
	if (startScalX - gameObject.GetTransform().GetLocalScale().x >= scale)
	{
		scaling = true;
	}
	else if (startScalX - gameObject.GetTransform().GetLocalScale().x <= -scale)
	{
		scaling = false;
	}

	if (scaling)
	{
		gameObject.GetTransform().SetScale(gameObject.GetTransform().GetLocalScale().x + (0.001 * speedScale), gameObject.GetTransform().GetLocalScale().y + (0.001 * speedScale), gameObject.GetTransform().GetLocalScale().z);
	}
	else
	{
		gameObject.GetTransform().SetScale(gameObject.GetTransform().GetLocalScale().x - (0.001 * speedScale), gameObject.GetTransform().GetLocalScale().y - (0.001 * speedScale), gameObject.GetTransform().GetLocalScale().z);
	}
}

void AnimationMove::RotateAnimation()
{
	if (startRotate - gameObject.GetTransform().GetLocalRotation().z >= rotate)
	{
		rotating = true;
	}
	else if (startRotate - gameObject.GetTransform().GetLocalRotation().z <= -rotate)
	{
		rotating = false;
	}

	if (rotating)
	{
		gameObject.GetTransform().SetRotation(gameObject.GetTransform().GetLocalRotation().x, gameObject.GetTransform().GetLocalRotation().y, gameObject.GetTransform().GetLocalRotation().z + (0.001 * speedRotate));
	}
	else
	{
		gameObject.GetTransform().SetRotation(gameObject.GetTransform().GetLocalRotation().x, gameObject.GetTransform().GetLocalRotation().y, gameObject.GetTransform().GetLocalRotation().z - (0.001 * speedRotate));
	}
}

void AnimationMove::ResetAnimationMoveX()
{
	actualRepeatMoveX = 0;
}

void AnimationMove::ResetAnimationMoveY()
{
	actualRepeatMoveY = 0;
}

void AnimationMove::ResetAnimationRotate()
{
	actualRepeatRotate = 0;
}

void AnimationMove::ResetAnimationScale()
{
	actualRepeatScale = 0;
}

void AnimationMove::PlayAnimationMoveX()
{
	animationMove = true;
}

void AnimationMove::PlayAnimationMoveY()
{
	animationMove = true;
}

void AnimationMove::PlayAnimationRotate()
{
	animationRotate = true;
}

void AnimationMove::PlayAnimationScale()
{
	animationScale = true;
}