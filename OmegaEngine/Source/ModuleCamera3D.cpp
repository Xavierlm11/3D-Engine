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
	//ScnCam = App->renderer3D->GameCam->GOcam;
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
	//if (IsWindow)
	//{

		if (App->physics->isWorldOn )
		{
			float3 newPos(0, 0, 0);
			float speed = 3.0f * dt;
			float3 ups = App->renderer3D->GameCam->GOtrans->GetPos();
			if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
				speed = 8.0f * dt;

			if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT)
				App->renderer3D->GameCam->GOtrans->GivePos({ ups.x,ups.y + speed,ups.z });
				//App->renderer3D->GameCam->GOcam->cameraFrustum.pos.y += speed;

			if (App->input->GetKey(SDL_SCANCODE_C) == KEY_REPEAT)
				App->renderer3D->GameCam->GOtrans->GivePos({ ups.x,ups.y - speed,ups.z });
				//App->renderer3D->GameCam->GOcam->cameraFrustum.pos.y -= speed;

			if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
				newPos += App->renderer3D->GameCam->GOcam->cameraFrustum.front * speed;

			if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
				newPos -= App->renderer3D->GameCam->GOcam->cameraFrustum.front * speed;


			if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
				newPos += App->renderer3D->GameCam->GOcam->cameraFrustum.WorldRight() * speed;

			if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
				newPos -= App->renderer3D->GameCam->GOcam->cameraFrustum.WorldRight() * speed;

			App->renderer3D->GameCam->GOtrans->GivePos(App->renderer3D->GameCam->GOcam->cameraFrustum.pos+newPos);
			//ScnCam->cameraFrustum.pos += newPos;
			//ScnCam->Reference += newPos;
			{
				// Mouse motion ----------------
				if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT && App->editor->selectedObj != nullptr)
				{
					float3 objectPosF = App->editor->selectedObj->GOtrans->pos;
					float3 objectPosV(objectPosF.x, objectPosF.y, objectPosF.z);
					App->renderer3D->GameCam->GOcam->LookAt(objectPosV);
					Orbit(App->renderer3D->GameCam->GOcam,objectPosF);
				}
				else if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
				{
					Rotate(App->renderer3D->GameCam->GOcam);
				}


				if (App->input->GetMouseZ() != 0 && IsWindow)
				{
					Zoom(App->renderer3D->GameCam->GOcam,speed);
				}

				if (App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT) {
					if (App->editor->selectedObj != nullptr)
					{
						float3 objectPosF = App->editor->selectedObj->GOtrans->pos;
						App->renderer3D->GameCam->GOcam->LookAt(App->editor->selectedObj->GOtrans->pos);

					}
				}

			}
		}
		else {

			if (IsWindow)
			{

				float3 newPos(0, 0, 0);
				float speed = 3.0f * dt;
				if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
					speed = 8.0f * dt;

				if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT)
					ScnCam->cameraFrustum.pos.y += speed;

				if (App->input->GetKey(SDL_SCANCODE_C) == KEY_REPEAT)
					ScnCam->cameraFrustum.pos.y -= speed;

				if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
					newPos += ScnCam->cameraFrustum.front * speed;

				if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
					newPos -= ScnCam->cameraFrustum.front * speed;


				if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
					newPos += ScnCam->cameraFrustum.WorldRight() * speed;

				if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
					newPos -= ScnCam->cameraFrustum.WorldRight() * speed;

				ScnCam->cameraFrustum.pos += newPos;
				ScnCam->Reference += newPos;
				{
					// Mouse motion ----------------
					if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT && App->editor->selectedObj != nullptr)
					{
						float3 objectPosF = App->editor->selectedObj->GOtrans->pos;
						float3 objectPosV(objectPosF.x, objectPosF.y, objectPosF.z);
						ScnCam->LookAt(objectPosV);
						Orbit(ScnCam, objectPosF);
					}
					else if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
					{
						Rotate(ScnCam);
					}


					if (App->input->GetMouseZ() != 0 && IsWindow)
					{
						Zoom(ScnCam, speed);
					}

					if (App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT) {
						if (App->editor->selectedObj != nullptr)
						{
							float3 objectPosF = App->editor->selectedObj->GOtrans->pos;
							ScnCam->LookAt(App->editor->selectedObj->GOtrans->pos);

						}
					}

				}
			}
		}
	//}

	return UPDATE_CONTINUE;
}


void ModuleCamera3D::Rotate(CCamera* cam) {

	int dx = -App->input->GetMouseXMotion();
	int dy = -App->input->GetMouseYMotion();

	float Sensitivity = 0.15f;


	Quat dir;
	cam->cameraFrustum.WorldMatrix().Decompose(float3(), dir, float3());

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

	if (App->physics->isWorldOn)
	{
		/*float3 _rot;
		float a;
		dir.Normalized();
		dir.ToAxisAngle(_rot, a);
		App->renderer3D->GameCam->GOtrans->GiveRot(App->renderer3D->GameCam->GOtrans->GetRot()+ _rot*a);
		*/
		float4x4 matrix = cam->cameraFrustum.WorldMatrix();
		matrix.SetRotatePart(dir.Normalized());
		cam->cameraFrustum.SetWorldMatrix(matrix.Float3x4Part());
	
	
	}
	else {

		float4x4 matrix = ScnCam->cameraFrustum.WorldMatrix();
		matrix.SetRotatePart(dir.Normalized());
		ScnCam->cameraFrustum.SetWorldMatrix(matrix.Float3x4Part());
	}

}

void ModuleCamera3D::Orbit(CCamera* cam,float3 target) {

	int dx = -App->input->GetMouseXMotion();
	int dy = -App->input->GetMouseYMotion();

	float Sensitivity = 0.15f;

	float distance = cam->cameraFrustum.pos.Distance(target);

	Rotate(cam);
	if (App->physics->isWorldOn)
	{
		App->renderer3D->GameCam->GOtrans->GivePos(target + (cam->cameraFrustum.front * -distance));
	}
	else {
		cam->cameraFrustum.pos = target + (cam->cameraFrustum.front * -distance);

	}
	
}

void ModuleCamera3D::Zoom(CCamera* cam,float _speed) {

	cam->cameraFrustum.pos+= ScnCam->cameraFrustum.front * App->input->GetMouseZ() * _speed*1.8;

}


