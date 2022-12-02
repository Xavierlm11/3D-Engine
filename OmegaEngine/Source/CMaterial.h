#pragma once
#include "Component.h"
//#include "MaterialData.h"

class MeshData;
class Component;
class MaterialData;

class CMaterial :public Component
{
public:
	CMaterial(GameObject* obj);
	virtual ~CMaterial();

	void Update() override;

	MaterialData* GetMaterial();

	void Enable() override { active = true; }

	void Disable()override { active = false; }

	bool IsEnable()override { return active; }

	bool active = true;
public:

	MaterialData* materialData = nullptr;
};

