#include "Mesh.h"

MeshData::MeshData()
{
	id_indices = 0; // index in VRAM
	num_indices = 0;
	indices = nullptr;

	id_vertices = 0; // unique vertex in VRAM
	num_vertices = 0;
	vertices = nullptr;
}

MeshData::~MeshData()
{}


