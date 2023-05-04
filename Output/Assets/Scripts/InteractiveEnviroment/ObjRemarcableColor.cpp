#include "ObjRemarcableColor.h"
HELLO_ENGINE_API_C ObjRemarcableColor* CreateObjRemarcableColor(ScriptToInspectorInterface* script)
{
	ObjRemarcableColor* classInstance = new ObjRemarcableColor();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void ObjRemarcableColor::Start()
{
	_fadeInCooldown = _fadeOutCooldown = 0;
	fading = false;

}
void ObjRemarcableColor::Update()
{

	fading ? FadeIn() : FadeOut();
	//recolor.SetColor(r, g, b, 255);
	recolor.SetColor(_r, _g, _b, 255);
}

void ObjRemarcableColor::FadeIn()
{
	if (_fadeInCooldown < fadeInTime)
	{
		_fadeInCooldown += Time::GetDeltaTime();
		_r = Lerp(1, r, _fadeInCooldown / fadeInTime);
		_g = Lerp(1, g, _fadeInCooldown / fadeInTime);
		_b = Lerp(1, b, _fadeInCooldown / fadeInTime);
		//recolor.SetColor(_r, _g, _b, 255);

	}
	else if (_fadeInCooldown >= fadeInTime)
	{
		fading = false;
		_fadeInCooldown = 0;
	}
}

void ObjRemarcableColor::FadeOut()
{
	if (_fadeOutCooldown < fadeOutTime)
	{
		_fadeOutCooldown += Time::GetDeltaTime();
		_r = Lerp(r, 1, _fadeOutCooldown / fadeOutTime);
		_g = Lerp(g, 1, _fadeOutCooldown / fadeOutTime);
		_b = Lerp(b, 1, _fadeOutCooldown / fadeOutTime);

	}
	else if (_fadeOutCooldown >= fadeOutTime)
	{
		fading = true;
		_fadeOutCooldown = 0;
	}
}


float ObjRemarcableColor::Lerp(float a, float b, float time)
{
	return a + time * (b - a);
}