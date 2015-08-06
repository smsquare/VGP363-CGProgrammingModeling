// CORE.Mesh.cpp
//////////////////////////////////////////////////////////////////////////

#include "main.h"

namespace CORE {

	void Mesh::AllocateVertices(int a_count) {
		this->vertexBuffer.itemSize = 3;
		this->vertexBuffer.numItems = a_count;
		this->vertexBuffer.data.floats = new float[vertexBuffer.itemSize * vertexBuffer.numItems];
	    
		this->normalBuffer.itemSize = 3;
		this->normalBuffer.numItems = a_count;
		this->normalBuffer.data.floats = new float[normalBuffer.itemSize * normalBuffer.numItems];

		this->textureCoordBuffer.itemSize = 2;
		this->textureCoordBuffer.numItems = a_count;
		this->textureCoordBuffer.data.floats = new float[textureCoordBuffer.itemSize * textureCoordBuffer.numItems];

		this->indexBuffer.itemSize = 1;
		this->indexBuffer.numItems = a_count;
		this->indexBuffer.data.ushorts = new unsigned short[indexBuffer.numItems];
	
		// Initialize index buffer data;
		for (int n = 0; n < a_count; ++n) {
			this->indexBuffer.data.ushorts[n] = n;
		}
	}

	void Mesh::SetVertex(int a_index, const MeshVertex& a_vertex) {
		this->vertexBuffer.data.floats[a_index*3 + 0] = a_vertex.x;
		this->vertexBuffer.data.floats[a_index*3 + 1] = a_vertex.y;
		this->vertexBuffer.data.floats[a_index*3 + 2] = a_vertex.z;
		this->normalBuffer.data.floats[a_index*3 + 0] = a_vertex.nx;
		this->normalBuffer.data.floats[a_index*3 + 1] = a_vertex.ny;
		this->normalBuffer.data.floats[a_index*3 + 2] = a_vertex.nz;
		this->textureCoordBuffer.data.floats[a_index*2 + 0] = a_vertex.tu;
		this->textureCoordBuffer.data.floats[a_index*2 + 1] = a_vertex.tv;
	}

	void Mesh::Build() {
		if (this->material != 0) {
			this->material->Build(this);
		} else {
			CDebug::ErrorLog("ERROR: Mesh cannot be built until it's material is assigned!!\n");
		}
	}

	void Mesh::Render(Camera* a_camera) {
		if (this->material != 0) {
			//TODO: Invoke this mesh's material's Render function
			this->material->Render(a_camera, this);
		}
	}

} // End of namespace CORE