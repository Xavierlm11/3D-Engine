#pragma once
#include "Component.h"
class CMeshes : public Component
{
public:

	CMeshes(GameObject* obj);
	virtual	~CMeshes();

	void Update() override;

	void MeshRenderer();

};

