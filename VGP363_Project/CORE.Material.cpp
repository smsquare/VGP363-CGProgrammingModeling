// CORE.Material.cpp
//////////////////////////////////////////////////////////////////////////

#include "main.h"

#define MAX_TEXTURE_CHANNELS 10

namespace CORE {
	
	CORE::Texture* _textureChannel[MAX_TEXTURE_CHANNELS] = {0};

	const char* FRAGMENT_SHADER_DEFAULT = SHADER_SOURCE
		(
			precision mediump float;
			
			varying vec2 vTextureCoord;
			varying vec3 vLightWeighting;

			uniform sampler2D uSamplerColor;
			uniform float uAlpha;

			void main(void) {
				gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
			}

		);

	const char* VERTEX_SHADER_DEFAULT = SHADER_SOURCE
		(
			attribute vec3 aVertexPosition;
			attribute vec3 aVertexNormal;
			attribute vec2 aTextureCoord;

			uniform mat4 uModelMatrix;
			uniform mat4 uViewMatrix;
			uniform mat4 uProjectionMatrix;
			uniform mat3 uNormalMatrix;

			uniform vec3 uAmbientColor;

			uniform vec3 uLightingDirection;
			uniform vec3 uDirectionColor;

			varying vec2 vTextureCoord;
			varying vec3 vLightWeighting;

			void main(void) {

				vec4 worldPos = uModelMatrix * vec4(aVertexPosition, 1.0);
				vec4 cameraPos = uViewMatrix * worldPos;
				vec4 screenPos = uProjectionMatrix * cameraPos;
				vTextureCoord = aTextureCoord;
				gl_Position = screenPos;

			}

		);
		/*
			Probably need to add these back into the vertex shader at some point
				
				vec3 transformedNormal = mat3(uViewMatrix * uModelMatrix) * aVertexNormal;
				float directionLightWeighting = max(dot(transformedNormal, uLightingDirection), 0.0);
				vLightWeighting = uAmbientColor + uDirectionalColor * directionalLightWeighting;

		*/
	// GL_CreateMaterial : Creates a material given the vertex and fragment shader sources.
	MATERIAL_ID GL_CreateMaterial(const char* a_vs_src, const char* a_fs_src) {
		GLuint fragmentShader = GL_CreateFragmentShader(a_fs_src);
		GLuint vertexShader = GL_CreateVertexShader(a_vs_src);

		GLuint material = glCreateProgram();

		glAttachShader(material, vertexShader);
		glAttachShader(material, fragmentShader);
		glLinkProgram(material);

		GLint result;
		glGetProgramiv(material, GL_LINK_STATUS, &result);
		if (result == 0) {
			CDebug::ErrorLog("ERROR: Material creation failed.\n");
			return -1;
		}
		
		return material;
	}

	// GL_CreateVertexShader : Builds a vertex shader given the source and returns it.
	GLuint GL_CreateVertexShader(const char* a_src) {
		GLuint shader = glCreateShader(GL_VERTEX_SHADER);

		int length = strlen(a_src);
		const GLchar* item = (const GLchar*)a_src;
		glShaderSource(shader, 1, &item, &length);

		return GL_CompileShader(shader);
	}

	// GL_CreateVertexShader : Builds a fragment shader given the source and returns it.
	GLuint GL_CreateFragmentShader(const char* a_src) {
		GLuint shader = glCreateShader(GL_FRAGMENT_SHADER);

		int length = strlen(a_src);
		const GLchar* item = (const GLchar*)a_src;
		glShaderSource(shader, 1, &item, &length);
		
		return GL_CompileShader(shader);
	}

	// GL_CompileShader : Internal function used to compile vertex and fragment shaders.
	GLuint GL_CompileShader(GLuint a_shader) {
		glCompileShader(a_shader);

		GLint result;
		glGetShaderiv(a_shader, GL_COMPILE_STATUS, &result);
		if (result == 0) {
			char infoLog[255] = {""};
			int length = 0;
			glGetShaderInfoLog(a_shader, 255, &length, infoLog);

			CDebug::ErrorLog("ERROR: Failed to compile shader:\n");
			CDebug::ErrorLog(infoLog);
			CDebug::ErrorLog("\n");
			return -1;
		}

		return a_shader;
	}

