#pragma once

#include "Component.h"
#include "Globals.h"
#include "glmath.h"

#include"MathGeoLib/include/Math/float4x4.h"
#include "MathGeoLib/include/Geometry/Frustum.h"

#include "Glew/include/glew.h"
#include "Color.h"
#include "Application.h"
class CCamera : public Component
{
public:
	CCamera(GameObject* obj);
	~CCamera();

	void Update() override;

	void Enable() override { active = true; }

	void Disable()override { active = false; }

	bool IsEnable()override { return active; }


	void OnInspector() override;
	//void OnInspector() override;

	bool active = true;


public:
	
	GLfloat* GetGlLoadMatCC();
	
	void Look(const float3& _Position, const float3& _Reference, bool _RotateAroundReference = false);
	void LookAt(const float3& Spot);
	void Move(const float3& Movement);
	
	float* GetViewMatrixOpenGL();
	float* GetProjectionMatrixOpenGL();



	unsigned int GetCCamBuffer();

	unsigned int GetFrameBuffer();

	void NewFrBuffer();

	void SetRatio(float ratio);

	void SetFOV(float FOV);

	void LoadBuffer(int _width, int _height);

public:

	float3 X, Y, Z, Position, Reference;
	Color background;

	float camFOV = 60.0f, aspRatio = 1.6f;
	unsigned int NewFrameBuffer, CCBuffer, ObjRenderBuffer;

	Frustum cameraFrustum;

	bool ismain;
private:
	mat4x4 ViewMatrix, ViewMatrixInverse;
	float4x4* ViewMatrixOpenGL;
	float4x4* ProjectionMatrixOpenGL;

	float nextRot = 0;

	bool isMovingAround = false;

	uint width, height;
};

