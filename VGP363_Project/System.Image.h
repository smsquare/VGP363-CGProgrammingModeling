// System.Image.h
////////////////////////////////////////////////////////

#pragma once
#ifndef __SYSTEM_IMAGE_H_
#define __SYSTEM_IMAGE_H_

#include "System.File.h"
#include "System.Data.h"

namespace CORE {

	class Image {
	public:
		Image() { memset(this, 0, sizeof(Image)); }
		int width, height;
		unsigned int format;
		void* data;
		void* fileInfo;
	};

}

#endif