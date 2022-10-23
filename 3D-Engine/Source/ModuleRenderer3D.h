#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"
#include "Glew/include/glew.h"

#define MAX_LIGHTS 8

class ModuleRenderer3D : public Module
{
public:
	enum class RenderMode
	{

		NORMAL,
		WIREFRAME,
		NONE,
	};

public:
	ModuleRenderer3D(Application* app, bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	void DrawArrayCube();
	void DrawArrayIndexCube();
	void DrawDirectCube();

	void LoadModelBuffers(MeshData mesh);
	void DrawMesh(MeshData mesh);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void OnResize(int x, int y, int width, int height);
	void Draw();

	RenderMode mode;

	bool buffersLoaded;

	uint num_vertices;
	uint my_id;

	uint my_indices;
	uint elementbuffer;
	uint num_indices;
	GLuint vertexbuffer;

	float vertices[18] = 
		{ 0.f, 0.f, 0.f,
		5.f, 0.f, 0.f,
		0.f, 5.f, 0.f,

		5.f, 0.f, 0.f,
		5.f, 5.f, 0.f,
		0.f, 5.f, 0.f };

	uint indices[6] =
	{
		0,
		1,
		2,

		3,
		4,
		5
	};

	

public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;
};

