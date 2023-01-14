#include "MeshData.h"
#include "glmath.h"
#include "External/MathGeoLib/include/Math/float3.h"
#include "Glew/include/glew.h"

MeshData::MeshData(const char* path) : Resource(path, Resource::Types::MESH)
{
	id_indices = 0; // index in VRAM
	num_indices = 0;
	indices = nullptr;

	id_vertices = 0; // unique vertex in VRAM
	num_vertices = 0;
	vertices = nullptr;

	id_normals = 0;
	num_normals = 0;
	normals = nullptr;

	id_textureCoords = 0;
	num_textureCoords = 0;
	textureCoords = nullptr;

	material = nullptr;

	hasLoadedBuffers = false;

	//assetName = path;
}

MeshData::~MeshData()
{
	indices = nullptr;
	delete indices;

	vertices = nullptr;
	delete vertices;

	normals = nullptr;
	delete normals;

	textureCoords = nullptr;
	delete textureCoords;

	material = nullptr;
	delete material;
}


void MeshData::LoadBuffers() {

	if (hasLoadedBuffers == false) {
		glGenBuffers(1, (GLuint*)&(id_vertices));
		glBindBuffer(GL_ARRAY_BUFFER, id_vertices);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_vertices * 3, &vertices[0], GL_STATIC_DRAW);

		glGenBuffers(1, (GLuint*)&(id_indices));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_indices);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * num_indices, &indices[0], GL_STATIC_DRAW);

		glGenBuffers(1, (GLuint*)&(id_normals));
		glBindBuffer(GL_ARRAY_BUFFER, id_normals);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_normals * 3, &normals[0], GL_STATIC_DRAW);

		glGenBuffers(1, (GLuint*)&(id_textureCoords));
		glBindBuffer(GL_ARRAY_BUFFER, id_textureCoords);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_textureCoords * 2, &textureCoords[0], GL_STATIC_DRAW);

		LOG("Mesh Loaded! Mesh indices: %i. Mesh vertices: %i", num_indices, num_vertices);
		hasLoadedBuffers = true;
	}
}

void MeshData::UnloadMesh() {

	if (hasLoadedBuffers == true) {

		if (id_indices != NULL) {
			glDeleteBuffers(1, &id_indices);
		}

		if (id_vertices != NULL) {
			glDeleteBuffers(1, &id_vertices);
		}

		if (id_normals != NULL) {
			glDeleteBuffers(1, &id_normals);
		}

		if (id_textureCoords != NULL) {
			glDeleteBuffers(1, &id_textureCoords);
		}

		hasLoadedBuffers = false;
	}

	if (indices != nullptr) {
		delete[] indices;
	}

	if (vertices != nullptr) {
		delete[] vertices;
	}

	if (normals != nullptr) {
		delete[] normals;
	}

	if (textureCoords != nullptr) {
		delete[] textureCoords;
	}

	indices = nullptr;
	vertices = nullptr;
	normals = nullptr;
	textureCoords = nullptr;

}

void MeshData::DrawMesh(GLuint textureID, mat4x4 mat) {
	if (hasLoadedBuffers == true) {

		//if (textureID != NULL) {
			glBindTexture(GL_TEXTURE_2D, textureID);
		//}
		//draw vertex
		glEnableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, id_vertices);
		glVertexPointer(3, GL_FLOAT, 0, NULL);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_indices);


		glEnableClientState(GL_NORMAL_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, id_normals);
		glNormalPointer(GL_FLOAT, 0, NULL);

		//draw textutes
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, id_textureCoords);
		glTexCoordPointer(2, GL_FLOAT, 0, NULL);

		//estas 2 funciones hacen el trnasform
		//glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glMultMatrixf(&mat);

		glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, NULL);
		//if (textureID != NULL) {

		glPopMatrix();
			glBindTexture(GL_TEXTURE_2D, 0);
		//}
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDisableClientState(GL_VERTEX_ARRAY);

		//if (num_textures != 0) {
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		//}
		//LOG("Mesh Loaded! Num indices: %i. Num vertices: %i. ID Indices: %i. ID Vertices: %i.", num_indices, num_vertices, id_indices, id_vertices);


		//LOG("Mesh Loaded! Num indices: %i. Num vertices: %i. ID Indices: %i. ID Vertices: %i.", num_indices, num_vertices, id_indices, id_vertices);


	}
}

