#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "MathGeoLib/include/MathGeoLib.h"

#include"MathGeoLib/include/Math/float4x4.h"
#include "MathGeoLib/include/Geometry/Frustum.h"

//#include "CCamera.h"

class CCamera;

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(Application* app, bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	//void UpdateFrustum();
	//GLfloat* GetGlLoadMat();
	void Rotate();
	//void Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference = false);
	//void LookAt(const vec3 &Spot);
	//void Move(const vec3 &Movement);
	void Zoom();
	void Orbit(float3 target);

	//float* GetViewMatrix();
	//float4x4 *GetViewMatrixOpenGL();
	//float4x4 *GetProjectionMatrixOpenGL();

	//void CalculateViewMatrix();
	//void CalculateViewMatrixOpenGL();

	//void CalculateProjectionMatrixOpenGL();

public:
	
	/*vec3 X, Y, Z, Position, Reference;
	Color background;

	float camFOV = 60.0f, aspRatio = 1.6f;*/

	CCamera* ScnCam=nullptr ;

	bool IsWindow = false;
	
private:
	/*mat4x4 ViewMatrix, ViewMatrixInverse;
	float4x4* ViewMatrixOpenGL;
	float4x4* ProjectionMatrixOpenGL;
	Frustum cameraFrustum;*/

	float nextRot = 0;

	bool isMovingAround = false;

	
};