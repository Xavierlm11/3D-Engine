#pragma once
#include "Component.h"
class CMaterial :public Component
{
public:
	CMaterial(GameObject* obj);
	virtual ~CMaterial();

	void Update() override;
};

