#include "CCamera.h"

CCamera::CCamera(GameObject* obj) :Component(obj, Types::CAMERA)
{
	X = vec3(1.0f, 0.0f, 0.0f);
	Y = vec3(0.0f, 1.0f, 0.0f);
	Z = vec3(0.0f, 0.0f, 1.0f);

	Position = vec3(0.0f, 1.0f, 5.0f);
	Reference = vec3(0.0f, 0.0f, 0.0f);

	cameraFrustum.type = math::PerspectiveFrustum;
	cameraFrustum.verticalFov = camFOV * DEGTORAD;
	cameraFrustum.horizontalFov = 2.0f * atanf(tanf(cameraFrustum.verticalFov / 2.0f) * 1.6f);
	cameraFrustum.nearPlaneDistance = 0.01f;
	cameraFrustum.farPlaneDistance = 1000.0f;
	cameraFrustum.pos = float3(Position.x, Position.y, Position.z);
	cameraFrustum.front = float3(Z.x, Z.y, Z.z);
	cameraFrustum.up = float3(Y.x, Y.y, Y.z);


	width = External->window->winWidth;
	height = External->window->winHeight;

	NewFrBuffer();
	
	//CalculateViewMatrixOpenGL();
	//CalculateProjectionMatrixOpenGL();
}

CCamera::~CCamera()
{
	glDeleteFramebuffers(1, &NewFrameBuffer);
	glDeleteFramebuffers(1, &CCBuffer);
	glDeleteFramebuffers(1, &ObjRenderBuffer);
}

void CCamera::Update()
{
	//GO.
}

void CCamera::NewFrBuffer()
{
	glGenFramebuffers(1, &NewFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, NewFrameBuffer);

	//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenTextures(1, &CCBuffer);
	glBindTexture(GL_TEXTURE_2D, CCBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);


	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, CCBuffer, 0);

	// create render buffer object
	glGenRenderbuffers(1, &ObjRenderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, ObjRenderBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, ObjRenderBuffer);


	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	/*glGenerateMipmap(GL_TEXTURE_2D);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CHECKERS_WIDTH, CHECKERS_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkerImage);

	glBindTexture(GL_TEXTURE_2D, 0);*/
}

void CCamera::UpdateFrustum()
{

}

GLfloat* CCamera::GetGlLoadMat()
{
	return (GLfloat*)GetProjectionMatrixOpenGL();
}

void CCamera::Rotate()
{

}

void CCamera::Look(const vec3& _Position, const vec3& _Reference, bool _RotateAroundReference)
{
	Position = _Position;
	Reference = _Reference;
	LookAt(Reference);
}

void CCamera::LookAt(const vec3& Spot)
{
	//Reference = Spot;
	//cameraFrustum.front = (Reference - cameraFrustum.pos).Normalized();
}

void CCamera::Move(const vec3& Movement)
{
}

void CCamera::Zoom()
{
}

void CCamera::Orbit(float3 target)
{
}

float* CCamera::GetViewMatrix()
{
	return nullptr;
}

float4x4* CCamera::GetViewMatrixOpenGL()
{
	return nullptr;
}

float4x4* CCamera::GetProjectionMatrixOpenGL()
{
	return nullptr;
}

void CCamera::CalculateViewMatrix()
{
}

void CCamera::CalculateViewMatrixOpenGL()
{
}

void CCamera::CalculateProjectionMatrixOpenGL()
{
}
