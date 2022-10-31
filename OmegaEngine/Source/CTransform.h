#pragma once
#include "Component.h"
class CTransform : public Component
{
public:
	CTransform(GameObject* obj);
	virtual ~CTransform();

	void Enable() override { active = true; }

	 void Disable()override { active = false; }

	 bool IsEnable()override { return active ; }

	bool active = false;

	void Update() override;

};

