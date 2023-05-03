#pragma once
#include "P_Module.h"
class P_ShapeModule :  public P_Module
{
	P_ShapeModule();
	~P_ShapeModule();

	void OnEditor() override;
};