	// GL_EnableTexture: a_channel = multitexturing channel
	//					 a_texture = texture RGB data and info
	//					 a_options = misc. user optionss
	void GL_EnableTexture(int a_channel, CORE::Texture* a_texture, int a_options) {
		if (a_channel < 0) a_channel = 0;
		if (a_channel > MAX_TEXTURE_CHANNELS-1) a_channel = MAX_TEXTURE_CHANNELS-1;
		if (a_texture == 0) return;

		_textureChannel[a_channel] = a_texture;
		if (a_texture->built == false) {
			glGenTextures(1, (GLuint*)a_texture->id);
			glBindTexture(GL_TEXTURE_2D, a_texture->id);
			glTexImage2D(GL_TEXTURE_2D, 0, a_texture->format, a_texture->width, a_texture->height, 0, a_texture->format, GL_UNSIGNED_BYTE, a_texture->data);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
			glGenerateMipmap(GL_TEXTURE_2D);
			//TODO: CONTINUE FROM STEP 19
		}
	}


	void DefaultMaterial::Build(Mesh* a_mesh) {
		Mesh& mesh = *a_mesh;
		if (mesh.built == true) {
			return;
		}

		mesh.built = true;

		// Build vertex data
		if (mesh.vertexBuffer.data.floats != 0) {
			glGenBuffers(1, &mesh.vertexBuffer.id);
			glBindBuffer(GL_ARRAY_BUFFER, mesh.vertexBuffer.id);
			glBufferData(GL_ARRAY_BUFFER, mesh.vertexBuffer.numItems * mesh.vertexBuffer.itemSize * sizeof(float), mesh.vertexBuffer.data.floats, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
		
		// Build normal data
		if (mesh.normalBuffer.data.floats != 0) {
			glGenBuffers(1, &mesh.normalBuffer.id);
			glBindBuffer(GL_ARRAY_BUFFER, mesh.normalBuffer.id);
			glBufferData(GL_ARRAY_BUFFER, mesh.normalBuffer.numItems * mesh.normalBuffer.itemSize * sizeof(float), mesh.normalBuffer.data.floats, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		// Build texture data
		if (mesh.textureCoordBuffer.data.floats != 0) {
			glGenBuffers(1, &mesh.textureCoordBuffer.id);
			glBindBuffer(GL_ARRAY_BUFFER, mesh.textureCoordBuffer.id);
			glBufferData(GL_ARRAY_BUFFER, mesh.textureCoordBuffer.numItems * mesh.textureCoordBuffer.itemSize * sizeof(float), mesh.textureCoordBuffer.data.floats, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		// Build index data
		if (mesh.indexBuffer.data.ushorts != 0) {
			glGenBuffers(1, &mesh.indexBuffer.id);
			glBindBuffer(GL_ARRAY_BUFFER, mesh.indexBuffer.id);
			glBufferData(GL_ARRAY_BUFFER, mesh.indexBuffer.numItems * mesh.indexBuffer.itemSize * sizeof(short), mesh.indexBuffer.data.ushorts, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		
	}

	void DefaultMaterial::UnBuild(Mesh* a_mesh) {
		if (a_mesh->vertexBuffer.data.floats != 0) {
			delete a_mesh->vertexBuffer.data.floats;
		}

		if (a_mesh->normalBuffer.data.floats != 0) {
			delete a_mesh->normalBuffer.data.floats;
		}

		if (a_mesh->textureCoordBuffer.data.floats != 0) {
			delete a_mesh->textureCoordBuffer.data.floats;
		}

		if (a_mesh->indexBuffer.data.ushorts != 0) {
			delete a_mesh->indexBuffer.data.ushorts;
		}

		if (a_mesh->vertexBuffer.id != 0) {
			glDeleteBuffers(1, &a_mesh->vertexBuffer.id);
		}

		if (a_mesh->normalBuffer.id != 0) {
			glDeleteBuffers(1, &a_mesh->normalBuffer.id);
		}

		if (a_mesh->textureCoordBuffer.id != 0) {
			glDeleteBuffers(1, &a_mesh->textureCoordBuffer.id);
		}

		if (a_mesh->indexBuffer.id != 0) {
			glDeleteBuffers(1, &a_mesh->indexBuffer.id);
		}

		a_mesh->built = false;
	}

	void DefaultMaterial::Activate(Mesh* a_mesh) {
		glUseProgram(this->shader);
		this->vertexPositionAttribute = glGetAttribLocation(this->shader, "aVertexPosition");
		glEnableVertexAttribArray(this->vertexPositionAttribute);
		this->vertexNormalAttribute = glGetAttribLocation(this->shader, "aVertexNormal");
		glEnableVertexAttribArray(this->vertexNormalAttribute);
		this->textureCoordAttribute = glGetAttribLocation(this->shader, "aTextureCoord");
		glEnableVertexAttribArray(this->textureCoordAttribute);
		
		this->projectionMatrixUniform = glGetUniformLocation(this->shader, "uProjectionMatrix");
		this->modelMatrixUniform = glGetUniformLocation(this->shader, "uModelMatrix");
		this->viewMatrixUniform = glGetUniformLocation(this->shader, "uViewMatrix");
		
		this->normalMatrixUniform = glGetUniformLocation(this->shader, "uNormalMatrix");
		
		this->alphaUniform = glGetUniformLocation(this->shader, "uAlpha");
		this->samplerColorUniform = glGetUniformLocation(this->shader, "uSamplerColor");

		this->lightingDirectionUniform = glGetUniformLocation(this->shader, "uLightingDirection");
		this->directionalColorUniform = glGetUniformLocation(this->shader, "uDirectionalColor");
	}

	void DefaultMaterial::Render(Camera* a_camera, Mesh* a_mesh) {
		Mesh& mesh = *a_mesh;
		GLBufferObject vertexBuffer = mesh.vertexBuffer;
		GLBufferObject normalBuffer = mesh.normalBuffer;
		GLBufferObject textureCoordBuffer = mesh.textureCoordBuffer;
		GLBufferObject indexBuffer = mesh.indexBuffer;
		
		this->Activate(a_mesh);

		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer.id);
		glVertexAttribPointer(
			this->vertexPositionAttribute,
			a_mesh->vertexBuffer.itemSize,
			GL_FLOAT,
			false,
			0,
			0
		);

		glBindBuffer(GL_ARRAY_BUFFER, a_mesh->normalBuffer.id);
		glVertexAttribPointer(
			this->vertexNormalAttribute,
			a_mesh->normalBuffer.itemSize,
			GL_FLOAT,
			false,
			0,
			0
		);

		glBindBuffer(GL_ARRAY_BUFFER, a_mesh->textureCoordBuffer.id);
		glVertexAttribPointer(
			this->textureCoordAttribute,
			a_mesh->textureCoordBuffer.itemSize,
			GL_FLOAT, 
			false,
			0,
			0
		);

		glUniform3f(this->ambientColorUniform, 0.15f, 0.15f, 0.15f);
		glUniform3f(this->lightingDirectionUniform, 0.0f, 0.0f, 1.0f);
		glUniform3f(this->directionalColorUniform, 0.88f, 0.88f, 0.88f);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer.id);

		glUniformMatrix4fv(this->projectionMatrixUniform, 1, false, (GLfloat*)_ProjectionMatrix);
		glUniformMatrix4fv(this->modelMatrixUniform, 1, false, (GLfloat*)_ModelMatrix);
		glUniformMatrix4fv(this->viewMatrixUniform, 1, false, (GLfloat*)_ViewMatrix);

		glDrawElements(GL_TRIANGLES, indexBuffer.numItems, GL_UNSIGNED_SHORT, 0);
	}

	DefaultMaterial* _defaultMaterial = 0;
	
	DefaultMaterial* GetDefaultMaterial() {
		if (_defaultMaterial == 0) {
			_defaultMaterial = CreateDefaultMaterial();
		}
		return _defaultMaterial;
	}

	DefaultMaterial* CreateDefaultMaterial() {
		DefaultMaterial* defaultMaterial = new DefaultMaterial();
		defaultMaterial->shader = GL_CreateMaterial(VERTEX_SHADER_DEFAULT, FRAGMENT_SHADER_DEFAULT);
		return defaultMaterial;
	}

	void DeleteMaterial(Material* a_material) {

	}


}