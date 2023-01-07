#pragma once
#include "Component.h"
#include "glmath.h"
#include "ImGui/imgui.h"
#include <MathGeoLib/include/Math/float3.h>

#include "GameObject.h"

#include "External/MathGeoLib/include/Math/float4x4.h"



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

	void SetGlobalTrans();

	void OnInspector() override;

	//mat4x4 GlobalMatrix();

	void SetPos(float3 _pos);

	void UpdatePos();

	void SetRot(float3 _rot);

	void UpdateRot();

	void SetScale(float3 _scl);

	void UpdateScl();

	void SetGPos(float3 _pos);

	void SetGRot(float3 _rot);

	void SetGScl(float3 _scl);

	float3 GetPos() { return pos; }
	float3 GetRot() { return rot; }
	float3 GetScale() { return scl; }

	

	//float4x4 GetMatrix() { return matrix; }

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
	//float4x4 matrix;
private:
	float3 ppos = { 0,0,0 };
	float3 prot = { 0,0,0 };
	float3 pscl = { 1,1,1 };

	float3 gpos = { 0,0,0 };
	float3 grot = { 0,0,0 };
	float3 gscl = { 1,1,1 };
};

