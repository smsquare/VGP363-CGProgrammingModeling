// CORE.Texture.h
//////////////////////////////////////////////////////////////////////////
#pragma once

#ifndef _CORE_TEXTURE_H
#define _CORE_TEXTURE_H

#include "CORE.Material.h"
#include "System.File.h"
#include "System.Image.h"
#include "System.Data.h"

namespace CORE {
	class Texture {
	public:
		Texture();
		static CORE::Texture* LoadPNG(char* a_fileName);
		static void UnLoadPNG(CORE::Texture* a_texture);
	public:
		CORE::Image* image;
		char* fileName;
		int id;
		int width, height;
		int format;
		(void*) data;
		bool built;
	};

} // End of namespace CORE

#endif