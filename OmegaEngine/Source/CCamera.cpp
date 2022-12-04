#include "CCamera.h"

CCamera::CCamera(GameObject* obj) :Component(obj, Types::CAMERA)
{
	X = float3(1.0f, 0.0f, 0.0f);
	Y = float3(0.0f, 1.0f, 0.0f);
	Z = float3(0.0f, 0.0f, 1.0f);

	Position = float3(0.0f, 1.0f, 5.0f);
	Reference = float3(0.0f, 0.0f, 0.0f);

	cameraFrustum.type = math::PerspectiveFrustum;
	cameraFrustum.verticalFov = camFOV * DEGTORAD;
	cameraFrustum.horizontalFov = 2.0f * atanf(tanf(cameraFrustum.verticalFov / 2.0f) * 1.6f);
	cameraFrustum.nearPlaneDistance = 0.01f;
	cameraFrustum.farPlaneDistance = 1000.0f;
	cameraFrustum.pos = float3(Position.x, Position.y, Position.z);
	cameraFrustum.front = float3(Z.x, Z.y, Z.z);
	cameraFrustum.up = float3(Y.x, Y.y, Y.z);


	

	NewFrBuffer();
	
	//CalculateViewMatrixOpenGL();
	//CalculateProjectionMatrixOpenGL();
}

CCamera::~CCamera()
{
	glDeleteFramebuffers(1, &NewFrameBuffer);
	glDeleteTextures(1, &CCBuffer);
	glDeleteRenderbuffers(1, &ObjRenderBuffer);
}

void CCamera::Update()
{
	//GO.
}

void CCamera::NewFrBuffer()
{

	width = External->window->winWidth;
	height = External->window->winHeight;

	glDeleteFramebuffers(1, &NewFrameBuffer);
	glDeleteTextures(1, &CCBuffer);
	glDeleteRenderbuffers(1, &ObjRenderBuffer);

	glGenFramebuffers(1, (GLuint*)&NewFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, NewFrameBuffer);

	//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenTextures(1, (GLuint*)&CCBuffer);
	glBindTexture(GL_TEXTURE_2D, CCBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);


	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, CCBuffer, 0);

	
	glGenRenderbuffers(1, (GLuint*)&ObjRenderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, ObjRenderBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, ObjRenderBuffer);


	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	/*glGenerateMipmap(GL_TEXTURE_2D);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CHECKERS_WIDTH, CHECKERS_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkerImage);

	glBindTexture(GL_TEXTURE_2D, 0);*/
}

void CCamera::SetRatio( float ratio)
{
	cameraFrustum.horizontalFov = 2.0f * atanf(tanf(cameraFrustum.verticalFov / 2.0f) * ratio);
	cameraFrustum.verticalFov =  DEGTORAD * camFOV ;
}

void CCamera::LoadBuffer(int _width, int _height)
{
	SetRatio((float)_width/ (float)_height);
	NewFrBuffer();

}

//void CCamera::UpdateFrustum()
//{
//
//}

GLfloat* CCamera::GetGlLoadMatCC()
{
	return (GLfloat*)GetProjectionMatrixOpenGL();
}

float* CCamera::GetViewMatrixOpenGL()
{
	CalculateViewMatrixOpenGL();
	return ViewMatrixOpenGL->ptr();
}

void CCamera::CalculateViewMatrixOpenGL() {

	math::float4x4 view;
	view = cameraFrustum.ViewMatrix();

	view.Transpose();
	ViewMatrixOpenGL = &view;
}

float* CCamera::GetProjectionMatrixOpenGL()
{
	CalculateProjectionMatrixOpenGL();
	return ProjectionMatrixOpenGL->ptr();
}

void CCamera::CalculateProjectionMatrixOpenGL() {

	static float4x4 view;
	view = cameraFrustum.ProjectionMatrix();

	view.Transpose();
	ProjectionMatrixOpenGL = &view;
}

unsigned int CCamera::GetCCamBuffer()
{
	return CCBuffer;
}

unsigned int CCamera::GetFrameBuffer()
{
	return NewFrameBuffer;
}

//void CCamera::Rotate()
//{
//
//}

void CCamera::Look(const float3& _Position, const float3& _Reference, bool _RotateAroundReference)
{
	cameraFrustum.pos = _Position;
	Reference = _Reference;
	LookAt(Reference);
}

void CCamera::LookAt(const float3& Spot)
{
	float3 _y = {0, 1, 0};
	Reference = Spot;
	cameraFrustum.front = (Reference - cameraFrustum.pos).Normalized();
	X = _y.Cross(cameraFrustum.front).Normalized();
	cameraFrustum.up = cameraFrustum.front.Cross(X);

}

void CCamera::Move(const float3& Movement)
{
	cameraFrustum.pos += Movement;
}

//void CCamera::Zoom()
//{
// 
//}

//void CCamera::Orbit(float3 target)
//{
//}

//float* CCamera::GetViewMatrix()
//{
//	return nullptr;
//}


//void CCamera::CalculateViewMatrix()
//{
//}
//void CCamera::CalculateViewMatrixOpenGL()
//{
//}
//
//void CCamera::CalculateProjectionMatrixOpenGL()
//{
//}
