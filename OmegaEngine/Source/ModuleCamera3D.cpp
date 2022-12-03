#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "CCamera.h"
ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	//CalculateViewMatrixOpenGL();

	//X = vec3(1.0f, 0.0f, 0.0f);
	//Y = vec3(0.0f, 1.0f, 0.0f);
	//Z = vec3(0.0f, 0.0f, 1.0f);

	//Position = vec3(0.0f, 1.0f, 5.0f);
	//Reference = vec3(0.0f, 0.0f, 0.0f);

	//cameraFrustum.type = math::PerspectiveFrustum;
	//cameraFrustum.verticalFov = camFOV * DEGTORAD;
	//cameraFrustum.horizontalFov = 2.0f * atanf(tanf(cameraFrustum.verticalFov / 2.0f) * 1.6f);
	//cameraFrustum.nearPlaneDistance = 0.01f;
	//cameraFrustum.farPlaneDistance = 1000.0f;
	//cameraFrustum.pos = float3(Position.x, Position.y, Position.z);
	//cameraFrustum.front = float3(Z.x, Z.y, Z.z);
	//cameraFrustum.up = float3(Y.x, Y.y, Y.z);

	////LookAt(0, 0, 0);
	//

	//CalculateViewMatrixOpenGL();
	//CalculateProjectionMatrixOpenGL();
	
}

