#pragma once
#include "Component.h"
#include "glmath.h"
#include "ImGui/imgui.h"
#include <MathGeoLib/include/Math/float3.h>
#include "External/MathGeoLib/include/Math/float4x4.h"
#include "GameObject.h"

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

	void TransformMatrix1(float3 _pos, float3 _rot, float3 _scl);

	void OnInspector() override;


	float3 GetPos() { return pos; }
	float3 GetRot() { return rot; }
	float3 GetScale() { return scl; }

	void SetPos(float3 _pos) { pos = _pos; TransformMatrix(pos, rot,scl); }
	void SetRot(float3 _rot) { rot = _rot; TransformMatrix(pos, rot, scl);}
	void SetScale(float3 _scl) { scl = _scl; TransformMatrix(pos, rot, scl); }


	void SetPos1(float3 _pos) { pos = _pos; TransformMatrix1(pos, rot, scl); }
	void SetRot1(float3 _rot) { rot = _rot; TransformMatrix1(pos, rot, scl); }
	void SetScale1(float3 _scl) { scl = _scl; TransformMatrix1(pos, rot, scl); }
	float4x4 GetMatrix() { return matrix1; }

	float4x4 GetGlobal();
	/*float4x4 GetGlobal()
	{
		if (GO->GetParent() == nullptr)
		{
			return;
		}
		return ;
	}*/

public:

	float3 pos={0,0,0};
	float3 rot = { 0,0,0 };
	float3 scl = { 1,1,1 };
	
	mat4x4 matrix;
	float4x4 matrix1;
	
};

