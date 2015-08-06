// CORE.Matrix.h
//////////////////////////////////////////////////////////////////////////
#pragma once

#ifndef _CORE_MATRIX_H
#define _CORE_MATRIX_H

namespace CORE {
	extern float _ModelMatrix[16];
	extern float _ViewMatrix[16];
	extern float _ProjectionMatrix[16];
	extern float _NormalMatrix[9];

	struct Vector {
		float x, y, z;
	};

	class Matrix {
		public:
			float m[16];
			Matrix();
			Matrix(
				float r0c0, float r0c1, float r0c2, float r0c3,
				float r1c0, float r1c1, float r1c2, float r1c3,
				float r2c0, float r2c1, float r2c2, float r2c3,
				float r3c0, float r3c1, float r3c2, float r3c3
			);
			static Matrix Identity();
			static Matrix Translation(float a_x, float a_y, float a_z);
			//static Matrix InverseTranslation(float a_x, float a_y, float a_z);
			static Matrix Perspective(float a_fov, float a_w, float a_h, float a_near, float a_far);
			static Matrix Rotation(float a_g, float a_b, float a_a);
			//static Matrix InverseRotation(float a_g, float a_b, float a_a);
	};

	void GL_SetModelMatrix(const Matrix& a_matrix);
	void GL_SetViewMatrix(const Matrix& a_matrix);
	void GL_SetProjectionMatrix(const Matrix& a_matrix);
	void GL_SetNormalMatrix(const Matrix& a_matrix);
}

#endif