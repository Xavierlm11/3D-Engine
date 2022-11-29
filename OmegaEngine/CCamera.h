#pragma once
///#include "Component.h"
#include "Source/Component.h"
class CCamera : public Component
{
public:
	CCamera(GameObject* obj);
	~CCamera();

	void Update() override;

	void Enable() override { active = true; }

	void Disable()override { active = false; }

	bool IsEnable()override { return active; }

	//void OnInspector() override;

	bool active = true;

};