ModuleCamera3D::~ModuleCamera3D()
{}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");
	bool ret = true;
	//App->camera->LookAt(vec3(0, 0, 0));

	//ScnCam = new CCamera(App->scene->RootParent);
	ScnCam = new CCamera(NULL);
	ScnCam->cameraFrustum.pos = float3(0, 0, -10);
	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");
	ScnCam = nullptr;
	delete ScnCam;
	return true;
}
update_status ModuleCamera3D::Update(float dt)
{
	// Implement a debug camera with keys and mouse
	// Now we can make this movememnt frame rate independant!

	float3 newPos(0,0,0);
	float speed = 3.0f * dt;
	if(App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = 8.0f * dt;

	if(App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT) 
		ScnCam->cameraFrustum.pos.y +=  speed;

	if(App->input->GetKey(SDL_SCANCODE_C) == KEY_REPEAT) 
		ScnCam->cameraFrustum.pos.y -=  speed;

	if(App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) 
		newPos += ScnCam->cameraFrustum.front * speed;

	if(App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) 
		newPos -= ScnCam->cameraFrustum.front * speed;


	if(App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) 
		newPos += ScnCam->cameraFrustum.WorldRight() * speed;

	if(App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) 
		newPos -= ScnCam->cameraFrustum.WorldRight() * speed;

	ScnCam->cameraFrustum.pos += newPos;
	ScnCam->Reference += newPos;
	{
		// Mouse motion ----------------
		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT && App->editor->selectedObj!=nullptr)
		{
			float3 objectPosF = App->editor->selectedObj->GOtrans->pos;
			float3 objectPosV(objectPosF.x, objectPosF.y, objectPosF.z);
			ScnCam->LookAt(objectPosV);
			//Orbit(objectPosF);
		}
		else if(App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
		{
			Rotate();
		}


		if (App->input->GetMouseZ() != 0 && IsWindow)
		{
			Zoom(speed);
		}


			// Recalculate matrix -------------
			////UpdateFrustum();
	}

	return UPDATE_CONTINUE;
}
//
//void ModuleCamera3D::UpdateFrustum() {
//
//
//
//
//	cameraFrustum.pos = float3(Position.x, Position.y, Position.z);
//	cameraFrustum.type = math::PerspectiveFrustum;
//	cameraFrustum.front = float3(Z.x, Z.y, Z.z);
//	cameraFrustum.up = float3(Y.x, Y.y, Y.z);
//	cameraFrustum.pos = float3(Position.x, Position.y, Position.z);
//
//	cameraFrustum.verticalFov = camFOV * DEGTORAD;
//	//cameraFrustum.horizontalFov = 2.0f * atanf(tanf(cameraFrustum.verticalFov / 2.0f) * App->window->winWidth/ App->window->winHeight);
//	cameraFrustum.horizontalFov = 2.0f * atanf(tanf(cameraFrustum.verticalFov / 2.0f) * aspRatio);
//	/*cameraFrustum.nearPlaneDistance = 0.1f;
//	cameraFrustum.farPlaneDistance = 1000.0f;
//	cameraFrustum.verticalFov = 60.0f * DEGTORAD;
//	cameraFrustum.horizontalFov = 2.0f * atanf(tanf(cameraFrustum.verticalFov / 2.0f) * 1.6f);*/
//	glMatrixMode(GL_PROJECTION);
//	glLoadMatrixf((GLfloat*)App->camera->GetProjectionMatrixOpenGL());
//	GetGlLoadMat();
//
//	//CalculateViewMatrixOpenGL();
//	//CalculateProjectionMatrixOpenGL();
//}
//
//GLfloat* ModuleCamera3D::GetGlLoadMat()
//{
//
//	return (GLfloat*)App->camera->GetProjectionMatrixOpenGL();
//}
//
// 

void ModuleCamera3D::Rotate() {

	int dx = -App->input->GetMouseXMotion();
	int dy = -App->input->GetMouseYMotion();

	float Sensitivity = 0.15f;

		/*if (dx != 0)
		{
			float AngleX = (float)dx * Sensitivity;

			X = rotate(X, AngleX, vec3(0.0f, 1.0f, 0.0f));
			Y = rotate(Y, AngleX, vec3(0.0f, 1.0f, 0.0f));
			Z = rotate(Z, AngleX, vec3(0.0f, 1.0f, 0.0f));
		}

		if (dy != 0)
		{
			float AngleY = (float)dy * Sensitivity;

			Y = rotate(Y, AngleY, -X);
			Z = rotate(Z, AngleY, -X);

			if (Y.y < 0.0f)
			{
				Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
				Y = cross(Z, X);
			}
		}*/

	Quat dir;
	ScnCam->cameraFrustum.WorldMatrix().Decompose(float3(), dir, float3());

	if (dy != 0) {
		float AngleY = (float)dy * Sensitivity;

		Quat Y = { 0, 0, 0, 1 };
		Y.SetFromAxisAngle(float3(1.0f, 0.0f, 0.0f), AngleY * DEGTORAD);

		dir = dir * Y;
	}

	if (dx != 0) {
		float AngleX = (float)dx * Sensitivity;

		Quat X = {0, 0, 0, 1};
		X.SetFromAxisAngle(float3(0.0f, 1.0f, 0.0f), AngleX * DEGTORAD);

		dir = X * dir;
	}

	float4x4 matrix = ScnCam->cameraFrustum.WorldMatrix();
	matrix.SetRotatePart(dir.Normalized());
	ScnCam->cameraFrustum.SetWorldMatrix(matrix.Float3x4Part());

}
//
//	UpdateFrustum();
//
//}
//
//void ModuleCamera3D::Orbit(float3 target) {
//
//	int dx = -App->input->GetMouseXMotion();
//	int dy = -App->input->GetMouseYMotion();
//
//	float Sensitivity = 0.15f;
//
//	float distance = cameraFrustum.pos.Distance(target);
//
//	if (dx != 0)
//	{
//		float DeltaX = (float)dx * Sensitivity;
//
//		X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
//		Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
//		Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
//	}
//
//	if (dy != 0)
//	{
//		float DeltaY = (float)dy * Sensitivity;
//
//		Y = rotate(Y, DeltaY, -X);
//		Z = rotate(Z, DeltaY, -X);
//
//		if (Y.y < 0.0f)
//		{
//			Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
//			Y = cross(Z, X);
//		}
//	}
//
//	//UpdateFrustum();
//	UpdateFrustum();
//	
//	float3 posFloat = target - distance * cameraFrustum.front.Normalized();
//	posFloat = target - distance * cameraFrustum.front.Normalized();
//	Position.x = posFloat.x;
//	Position.y = posFloat.y;
//	Position.z = posFloat.z;
//	UpdateFrustum();
//}
//

// -----------------------------------------------------------------

void ModuleCamera3D::Zoom(float _speed) {

	//vec3 frontVec = (cameraFrustum.front.x, cameraFrustum.front.y, cameraFrustum.front.z);
	ScnCam->cameraFrustum.pos+= ScnCam->cameraFrustum.front * App->input->GetMouseZ() * _speed*1.5;
	//Reference += Z * App->input->GetMouseZ() * 1.5f;

	//cameraFrustum.nearPlaneDistance += App->input->GetMouseZ() * 0.8f;
	//cameraFrustum.nearPlaneDistance += 2;

	//UpdateFrustum();
	//CalculateViewMatrixOpenGL();
	

	/*glMatrixMode(GL_PROJECTION);
	glLoadMatrixf((GLfloat*)GetProjectionMatrixOpenGL());
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();*/

}
//
//// -----------------------------------------------------------------
//void ModuleCamera3D::Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference)
//{
//	this->Position = Position;
//	this->Reference = Reference;
//
//	Z = normalize(Position - Reference);
//	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
//	Y = cross(Z, X);
//
//	if(!RotateAroundReference)
//	{
//		this->Reference = this->Position;
//		this->Position += Z * 0.05f;
//	}
//
//	UpdateFrustum();
//}
//
//// -----------------------------------------------------------------
//void ModuleCamera3D::LookAt( const vec3 &Spot)
//{
//	Reference = Spot;
//
//	Z = normalize(Reference - Position);
//	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
//	Y = cross(Z, X);
//
//	UpdateFrustum();
//}
//
//
//// -----------------------------------------------------------------
//void ModuleCamera3D::Move(const vec3 &Movement)
//{
//	Position += Movement;
//	Reference += Movement;
//
//	UpdateFrustum();
//}
//
//// -----------------------------------------------------------------
//float* ModuleCamera3D::GetViewMatrix()
//{
//	return &ViewMatrix;
//}
//
//
//
//// -----------------------------------------------------------------
//void ModuleCamera3D::CalculateViewMatrix()
//{
//	ViewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -dot(X, Position), -dot(Y, Position), -dot(Z, Position), 1.0f);
//	ViewMatrixInverse = inverse(ViewMatrix);
//}
//
//float4x4 *ModuleCamera3D::GetViewMatrixOpenGL()
//{
//	CalculateViewMatrixOpenGL();
//	return ViewMatrixOpenGL;
//}
//
//void ModuleCamera3D::CalculateViewMatrixOpenGL() {
//
//	math::float4x4 view;
//	view = cameraFrustum.ViewMatrix();
//	
//	view.Transpose();
//	ViewMatrixOpenGL = &view;
//}
//
//float4x4 *ModuleCamera3D::GetProjectionMatrixOpenGL()
//{
//	CalculateProjectionMatrixOpenGL();
//	return ViewMatrixOpenGL;
//}
//
//void ModuleCamera3D::CalculateProjectionMatrixOpenGL() {
//
//	static float4x4 view;
//	view = cameraFrustum.ProjectionMatrix();
//
//	view.Transpose();
//	ViewMatrixOpenGL = &view;
//}
