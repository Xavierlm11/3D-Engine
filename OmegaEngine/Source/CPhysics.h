#pragma once
#include "Component.h"
#include "PhysBody3D.h"
#include "Application.h"
#include "Globals.h"

class Component;
class CPhysics : public Component
{
public:

	enum class ColliderShape
	{
		BOX,
		SPHERE,
		CYLINDER,
		Count,
		NONE,
		
	};

	CPhysics(GameObject* obj);
	virtual	~CPhysics();

	void Update() override;

	void Enable() override { active = true; }

	void Disable()override { active = false; }

	bool IsEnable()override { return active; }

	void OnInspector()override;

	void CheckShapes();

	bool active = true;

public:

	ModulePhysics3D* phys;

	ColliderShape shapeSelected;
	bool isShapeSelectedTrigger[3];
	bool isShapeSelected[3];
	bool isShapeCreated[3];
	bool isStatic;
	PhysBody3D* collider;

};

