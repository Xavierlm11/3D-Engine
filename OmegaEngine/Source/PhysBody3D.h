#pragma once
#ifndef __PhysBody3D_H__
#define __PhysBody3D_H__

#include <vector>

#include "MathGeoLib/include/MathGeoLib.h"
#include "MathGeoLib/include/Math/float4x4.h"
//#include "GameObject.h"

class btRigidBody;
class Module;
class GameObject;
class mat4x4;

// =================================================
struct PhysBody3D
{
	friend class ModulePhysics3D;
public:
	PhysBody3D(btRigidBody* body);
	~PhysBody3D();

	void Push(float x, float y, float z);
	void GetTransform(float* matrix) const;
	float4x4 GetColTransform(float4x4 matrix, GameObject * go);
	float4x4 Mat4x4ToFloat4x4(mat4x4 matrix);
	void SetTransform(const float* matrix) const;
	void SetColTransform(float4x4 matrix, GameObject* go);
	void SetPos(float x, float y, float z);
	
public:
	btRigidBody* body = nullptr;
	float4x4 globalTrans;
	float4x4 transformMat;
public:
	std::vector <Module*> collision_listeners;
};

#endif // __PhysBody3D_H__