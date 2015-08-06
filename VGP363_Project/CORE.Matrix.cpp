// CORE.Matrix.cpp
//////////////////////////////////////////////////////////////////////////

#include "main.h"

namespace CORE {
	float _ModelMatrix[16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
	float _ViewMatrix[16]= {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
	float _ProjectionMatrix[16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
	float _NormalMatrix[9] = {1, 0, 0, 0, 1, 0, 0, 0, 1};

	Matrix::Matrix() {
		memset(this->m, 0.0, sizeof(float) * 16);
	}

	Matrix::Matrix(
		float r0c0, float r0c1, float r0c2, float r0c3,
		float r1c0, float r1c1, float r1c2, float r1c3,
		float r2c0, float r2c1, float r2c2, float r2c3,
		float r3c0, float r3c1, float r3c2, float r3c3
	) {
		this->m[0] = r0c0;
		this->m[1] = r0c1;
		this->m[2] = r0c2;
		this->m[3] = r0c3;

		this->m[4] = r1c0;
		this->m[5] = r1c1;
		this->m[6] = r1c2;
		this->m[7] = r1c3;

		this->m[8] = r2c0;
		this->m[9] = r2c1;
		this->m[10] = r2c2;
		this->m[11] = r2c3;

		this->m[12] = r3c0;
		this->m[13] = r3c1;
		this->m[14] = r3c2;
		this->m[15] = r3c3;
	}

	Matrix Matrix::Identity() {
		return Matrix(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
			);
	}
	Matrix Matrix::Translation(float a_x, float a_y, float a_z) {
		return Matrix(
			1, 0, 0, a_x,
			0, 1, 0, a_y,
			0, 0, 1, a_z,
			0, 0, 0, 1
			);
	}
	//Matrix Matrix::InverseTranslation(float a_x, float a_y, float a_z) {
	//
	//}
	Matrix Matrix::Perspective(float a_fov, float a_w, float a_h, float a_near, float a_far) {
		return Matrix ((1/tan((a_fov/(a_w/a_h))/2))/(a_w/a_h),	 0,					0,		0,
		0,	1/tan((a_fov/(a_w/a_h))/2),								0,		0,
		0,							0, (a_far+a_near)/(a_near-a_far) , (2*a_far*a_near)/(a_near-a_far),
		0,							0,								-1,		0);
	}

	Matrix Matrix::Rotation(float x, float y, float z) {
		return Matrix(
			cos(z)*cos(y), cos(z)*sin(y)*sin(x)-sin(z)*cos(x), cos(z)*sin(y)*cos(x)+sin(z)*sin(x), 0,
			sin(z)*cos(y), sin(z)*sin(y)*sin(x)+cos(z)*cos(x), sin(z)*sin(y)*cos(x)-cos(z)*sin(x), 0,
				  -sin(y),					    cos(y)*sin(x),						cos(y)*cos(x), 0,
						0,									0,									0, 1
			);
	}

	void GL_SetModelMatrix(const Matrix& a_matrix) {
		memcpy(_ModelMatrix, a_matrix.m, sizeof(float) * 16);
	}

	void GL_SetViewMatrix(const Matrix& a_matrix) {
		memcpy(_ViewMatrix, a_matrix.m, sizeof(float) * 16);
	}

	void GL_SetProjectionMatrix(const Matrix& a_matrix) {
		memcpy(_ProjectionMatrix, a_matrix.m, sizeof(float) * 16);
	}

	void GL_SetNormalMatrix(const Matrix& a_matrix) {
		memcpy(_NormalMatrix, a_matrix.m, sizeof(float) * 9);
	}
}
