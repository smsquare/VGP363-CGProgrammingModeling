// CORE.Mesh.h
//////////////////////////////////////////////////////////////////////////
#pragma once

#ifndef _CORE_MESH_H
#define _CORE_MESH_H

#include "main.h"
#include "System.Data.h"
#include "CORE.Texture.h"

namespace CORE {
	class Camera;
	class Material;

	typedef union DATA_BUFFER {
		float* floats;
		unsigned short* ushorts;
		int* ints;
	};

	typedef struct GLBufferObject {
		GLBufferObject() { memset(this, 0, sizeof(GLBufferObject)); }
		GLuint id;
		int itemSize;
		int numItems;
		DATA_BUFFER data;
	};

	class MeshVertex {
		public:
			MeshVertex(float x, float y, float z, float nx, float ny, float nz, float tu, float tv) {
				this->x = x;
				this->y = y;
				this->z = z;
				this->w = 1.0f;
				this->nx = nx;
				this->ny = ny;
				this->nz = nz;
				this->nw = 0.0f;
				this->tu = tu;
				this->tv = tv;
				this->gu = 0.0f;				
				this->gv = 0.0f;
			}
			float x, y, z, w;
			float nx, ny, nz, nw;
			float tu, tv, gu, gv;
	};

	class Mesh {
		public:
			Mesh() { memset(this, 0, sizeof(Mesh)); this->alpha = 1.0f; }
			Material* material;
			Texture* texture;
			float alpha;
			bool built;

			DATA::ValueList<MeshVertex> points;
			GLBufferObject vertexBuffer;
			GLBufferObject normalBuffer;
			GLBufferObject textureCoordBuffer;
			GLBufferObject indexBuffer;

			void AllocateVertices(int a_count);
			void SetVertex(int a_index, const MeshVertex& a_vertex);
			void Build();
			void Render(Camera* a_camera);
	};

}

#endif