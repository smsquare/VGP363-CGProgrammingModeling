// CORE.Material.h
//////////////////////////////////////////////////////////////////////////
#pragma once

#ifndef _CORE_MATERIAL_H
#define _CORE_MATERIAL_H

#include "main.h"
#include "System.Data.h"
#include "CORE.Texture.h"

#define SHADER_SOURCE(...) #__VA_ARGS__

typedef GLuint MATERIAL_ID;
#define INVALID_MATERIAL_VALUE (-1)

typedef GLint MATERIAL_ATTRIBUTE;
#define INVALID_MATERIAL_ATTRIBUTE_VALUE (-1)

typedef GLuint TEXTURE;
#define INVALID_TEXTURE_VALUE (-1)

namespace CORE {
	class Mesh;
	class Camera;

	class Material {
		public:
			MATERIAL_ID shader;
			virtual void Build(Mesh* a_mesh) = 0;
			virtual void UnBuild(Mesh* a_mesh) = 0;
			virtual void Activate(Mesh* a_mesh) = 0;
			virtual void Render(Camera* a_camera, Mesh* a_mesh) = 0;
	};

	class DefaultMaterial : public Material {
		public:
			void Build(Mesh* a_mesh);
			void UnBuild(Mesh* a_mesh);
			void Activate(Mesh* a_mesh);
			void Render(Camera* a_camera, Mesh* a_mesh);
		public:
			MATERIAL_ATTRIBUTE vertexPositionAttribute;
			MATERIAL_ATTRIBUTE vertexNormalAttribute;
			MATERIAL_ATTRIBUTE textureCoordAttribute;

			MATERIAL_ATTRIBUTE projectionMatrixUniform;
			MATERIAL_ATTRIBUTE modelMatrixUniform;
			MATERIAL_ATTRIBUTE viewMatrixUniform;

			MATERIAL_ATTRIBUTE normalMatrixUniform;
			MATERIAL_ATTRIBUTE samplerColorUniform;
			MATERIAL_ATTRIBUTE ambientColorUniform;
			MATERIAL_ATTRIBUTE lightingDirectionUniform;
			MATERIAL_ATTRIBUTE directionalColorUniform;
			MATERIAL_ATTRIBUTE alphaUniform;
	};

	DefaultMaterial* GetDefaultMaterial();
	DefaultMaterial* CreateDefaultMaterial();
	
	MATERIAL_ID GL_CreateMaterial(const char* a_vs_src, const char* a_fs_src);
	GLuint GL_CreateVertexShader(const char* a_src);
	GLuint GL_CreateFragmentShader(const char* a_src);
	GLuint GL_CompileShader(GLuint a_shader);

	void GL_EnableTexture(int a_channel, CORE::Texture* a_texture, int a_options);

}

#endif