// System.Image.PNG.h
////////////////////////////////////////////////////////

#pragma once
#ifndef __SYSTEM_IMAGE_PNG_H_
#define __SYSTEM_IMAGE_PNG_H_

#include "System.File.h"
#include "System.Data.h"

#include "System.Image.h"

namespace CORE {
	namespace IMAGE {
		namespace PNG {

			Image* Load(File::Data* fileData);
			void Unload(Image **image);

		}
	}
}

#endif