// CORE.Texture.cpp
//////////////////////////////////////////////////////////////////////////

#include "main.h"

namespace CORE {
	Texture::Texture() {
		this->id = -1;
		this->image = nullptr;
		this->fileName = "";
		this->width = 0;
		this->height = 0;
		this->format = 0;
		this->data = nullptr;
		this->built = false;
	}

	CORE::Texture* Texture::LoadPNG(char* a_fileName) {
		File::Data* fileData = File::LoadAsset(a_fileName);
		if (fileData == 0) return 0;
		
		Image* imgData = CORE::IMAGE::PNG::Load(fileData);
		if (imgData == 0) return 0;

		Texture* texture = new Texture();
		texture->fileName = a_fileName;
		texture->image = imgData;
		texture->data = imgData->data;
		texture->width = imgData->width;
		texture->height = imgData->height;
		texture->format = imgData->format;

		// Unload the local variable
		File::UnloadAsset(fileData);

		return texture;
	}

	void Texture::UnLoadPNG(CORE::Texture* a_texture) {
		if (a_texture == 0) return;
		CORE::IMAGE::PNG::Unload(&a_texture->image);
		delete a_texture;
	}

} // End of namespace CORE