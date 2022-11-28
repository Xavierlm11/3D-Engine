#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"
#include "Glew/include/glew.h"
#include <vector>
#include "CMeshes.h"
#define MAX_LIGHTS 8

#define CHECKERS_HEIGHT 100
#define CHECKERS_WIDTH 100

class CMesh;

class ModuleRenderer3D : public Module
{
public:
	enum class RenderMode
	{
		NORMAL,
		CHECKERS,
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

	void LoadTextureBuffers();
	void DrawMesh(MeshData* mesh);
	update_status PostUpdate(float dt);
	void GoRender();
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

	float vertices[24] = 
		{ 0.f, 0.f, 0.f,
		5.f, 0.f, 0.f,
		5.f, 5.f, 0.f,
		0.f, 5.f, 0.f,
		
		0.f, 0.f, -5.f,
		5.f, 0.f, -5.f,
		5.f, 5.f, -5.f,
		0.f, 5.f, -5.f
	};

	uint indices[36] =
	{
		0,1,2,
		2,3,0,

		1,5,2,
		2,5,6,

		5,4,7,
		7,6,5,

		4,0,3,
		3,7,4,

		3,2,6,
		6,7,3,

		0,4,5,
		5,1,0




		
	};

	GLuint GetBuffCam();

	bool hasLoadedMesh;


	//Textures
	GLuint checkersID;
	GLubyte checkerImage[CHECKERS_HEIGHT][CHECKERS_WIDTH][4];

	GLuint houseTexID;

	std::vector<CMesh*> meshlist;

public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	//mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;
};

