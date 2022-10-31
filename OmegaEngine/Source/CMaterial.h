#pragma once
#include "Component.h"
class CMaterial :public Component
{
public:
	CMaterial(GameObject* obj);
	virtual ~CMaterial();

	void Update() override;

	void HasMeses();

	void Enable() override { active = true; }

	void Disable()override { active = false; }

	bool IsEnable()override { return active; }

	bool active = false;
};

