#include "MeshImporter.h"

void MeshImporter::Import(const aiMesh* mesh, MeshData* meshData) {

	//copy vertices
	meshData->num_vertices = mesh->mNumVertices;
	meshData->vertices = new float[meshData->num_vertices * 3];

	memcpy(meshData->vertices, mesh->mVertices, sizeof(float) * meshData->num_vertices * 3);

	LOG("New mesh with %d vertices", meshData->num_vertices);

	//copy faces
	if (mesh->HasFaces())
	{
		meshData->num_indices = mesh->mNumFaces * 3;
		meshData->indices = new uint[meshData->num_indices]; // assume each face is a triangle
		for (uint i = 0; i < mesh->mNumFaces; ++i)
		{
			if (mesh->mFaces[i].mNumIndices != 3) {
				LOG("WARNING, geometry face with != 3 indices!");
			}
			else
			{
				memcpy(&meshData->indices[i * 3], mesh->mFaces[i].mIndices, 3 * sizeof(uint));
			}
		}
	}

	//copy normals
	if (mesh->HasNormals()) {
		meshData->num_normals = mesh->mNumVertices;
		meshData->normals = new float[meshData->num_normals * 3];

		memcpy(meshData->normals, mesh->mNormals, sizeof(float) * meshData->num_normals * 3);
	}

	//copy texture coords
	if (mesh->HasTextureCoords(0)) {
		meshData->num_textureCoords = mesh->mNumVertices;
		meshData->textureCoords = new float[mesh->mNumVertices * 2];

		for (unsigned int i = 0; i < meshData->num_textureCoords; i++) {
			meshData->textureCoords[i * 2] = mesh->mTextureCoords[0][i].x;
			meshData->textureCoords[i * 2 + 1] = mesh->mTextureCoords[0][i].y;
		}
	}

	

}

char* MeshImporter::Save(const MeshData* mesh, uint& size){ //char** buffer) {
	// amount of indices / vertices / normals / texture_coords
	uint ranges[4] = { mesh->num_indices, mesh->num_vertices, mesh->num_normals, mesh->num_textureCoords};
	
	size = sizeof(ranges) + (sizeof(uint) * mesh->num_indices) + (sizeof(float) * mesh->num_vertices * 3) + (sizeof(float) * mesh->num_normals * 3) + (sizeof(float) * mesh->num_textureCoords * 2);
	
	char* fileBuffer = new char[size]; // Allocate
	char* cursor = fileBuffer;

	uint bytes = sizeof(ranges); // First store ranges
	memcpy(cursor, ranges, bytes);
	cursor += bytes;
	
	// Store indices
	bytes = sizeof(uint) * mesh->num_indices;
	memcpy(cursor, mesh->indices, bytes);
	cursor += bytes;

	// Store vertices
	bytes = sizeof(float) * mesh->num_vertices;
	memcpy(cursor, mesh->vertices, bytes);
	cursor += bytes;

	// Store normals
	bytes = sizeof(float) * mesh->num_normals;
	memcpy(cursor, mesh->normals, bytes);
	cursor += bytes;

	// Store texture_coords
	bytes = sizeof(float) * mesh->num_textureCoords;
	memcpy(cursor, mesh->textureCoords, bytes);
	cursor += bytes;

	return fileBuffer;
}

void MeshImporter::Load(const char* buffer, MeshData* meshData) {

	
	const char* cursor = buffer;

	// amount of indices / vertices / normals / texture_coords
	uint ranges[4];
	uint bytes = sizeof(ranges);

	//char* fileBuffer = new char[bytes]; // Allocatew
	

	memcpy(ranges, cursor, bytes);
	cursor += bytes;

	meshData->num_indices = ranges[0];
	meshData->num_vertices = ranges[1];
	meshData->num_normals = ranges[2];
	meshData->num_textureCoords = ranges[3];

	// Load indices
	bytes = sizeof(uint) * meshData->num_indices;
	meshData->indices = new uint[meshData->num_indices];
	memcpy(meshData->indices, cursor, bytes);
	cursor += bytes;

	// Load vertices
	bytes = sizeof(float) * meshData->num_vertices;
	meshData->vertices = new float[meshData->num_vertices];
	memcpy(meshData->vertices, cursor, bytes);
	cursor += bytes;

	// Load normals
	bytes = sizeof(float) * meshData->num_normals;
	meshData->normals = new float[meshData->num_normals];
	memcpy(meshData->normals, cursor, bytes);
	cursor += bytes;

	// Load texture_coords
	bytes = sizeof(float) * meshData->num_textureCoords;
	meshData->textureCoords = new float[meshData->num_textureCoords];
	memcpy(meshData->textureCoords, cursor, bytes);
	cursor += bytes;
}