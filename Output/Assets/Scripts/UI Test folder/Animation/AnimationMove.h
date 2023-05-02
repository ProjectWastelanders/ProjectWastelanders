#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class AnimationMove : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	void MoveAnimationX();
	void MoveAnimationY();
	void ScaleAnimation();
	void RotateAnimation();

	void ResetAnimationMoveX();
	void ResetAnimationMoveY();
	void ResetAnimationScale();
	void ResetAnimationRotate();

	void PlayAnimationMoveX();
	void PlayAnimationMoveY();
	void PlayAnimationScale();
	void PlayAnimationRotate();

	API_GameObject gameObject;

	bool animationMove = false;
	bool animationScale = false;
	bool animationRotate = false;

	float distanceX = 0;
	float distanceY = 0;
	float scale = 0;
	float rotate = 0;

	float speedX= 0;
	float speedY= 0;

	float speedScale= 0;

	float speedRotate= 0;

	float startPosX;
	float startPosY;

	float startScalX;
	float startScalY;

	float startRotate;

	//true is up or right, false is oposite
	bool movingUP = true;
	bool movingRight = true;
	bool scaling = true;
	bool rotating = true;

	bool loopMove = false;
	bool loopScale = false;
	bool loopRotate = false;
	int repeatMoveX = 0;
	int repeatMoveY = 0;
	int repeatScale = 0;
	int repeatRotate = 0;
	int actualRepeatMoveX = 0;
	int actualRepeatMoveY = 0;
	int actualRepeatScale = 0;
	int actualRepeatRotate = 0;
};

