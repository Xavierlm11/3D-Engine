#pragma once
#include "Component.h"
#include "glmath.h"
#include "ImGui/imgui.h"
#include <MathGeoLib/include/Math/float3.h>



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

	void TransformMatrix();

	void OnInspector() override;

	vec3 GetPos() { return pos; }
	vec3 GetRot() { return rot; }
	vec3 GetScale() { return scl; }

	void SetPos(vec3 _pos) { pos = _pos; TransformMatrix(); }
	void SetRot(vec3 _rot) {  rot = _rot; TransformMatrix();}
	void SetScale(vec3 _scl) {  scl = _scl; TransformMatrix(); }

public:

	vec3 pos={0,0,0};
	vec3 rot = { 0,0,0 };
	vec3 scl = { 1,1,1 };
	
	mat4x4 matrix;
	
};

