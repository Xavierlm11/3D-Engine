#include "MaterialImporter.h"

#include "DevIL/include/ilu.h"
#include "DevIL/include/ilut.h"
#include "DevIL/include/il.h"


void MaterialImporter::LoadTextureLump(const char* buffer, uint size){

	if (ilLoadL(IL_TYPE_UNKNOWN, (const void*)buffer, size)==true)
	{
		LOG("Correct ilLoadL");
	}
}

Uint64 MaterialImporter::Save(char** fileBuffer) {

	ILuint size;
	ILubyte* data;

	ilSetInteger(IL_DXTC_FORMAT, IL_DXT5); // To pick a specific DXT compression use
	size = ilSaveL(IL_DDS, nullptr, 0); // Get the size of the data buffer
	//LOG("Error: %s", ilGetError());

	if (size > 0) {
		data = new ILubyte[size]; // allocate data buffer
		if (ilSaveL(IL_DDS, data, size) > 0) // Save to buffer with the ilSaveIL function
		{
			*fileBuffer = (char*)data;
			
		} 
			
	}

	return size;
}

void MaterialImporter::Load(const char* fileBuffer, MaterialData* materialData, uint size) {

	ILuint texID;

	ilGenImages(1, &texID);
	ilBindImage(texID);

	if (ilLoadL(IL_TYPE_UNKNOWN, (const void*)fileBuffer, size) == true) {
		LOG("Texture applied!");
	}
	else {
		LOG("Texture NOT applied!");
	}

	GLuint texIDGL = ilutGLBindTexImage();

	glBindTexture(GL_TEXTURE_2D, texIDGL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	///
	//GLuint texID;
	//GLuint texture;


	//////////glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	//////////texID = ilutGLBindTexImage();

	//////////glBindTexture(GL_TEXTURE_2D, texID);
	//////////glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	//////////glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	//////////glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//////////glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	////////////glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CHECKERS_WIDTH, CHECKERS_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, texImage);
	//////////glGenerateMipmap(GL_TEXTURE_2D);
	//////////glBindTexture(GL_TEXTURE_2D, 0);

	//////////ilDeleteImages(1, &texID);

	//glBindTexture(GL_TEXTURE_2D, 0);

	///


	materialData->texture_id = texIDGL;

	ilDeleteImages(1, &texID);




	

}