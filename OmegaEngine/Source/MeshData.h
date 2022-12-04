#pragma once
#include "Globals.h"
#include "External/MathGeoLib/include/Math/float3.h"
#include "Glew/include/glew.h"
#include "Assimp/include/scene.h"
#include "MaterialData.h"
#include "glmath.h"
#include "Resource.h"
#include "MathGeoLib/include/Math/float4x4.h"
class MeshData: public Resource{

public:
	MeshData(const char* path);
	~MeshData();

	void LoadBuffers();
	void DrawMesh(GLuint textureID, float4x4 mat);
	void UnloadMesh();

	uint id_indices = 0; // index in VRAM
	uint num_indices = 0;
	uint* indices = nullptr;

	uint id_vertices = 0; // unique vertex in VRAM
	uint num_vertices = 0;
	float* vertices = nullptr;

	uint id_normals = 0;
	uint num_normals = 0;
	float* normals = nullptr;

	uint id_textureCoords = 0;
	uint num_textureCoords = 0;
	float* textureCoords = nullptr;

	

	bool hasLoadedBuffers;

	MaterialData* material = nullptr;

	Uint64 materialAttachedID = 0;

	//GameObject* obj;
};
