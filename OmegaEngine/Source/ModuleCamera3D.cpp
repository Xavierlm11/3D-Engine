#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "CCamera.h"
ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{

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
	ScnCam->cameraFrustum.pos = float3(0,4, -10);
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
			Orbit(objectPosF);
		}
		else if(App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
		{
			Rotate();
		}


		if (App->input->GetMouseZ() != 0 && IsWindow)
		{
			Zoom(speed);
		}


	}

	return UPDATE_CONTINUE;
}


void ModuleCamera3D::Rotate() {

	int dx = -App->input->GetMouseXMotion();
	int dy = -App->input->GetMouseYMotion();

	float Sensitivity = 0.15f;


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

void ModuleCamera3D::Orbit(float3 target) {

	int dx = -App->input->GetMouseXMotion();
	int dy = -App->input->GetMouseYMotion();

	float Sensitivity = 0.15f;

	float distance = ScnCam->cameraFrustum.pos.Distance(target);

	Rotate();
	ScnCam->cameraFrustum.pos = target + (ScnCam->cameraFrustum.front * -distance);
	
}
//

// -----------------------------------------------------------------

void ModuleCamera3D::Zoom(float _speed) {

	//vec3 frontVec = (cameraFrustum.front.x, cameraFrustum.front.y, cameraFrustum.front.z);
	ScnCam->cameraFrustum.pos+= ScnCam->cameraFrustum.front * App->input->GetMouseZ() * _speed*1.8;
	

}


