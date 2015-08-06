// System.Image.PNG.cpp
////////////////////////////////////////////////////////////////
#include "main.h"
#include "System.Image.PNG.h"
#include "System.Data.h"
#include "System.File.h"
#include "System.Image.h"
#include "./Library/libpng/png.h"

namespace CORE {
	namespace IMAGE {
		namespace PNG {

			/* PNG LOADING FUNCTIONALITY PROVIDED BY THE BOOK:
			GFX Lightweight OpenGLES 2.0 Game and Graphics Engine

			Copyright (C) 2011 Romain Marucchi-Foino http://gfx.sio2interactive.com

			This software is provided 'as-is', without any express or implied warranty.
			In no event will the authors be held liable for any damages arising from the use of
			this software. Permission is granted to anyone to use this software for any purpose,
			including commercial applications, and to alter it and redistribute it freely,
			subject to the following restrictions:

			1. The origin of this software must not be misrepresented; you must not claim that
			you wrote the original software. If you use this software in a product, an acknowledgment
			in the product would be appreciated but is not required.

			2. Altered source versions must be plainly marked as such, and must not be misrepresented
			as being the original software.

			3. This notice may not be removed or altered from any source distribution.
			*/
			#ifndef MAX_PATH
			#define MAX_PATH 260
			#endif
			#define MEMORY File::Data
			typedef struct
			{
				char			name[ MAX_PATH ];
				unsigned int	tid;
				unsigned short	width;
				unsigned short	height;
				unsigned char	byte;
				unsigned int	size;
				unsigned int	target;
				unsigned int	internal_format;
				unsigned int	format;
				unsigned int	texel_type;
				unsigned char	*texel_array;
				unsigned int	n_mipmap;
				unsigned int	compression;
		
			} PNGFILE;
			void LoadPNG(PNGFILE *texture, File::Data *fileData);
			Image* Load(File::Data* fileData) {
				Image *img = new Image();
				PNGFILE *fileInfo = new PNGFILE();
				fileInfo->target = GL_TEXTURE_2D;
				LoadPNG(fileInfo, fileData);
				img->data = fileInfo->texel_array;
				img->width = fileInfo->width;
				img->height = fileInfo->height;
				img->format = fileInfo->format;
				img->fileInfo = fileInfo;
				return img;
			}
			void UnloadPNG(PNGFILE **texture);
			void Unload(Image **image) {
				if(image == NULL) return;
				if(*image == NULL) return;
				Image &img = **image;
				UnloadPNG((PNGFILE**)&img.fileInfo);
				if(img.fileInfo != NULL) {
					delete img.fileInfo;
					img.fileInfo = NULL;
				}
				delete *image;
				*image = NULL;
			}
			unsigned int mread(MEMORY *memory, void *dst, unsigned int size)
			{
				if( ( memory->position + size ) > memory->size )
				{ size = memory->size - memory->position; }
				memcpy( dst, &memory->buffer[ memory->position ], size );
				memory->position += size;
				return size;
			}
			void png_memory_read(png_structp structp, png_bytep bytep, png_size_t size)
			{
				MEMORY *m = (MEMORY *) png_get_io_ptr(structp);
				mread(m, bytep, size);
			}
			void UnloadPNG(PNGFILE **texture) {
				if(texture == NULL) return;
				if(*texture == NULL) return;
				PNGFILE &tex = **texture;
				if(tex.texel_array != NULL) {
					free(tex.texel_array);
				}
				tex.texel_array = NULL;
			}
			void LoadPNG(PNGFILE *texture, MEMORY *memory)
			{
				png_structp structp;
				png_infop infop;

				png_bytep *bytep = NULL;

				unsigned int i = 0;
	
				int n, png_bit_depth, png_color_type;

				structp = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

				infop = png_create_info_struct(structp);

				png_set_read_fn(structp, (png_voidp *)memory, png_memory_read);

				png_read_info(structp, infop);

				png_bit_depth = png_get_bit_depth(structp, infop);
	
				png_color_type = png_get_color_type(structp, infop);

				if(png_color_type == PNG_COLOR_TYPE_PALETTE)
				{ png_set_expand(structp); }

				if(png_color_type == PNG_COLOR_TYPE_GRAY && png_bit_depth < 8)
				{ png_set_expand(structp); }
	
				if(png_get_valid(structp, infop, PNG_INFO_tRNS) )
				{ png_set_expand(structp); }
	
				if(png_bit_depth == 16)
				{ png_set_strip_16(structp); }
	
				if(png_color_type == PNG_COLOR_TYPE_GRAY || png_color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
				{ png_set_gray_to_rgb(structp); }

				png_read_update_info(structp, infop);

				png_get_IHDR( structp,
							  infop,
							  ( png_uint_32 * )( &texture->width  ),
							  ( png_uint_32 * )( &texture->height ),
							  &png_bit_depth,
							  &png_color_type,
							  NULL, NULL, NULL );

				switch( png_color_type )
				{
					case PNG_COLOR_TYPE_GRAY:
					{
						texture->byte			 = 1;
						texture->internal_format = 
						texture->format		     = GL_LUMINANCE;
			
						break;
					}

					case PNG_COLOR_TYPE_GRAY_ALPHA:
					{
						texture->byte			 = 2;
						texture->internal_format = 
						texture->format		     = GL_LUMINANCE_ALPHA;
					
						break;
					}

					case PNG_COLOR_TYPE_RGB:
					{
						texture->byte			 = 3;
						texture->internal_format = 
						texture->format		     = GL_RGB;
			
						break;
					}

					case PNG_COLOR_TYPE_RGB_ALPHA:
					{
						texture->byte			 = 4;
						texture->internal_format = 
						texture->format		     = GL_RGBA;
			
						break;
					}
				}

				texture->texel_type = GL_UNSIGNED_BYTE;
				texture->size = texture->width * texture->height * texture->byte;
				texture->texel_array = ( unsigned char * ) malloc( texture->size );
				bytep = ( png_bytep * ) malloc( texture->height * sizeof( png_bytep ) );
	
				while( i != texture->height )
				{
					n = texture->height - ( i + 1 );
					bytep[ n ] = texture->texel_array + ( n * texture->width * texture->byte );
					++i;
				}	

				png_read_image(structp, bytep);
				png_read_end(structp, NULL);

				png_destroy_read_struct(&structp, &infop, NULL);

				free(bytep);
			}

		}
	}
}