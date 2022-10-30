#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"

ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	//CalculateViewMatrixOpenGL();

	X = vec3(1.0f, 0.0f, 0.0f);
	Y = vec3(0.0f, 1.0f, 0.0f);
	Z = vec3(0.0f, 0.0f, 1.0f);

	Position = vec3(0.0f, 1.0f, 5.0f);
	Reference = vec3(0.0f, 0.0f, 0.0f);

	cameraFrustum.type = math::PerspectiveFrustum;
	cameraFrustum.front = float3(Z.x, Z.y, Z.z);
	cameraFrustum.up = float3(Y.x, Y.y, Y.z);
	cameraFrustum.pos = float3(Position.x, Position.y, Position.z);
	cameraFrustum.nearPlaneDistance = 0.1f;
	cameraFrustum.farPlaneDistance = 1000.0f;
	cameraFrustum.verticalFov = 60.0f * DEGTORAD;
	cameraFrustum.horizontalFov = 2.0f * atanf(tanf(cameraFrustum.verticalFov / 2.0f) * 1.6f);

	//LookAt(0, 0, 0);
	

	CalculateViewMatrixOpenGL();
	CalculateProjectionMatrixOpenGL();
	//App->camera->LookAt(vec3(0, 0, 0));
}

ModuleCamera3D::~ModuleCamera3D()
{}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");
	bool ret = true;

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");

	return true;
}

void ModuleCamera3D::UpdateFrustum() {

	cameraFrustum.pos = float3(Position.x, Position.y, Position.z);
	cameraFrustum.type = math::PerspectiveFrustum;
	cameraFrustum.front = float3(Z.x, Z.y, Z.z);
	cameraFrustum.up = float3(Y.x, Y.y, Y.z);
	cameraFrustum.pos = float3(Position.x, Position.y, Position.z);
	/*cameraFrustum.nearPlaneDistance = 0.1f;
	cameraFrustum.farPlaneDistance = 1000.0f;
	cameraFrustum.verticalFov = 60.0f * DEGTORAD;
	cameraFrustum.horizontalFov = 2.0f * atanf(tanf(cameraFrustum.verticalFov / 2.0f) * 1.6f);*/

}

void ModuleCamera3D::Rotate() {

	int dx = -App->input->GetMouseXMotion();
	int dy = -App->input->GetMouseYMotion();

	float Sensitivity = 0.15f;

	if (dx != 0)
	{
		float DeltaX = (float)dx * Sensitivity;

		X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
		Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
		Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
	}

	if (dy != 0)
	{
		float DeltaY = (float)dy * Sensitivity;

		Y = rotate(Y, DeltaY, -X);
		Z = rotate(Z, DeltaY, -X);

		if (Y.y < 0.0f)
		{
			Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
			Y = cross(Z, X);
		}
	}

}


// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
	// Implement a debug camera with keys and mouse
	// Now we can make this movememnt frame rate independant!

	vec3 newPos(0,0,0);
	float speed = 3.0f * dt;
	if(App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = 8.0f * dt;

	if(App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT) newPos += Y * speed;
	if(App->input->GetKey(SDL_SCANCODE_C) == KEY_REPEAT) newPos -= Y * speed;

	if(App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos += Z * speed;
	if(App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos -= Z * speed;


	if(App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos += X * speed;
	if(App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos -= X * speed;

	Position += newPos;
	Reference += newPos;

	// Mouse motion ----------------

	if(App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		nextRot += dt;
		if (nextRot > 0.005f) {
			nextRot = 0;
			//Rotate();
		}
		Rotate();

		/*int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		float Sensitivity = 0.25f;

		Position -= Reference;

		if(dx != 0)
		{
			float DeltaX = (float)dx * Sensitivity;

			X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
		}

		if(dy != 0)
		{
			float DeltaY = (float)dy * Sensitivity;

			Y = rotate(Y, DeltaY, X);
			Z = rotate(Z, DeltaY, X);

			if(Y.y < 0.0f)
			{
				Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
				Y = cross(Z, X);
			}
		}

		Position = Reference + Z * length(Position);*/
	}

	// Recalculate matrix -------------
	UpdateFrustum();

	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ModuleCamera3D::Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	if(!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}

	UpdateFrustum();
}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt( const vec3 &Spot)
{
	Reference = Spot;

	Z = normalize(Reference - Position);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	UpdateFrustum();
}


// -----------------------------------------------------------------
void ModuleCamera3D::Move(const vec3 &Movement)
{
	Position += Movement;
	Reference += Movement;

	UpdateFrustum();
}

// -----------------------------------------------------------------
float* ModuleCamera3D::GetViewMatrix()
{
	return &ViewMatrix;
}



// -----------------------------------------------------------------
void ModuleCamera3D::CalculateViewMatrix()
{
	ViewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -dot(X, Position), -dot(Y, Position), -dot(Z, Position), 1.0f);
	ViewMatrixInverse = inverse(ViewMatrix);
}

float4x4 *ModuleCamera3D::GetViewMatrixOpenGL()
{
	CalculateViewMatrixOpenGL();
	return ViewMatrixOpenGL;
}

void ModuleCamera3D::CalculateViewMatrixOpenGL() {

	math::float4x4 view;
	view = cameraFrustum.ViewMatrix();
	
	view.Transpose();
	ViewMatrixOpenGL = &view;
}

float4x4 *ModuleCamera3D::GetProjectionMatrixOpenGL()
{
	CalculateProjectionMatrixOpenGL();
	return ViewMatrixOpenGL;
}

void ModuleCamera3D::CalculateProjectionMatrixOpenGL() {

	static float4x4 view;
	view = cameraFrustum.ProjectionMatrix();

	view.Transpose();
	ViewMatrixOpenGL = &view;
}
