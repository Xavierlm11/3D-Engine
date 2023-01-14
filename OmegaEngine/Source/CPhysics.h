#pragma once
#include "Component.h"
#include "PhysBody3D.h"
#include "Application.h"
#include "Globals.h"
#include "External/MathGeoLib/include/Math/float4x4.h"
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

	void CallUpdateShape();

	void CreateCollider();

public:

	ModulePhysics3D* phys;

	ColliderShape shapeSelected;
	bool isShapeSelected[3];
	bool isShapeCreated[3];
	bool isStatic;
	PhysBody3D* collider;
	bool hasInit;

	float3 colPos = { 0,0,0 };
	float3 colRot = { 0,0,0 };
	float3 colScl = { 1,1,1 };

	float sphereRadius;
	float2 cylRadiusHeight;

	mat4x4 offsetMatrix;
	mat4x4 matrixBeforePhys;

	mat4x4 glMat4x4;

};

