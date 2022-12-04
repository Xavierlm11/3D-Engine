#pragma once
#include "Component.h"
#include "glmath.h"
#include "ImGui/imgui.h"
#include <MathGeoLib/include/Math/float3.h>
#include "MathGeoLib/include/Math/float4x4.h"
//#include "Component.h"
#include "Globals.h"
#include "Application.h"
class Component;
class CTransform : public Component
{
public:

	CTransform(GameObject* obj);
	virtual ~CTransform();

	void Enable() override { active = true; }

	 void Disable()override { active = false; }

	 bool IsEnable()override { return active ; }

	bool active = true;

	void Update() override;

	void TransformMatrix(float3 _pos, float3 _rot, float3 _scl);

	void OnInspector() override;

	float3 GetPos() { return pos; }
	float3 GetRot() { return rot; }
	float3 GetScale() { return scl; }

	void SetPos(float3 _pos) { pos = _pos; TransformMatrix(pos, rot,scl); }
	void SetRot(float3 _rot) { rot = _rot; TransformMatrix(pos, rot, scl);}
	void SetScale(float3 _scl) { scl = _scl; TransformMatrix(pos, rot, scl); }

	float4x4 GetMatrix() { return matrix; }

	float4x4 GetGlobal();
	

public:

	float3 pos={0,0,0};
	float3 rot = { 0,0,0 };
	float3 scl = { 1,1,1 };
	
	float4x4 matrix, glmatr ;
	
	
};

