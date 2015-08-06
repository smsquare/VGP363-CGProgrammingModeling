// APP.Main.cpp
//////////////////////////////////////////////////////////////////////////

#include "APP.Main.h"
#include "CORE.Mesh.h"
#include "CORE.Material.h"
#include "CORE.Time.h"
using namespace CORE;

namespace APP {
	
	Mesh* g_Mesh = NULL;
	float g_RotX = 0.0f;
	float g_RotY = 0.0f;
	float g_RotZ = 0.0f;

	void OnInitialize() {
		Mesh* tmpMesh = new Mesh();
		tmpMesh->AllocateVertices(3);
		tmpMesh->SetVertex(0, MeshVertex(-0.75f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f));
		tmpMesh->SetVertex(1, MeshVertex(0.75f, 0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 1.0f, 0.0f));
		tmpMesh->SetVertex(2, MeshVertex(0.0f, 0.75f, 0.0f, 0.5f, 1.0f, -1.0f, 0.5f, 1.0f));
		tmpMesh->material = GetDefaultMaterial();	
		tmpMesh->texture = CORE::Texture::LoadPNG("Resources\REFERENCE_GRASS_PHOTO.png");
		tmpMesh->Build();
		g_Mesh = tmpMesh;
		GL_SetModelMatrix(Matrix::Identity());
		GL_SetViewMatrix(Matrix::Identity());
		GL_SetProjectionMatrix(Matrix::Identity());
		GL_SetNormalMatrix(Matrix::Identity());
	}

	void OnFrame() {
		
		float rotx = g_RotX;
		float roty = g_RotY;
		float rotz = g_RotZ;

		rotx += CORE::TIME::DeltaTime * 0.1f;
		roty += CORE::TIME::DeltaTime * 0.2f;
		rotz += CORE::TIME::DeltaTime * 0.3f;
		GL_SetModelMatrix(Matrix::Rotation(rotx, roty, rotz));
		g_RotX = rotx; 
		g_RotY = roty; 
		g_RotZ = rotz; 
		g_Mesh->Render(NULL);
		CORE::TIME::Update();
	}
}