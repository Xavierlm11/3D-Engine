#pragma once
#include "Component.h"
#include "PhysBody3D.h"
#include "Application.h"
#include "Globals.h"
#include "External/MathGeoLib/include/Math/float4x4.h"
//#include "Bullet/include/BulletDynamics/ConstraintSolver/btTypedConstraint.h"

class Component;
class btTypedConstraint;
class btHingeConstraint;

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

	enum class ConstraintType
	{
		P2P,
		HINGE,
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
	void CheckConstraints();

	bool active = true;

	void CallUpdateShape();

	void CreateCollider();

	void AddColliderRelations();

	void RemoveCollider();

	void RemoveConstraint();

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

	mat4x4 glMat4x4;

	ConstraintType constraitTypeSelected;
	bool isConstraitSelected[2];
	bool isConstraitCreated[2];

	GameObject* constraintGO;
	btTypedConstraint* p2pConstraint;
	btHingeConstraint* hingeConstraint;

};

