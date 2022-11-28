#pragma once
#include "Component.h"
#include "glmath.h"
#include "ImGui/imgui.h"



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

public:

	vec3 pos={0,0,0};
	vec3 rot = { 0,0,0 };
	vec3 scl = { 1,1,1 };

	mat4x4 matrix;
};

