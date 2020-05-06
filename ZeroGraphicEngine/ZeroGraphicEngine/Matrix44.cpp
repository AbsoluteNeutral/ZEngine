
#include "stdafx.h"
#include "Matrix44.h"

namespace zg {
	//static
	const Matrix44 Matrix44::Identity {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
	};
	const Matrix44 Matrix44::Zero{};
	
	Matrix44::Matrix44() noexcept
		:m{ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }
	{}
	Matrix44::Matrix44(float _00, float _01, float _02, float _03,
					   float _10, float _11, float _12, float _13,
					   float _20, float _21, float _22, float _23,
					   float _30, float _31, float _32, float _33) noexcept
	#ifdef ROW_MAJOR
		:m{ _00, _01, _02, _03,
			_10, _11, _12, _13,
			_20, _21, _22, _23,
			_30, _31, _32, _33 }
	#else
		:m{ _00, _10, _20, _30,
			_01, _11, _21, _31,
			_02, _12, _22, _32,
			_03, _13, _23, _33 }
	#endif
	{}
	
	//setters
	void Matrix44::SetIdentity() {
		int M = 4;
		float *b = *md;			/*begin of matrix*/
		float *e = *md + M * M; /*end of matrix  */
		float *e3 = *md + M;    /*end of each row*/
		while (b < e) {
			float *b1 = b + 1;  /*get next col of matrix*/
			float *b2 = b + M;  /*get next row of matrix*/
			while (b1 != e3) {
				*b1 *= 0.0f;
				*b2 *= 0.0f;
				++b1;           /*get next row of matrix*/
				b2 = b2 + M;    /*get next col of matrix*/
			}
			*b = 1.0f;
			b = b + M + 1;    /*get next diagonal matrix*/
			e3 = e3 + M;      /*get end of next row*/
		}
	}

	void Matrix44::SetZero() {
		float* b = m;
		float* e = m + 16;
		while (b != e) {
			*b *= 0.0f; *(b + 1) *= 0.0f; *(b + 2) *= 0.0f; *(b + 3) *= 0.0f;
			b += 4;
		}
	}
	void Matrix44::Transpose() {
		int M = 4;
		float *b = *md;		 /*begin of matrix*/
		float *e = *md + M * M; /*end of matrix  */
		float *e3 = *md + M;    /*end of each row*/
								/*
								b, is incremented digonally each loop
								b1 - is the next right element of the current row
								- OR the next element of the next column
								b2 - is the next down element of the current col
								- OR the next element of the next row
								then swap
								*/
		while (b < e) {
			float *b1 = b + 1;  /*get next col of matrix*/
			float *b2 = b + M;  /*get next row of matrix*/
			while (b1 != e3) {
				float x = *b1;    /*swap*/
				float y = *b2;
				*b1 = y;
				*b2 = x;
				++b1;           /*get next row of matrix*/
				b2 = b2 + M;    /*get next col of matrix*/
			}
			b = b + M + 1;    /*get next diagonal matrix*/
			e3 = e3 + M;      /*get end of next row*/
		}
	}

	//getters
	float* Matrix44::GetPtr() { return m; }
	Vector4 Matrix44::GetCol(unsigned index_) const {
#ifdef ROW_MAJOR
		return Vector4{ m[index_],
						m[4 + index_],
						m[8 + index_],
						m[12 + index_] };
#else
		unsigned i = index_ * 4;
		return Vector4{ m[i],
						m[i + 1],
						m[i + 2],
						m[i + 3] };
#endif
	}
	Vector4 Matrix44::GetRow(unsigned index_) const {
#ifdef ROW_MAJOR
		unsigned i = index_ * 4;
		return Vector4{ m[i],
						m[i + 1],
						m[i + 2],
						m[i + 3] };
#else
		return Vector4{ m[index_],
						m[4 + index_] ,
						m[8 + index_] ,
						m[12 + index_] };
#endif
	}
	float Matrix44::Trace() const { return m00 + m11 + m22 + m33; }

	Matrix44 Matrix44::Transposed() const {
#ifdef ROW_MAJOR
		Matrix44 tmp{ *this };
		tmp.Transpose();
		return tmp;
#else
		return Matrix44{ *this };
#endif
	}

	//operators
	Matrix44::operator float*() { return m; }
	Matrix44& Matrix44::operator*=(float f_) {
		float* b = m;
		float* e = m + 16;
		while (b != e) {
			*b *= f_; *(b + 1) *= f_; *(b + 2) *= f_; *(b + 3) *= f_;
			b += 4;
		}
		//#version 1
		//m0 *= f;  m1 *= f; m2  *= f; m3  *= f;
		//m4 *= f;  m5 *= f; m6  *= f; m7  *= f;
		//m8 *= f;  m9 *= f; m10 *= f; m11 *= f;
		//m12*= f; m13 *= f; m14 *= f; m15 *= f;
		return *this;
	}
	Matrix44& Matrix44::operator+=(const Matrix44& matrix_) {
		const float* b1 = matrix_.m;
		float* b = m;
		float* e = m + 16;
		while (b != e) {
			*b += *b1; 
			*(b + 1) += *(b1 + 1);
			*(b + 2) += *(b1 + 2);
			*(b + 3) += *(b1 + 3);
			b += 4; b1 += 4;
		}
		//#version 1
		//m0  += rhs.m0;  m1  += rhs.m1; m2   += rhs.m2;  m3  += rhs.m3;
		//m4  += rhs.m4;  m5  += rhs.m5; m6   += rhs.m6;  m7  += rhs.m7;
		//m8  += rhs.m8;  m9  += rhs.m9; m10  += rhs.m10; m11 += rhs.m11;
		//m12 += rhs.m12; m13 += rhs.m13; m14 += rhs.m14; m15 += rhs.m15;
		return *this;
	}
	Matrix44& Matrix44::operator-= (const Matrix44 &matrix_) {
		const float* b1 = matrix_.m;
		float* b = m;
		float* e = m + 16;
		while (b != e) {
			*b -= *b1;
			*(b + 1) -= *(b1 + 1);
			*(b + 2) -= *(b1 + 2);
			*(b + 3) -= *(b1 + 3);
			b += 4; b1 += 4;
		}
		//#version 1
		//m[0]  -=  rhs.m[0];  m[1] -= rhs.m[1]; m[2]   -= rhs.m[2];  m[3]  -= rhs.m[3];
		//m[4]  -=  rhs.m[4];  m[5] -= rhs.m[5]; m[6]   -= rhs.m[6];  m[7]  -= rhs.m[7];
		//m[8]  -=  rhs.m[8];  m[9] -= rhs.m[9]; m[10]  -= rhs.m[10]; m[11] -= rhs.m[11];
		//m[12] -= rhs.m[12]; m[13] -= rhs.m[13]; m[14] -= rhs.m[14]; m[15] -= rhs.m[15];
		return *this;
	}
	
#ifdef USING_SOL2
	Matrix44 Matrix44::operator+(const Matrix44 &rhs) const {
		Matrix44 tmp{ *this };
		return tmp.operator+=(rhs);
	}
	Matrix44 Matrix44::operator*(const Matrix44 &rhs) const {
		Vector4 row0{ GetRow(0) };
		Vector4 row1{ GetRow(1) };
		Vector4 row2{ GetRow(2) };
		Vector4 row3{ GetRow(3) };
	
		Vector4 col0{ rhs.GetCol(0) };
		Vector4 col1{ rhs.GetCol(1) };
		Vector4 col2{ rhs.GetCol(2) };
		Vector4 col3{ rhs.GetCol(3) };
		return Matrix44{
			Dot(row0, col0), Dot(row0, col1), Dot(row0, col2), Dot(row0, col3),
			Dot(row1, col0), Dot(row1, col1), Dot(row1, col2), Dot(row1, col3),
			Dot(row2, col0), Dot(row2, col1), Dot(row2, col2), Dot(row2, col3),
			Dot(row3, col0), Dot(row3, col1), Dot(row3, col2), Dot(row3, col3)
		};
	}
	Vector2 Matrix44::operator*(const Vector2 &rhs) const {
		Vector4 tmp{ rhs.x, rhs.y, 0.0f, 1.0f };
		return Vector2{
			Dot(GetRow(0), tmp),
			Dot(GetRow(1), tmp)
		};
	}
	Vector3 Matrix44::operator*(const Vector3 &rhs) const {
		Vector4 tmp{ rhs.x, rhs.y, rhs.z, 1.0f };
		return Vector3{
			Dot(GetRow(0), tmp),
			Dot(GetRow(1), tmp),
			Dot(GetRow(2), tmp)
		};
	}
	Vector4 Matrix44::operator*(const Vector4 &rhs) const {
		return Vector4{
			Dot(GetRow(0), rhs),
			Dot(GetRow(1), rhs),
			Dot(GetRow(2), rhs),
			Dot(GetRow(3), rhs)
		};
	}
	void Matrix44::BindLua(sol::state & lua)
	{
		lua.new_usertype<Matrix44>("Matrix44",
			sol::constructors<
			Matrix44(),
			Matrix44(float, float, float, float,
				float, float, float, float,
				float, float, float, float,
				float, float, float, float)
			>(),
	
			"Transposed", &Matrix44::Transposed,
			"Transpose", &Matrix44::Transpose,
			"Identity", &Matrix44::Identity,
			"GetCol", &Matrix44::GetCol,
			"GetRow", &Matrix44::GetRow,
	
			sol::meta_function::addition, &Matrix44::operator+,
	
			sol::meta_function::multiplication, sol::overload(
	
				static_cast<Matrix44(Matrix44::*)(const Matrix44&) const>(&Matrix44::operator*),
				static_cast<Vector2(Matrix44::*)(const Vector2&) const>(&Matrix44::operator*),
				static_cast<Vector3(Matrix44::*)(const Vector3&) const>(&Matrix44::operator*),
				static_cast<Vector4(Matrix44::*)(const Vector4&) const>(&Matrix44::operator*)
			)
			);
	}
#else
	Matrix44 operator-(const Matrix44& lhs, const Matrix44 &rhs) {
		Matrix44 tmp{ lhs };
		return tmp.operator-=(rhs);
	}
	Matrix44 operator+(const Matrix44& lhs, const Matrix44 &rhs) {
		Matrix44 tmp{ lhs };
		return tmp.operator+=(rhs);
	}

	//Matrix44 operator*(const Matrix44 &lhs, const Matrix44 &rhs) {
	//	//return Matrix44{
	//	//	lhs.m[0]  * rhs.m[0] + lhs.m[1]  * rhs.m[4] + lhs.m[2]  * rhs.m[8]  + lhs.m[3]  * rhs.m[12]	,
	//	//	lhs.m[0]  * rhs.m[1] + lhs.m[1]  * rhs.m[5] + lhs.m[2]  * rhs.m[9]  + lhs.m[3]  * rhs.m[13]	,
	//	//	lhs.m[0]  * rhs.m[2] + lhs.m[1]  * rhs.m[6] + lhs.m[2]  * rhs.m[10] + lhs.m[3]  * rhs.m[14]	,
	//	//	lhs.m[0]  * rhs.m[3] + lhs.m[1]  * rhs.m[7] + lhs.m[2]  * rhs.m[11] + lhs.m[3]  * rhs.m[15]	,
	//	//
	//	//	lhs.m[4]  * rhs.m[0] + lhs.m[5]  * rhs.m[4] + lhs.m[6]  * rhs.m[8]  + lhs.m[7]  * rhs.m[12]	,
	//	//	lhs.m[4]  * rhs.m[1] + lhs.m[5]  * rhs.m[5] + lhs.m[6]  * rhs.m[9]  + lhs.m[7]  * rhs.m[13]	,
	//	//	lhs.m[4]  * rhs.m[2] + lhs.m[5]  * rhs.m[6] + lhs.m[6]  * rhs.m[10] + lhs.m[7]  * rhs.m[14]	,
	//	//	lhs.m[4]  * rhs.m[3] + lhs.m[5]  * rhs.m[7] + lhs.m[6]  * rhs.m[11] + lhs.m[7]  * rhs.m[15]	,
	//	//
	//	//	lhs.m[8]  * rhs.m[0] + lhs.m[9]  * rhs.m[4] + lhs.m[10] * rhs.m[8]  + lhs.m[11] * rhs.m[12]	,
	//	//	lhs.m[8]  * rhs.m[1] + lhs.m[9]  * rhs.m[5] + lhs.m[10] * rhs.m[9]  + lhs.m[11] * rhs.m[13]	,
	//	//	lhs.m[8]  * rhs.m[2] + lhs.m[9]  * rhs.m[6] + lhs.m[10] * rhs.m[10] + lhs.m[11] * rhs.m[14]	,
	//	//	lhs.m[8]  * rhs.m[3] + lhs.m[9]  * rhs.m[7] + lhs.m[10] * rhs.m[11] + lhs.m[11] * rhs.m[15]	,
	//	//
	//	//	lhs.m[12] * rhs.m[0] + lhs.m[13] * rhs.m[4] + lhs.m[14] * rhs.m[8]  + lhs.m[15] * rhs.m[12]	,
	//	//	lhs.m[12] * rhs.m[1] + lhs.m[13] * rhs.m[5] + lhs.m[14] * rhs.m[9]  + lhs.m[15] * rhs.m[13]	,
	//	//	lhs.m[12] * rhs.m[2] + lhs.m[13] * rhs.m[6] + lhs.m[14] * rhs.m[10] + lhs.m[15] * rhs.m[14]	,
	//	//	lhs.m[12] * rhs.m[3] + lhs.m[13] * rhs.m[7] + lhs.m[14] * rhs.m[11] + lhs.m[15] * rhs.m[15]
	//	//};
	//	return Matrix44{
	//		lhs.GetRow(0) * rhs.GetCol(0), lhs.GetRow(0) * rhs.GetCol(1), lhs.GetRow(0) * rhs.GetCol(2), lhs.GetRow(0) * rhs.GetCol(3),
	//		lhs.GetRow(1) * rhs.GetCol(0), lhs.GetRow(1) * rhs.GetCol(1), lhs.GetRow(1) * rhs.GetCol(2), lhs.GetRow(1) * rhs.GetCol(3),
	//		lhs.GetRow(2) * rhs.GetCol(0), lhs.GetRow(2) * rhs.GetCol(1), lhs.GetRow(2) * rhs.GetCol(2), lhs.GetRow(2) * rhs.GetCol(3),
	//		lhs.GetRow(3) * rhs.GetCol(0), lhs.GetRow(3) * rhs.GetCol(1), lhs.GetRow(3) * rhs.GetCol(2), lhs.GetRow(3) * rhs.GetCol(3),
	//	};
	//}
	Matrix44 operator*(const Matrix44 &lhs, const Matrix44 &rhs) {
		Vector4 row0{ lhs.GetRow(0) };
		Vector4 row1{ lhs.GetRow(1) };
		Vector4 row2{ lhs.GetRow(2) };
		Vector4 row3{ lhs.GetRow(3) };
	
		Vector4 col0{ rhs.GetCol(0) };
		Vector4 col1{ rhs.GetCol(1) };
		Vector4 col2{ rhs.GetCol(2) };
		Vector4 col3{ rhs.GetCol(3) };
	
		return Matrix44{
			Dot(row0, col0), Dot(row0, col1), Dot(row0, col2), Dot(row0, col3),
			Dot(row1, col0), Dot(row1, col1), Dot(row1, col2), Dot(row1, col3),
			Dot(row2, col0), Dot(row2, col1), Dot(row2, col2), Dot(row2, col3),
			Dot(row3, col0), Dot(row3, col1), Dot(row3, col2), Dot(row3, col3)
		};
	}
	Vector2 operator*(const Matrix44& pMtx, const Vector2 &pt_) {
		Vector4 tmp{ pt_.x, pt_.y, 0.0f, 1.0f };
		return Vector2{
			Dot(pMtx.GetRow(0), tmp),
			Dot(pMtx.GetRow(1), tmp)
		};	
	}		
	Vector3 operator*(const Matrix44& pMtx, const Vector3 &pt_) {
		Vector4 tmp{ pt_.x, pt_.y, pt_.z, 1.0f };
		return Vector3{
			Dot(pMtx.GetRow(0), tmp),
			Dot(pMtx.GetRow(1), tmp),
			Dot(pMtx.GetRow(2), tmp)
		};
	}
	Vector4 operator*(const Matrix44& pMtx, const Vector4 &rhs) {
		return Vector4{
			Dot(pMtx.GetRow(0), rhs),
			Dot(pMtx.GetRow(1), rhs),
			Dot(pMtx.GetRow(2), rhs),
			Dot(pMtx.GetRow(3), rhs)
		};
	}
#endif
	std::ostream& operator<<(std::ostream& os, const Matrix44& matrix_) {
		printf("{ %.6f %.6f %.6f %.6f\n  %.6f %.6f %.6f %.6f\n  %.6f %.6f %.6f %.6f\n  %.6f %.6f %.6f %.6f }\n",
			matrix_.m00, matrix_.m01, matrix_.m02, matrix_.m03,
			matrix_.m10, matrix_.m11, matrix_.m12, matrix_.m13,
			matrix_.m20, matrix_.m21, matrix_.m22, matrix_.m23,
			matrix_.m30, matrix_.m31, matrix_.m32, matrix_.m33
		);
		// os << "{ " << lhs.md[0][0] << " " << lhs.md[0][1] << " " << lhs.md[0][2] << " " << lhs.md[0][3] << "\n"
		//	<< "  " << lhs.md[1][0] << " " << lhs.md[1][1] << " " << lhs.md[1][2] << " " << lhs.md[1][3] << "\n"
		//	<< "  " << lhs.md[2][0] << " " << lhs.md[2][1] << " " << lhs.md[2][2] << " " << lhs.md[2][3] << "\n"
		//	<< "  " << lhs.md[3][0] << " " << lhs.md[3][1] << " " << lhs.md[3][2] << " " << lhs.md[3][3] << "\n\n";
		return os;
	}

	//extern function
	Matrix44 BuildTranslate4x4(float x_, float y_, float z_) {
		return Matrix44{ 1.0f, 0.0f, 0.0f, x_,
						 0.0f, 1.0f, 0.0f, y_,
						 0.0f, 0.0f, 1.0f, z_,
						 0.0f, 0.0f, 0.0f, 1.0f };
	}
	Matrix44 BuildTranslate4x4(const Vector3& v_) { return BuildTranslate4x4(v_.x, v_.y, v_.z); }

	Matrix44 BuildScale4x4(float x_) {
			return Matrix44{ x_,    0.0f,  0.0f, 0.0f,
							 0.0f, x_,     0.0f, 0.0f,
							 0.0f, 0.0f,  x_,    0.0f,
							 0.0f, 0.0f,  0.0f, 1.0f };
	}
	Matrix44 BuildScale4x4(float x_, float y_, float z_) {
		return Matrix44{ x_,    0.0f,  0.0f, 0.0f,
						0.0f, y_,     0.0f, 0.0f,
						0.0f, 0.0f,  z_,    0.0f,
						0.0f, 0.0f,  0.0f, 1.0f };
	}
	Matrix44 BuildScale4x4(const Vector3& v_) { return BuildScale4x4(v_.x, v_.y, v_.z); }

	Matrix44 BuildRotation4x4(float degree_, const Vector3& axis_) {
		/*	Rotate on an axis
		r(cos0) + (1-cos0)(r.n)n + (sin0)(n X r)

		(cos0)		(1-cos0)			(sin0)
		|1 0 0 0| |RxRx RxRy RxRz 0| |0  -Rz Ry  0|
		|0 1 0 0|+|RxRy RyRy RyRz 0|+|Rz  0  -Rx 0|
		|0 0 1 0| |RxRz RyRz RzRz 0| |-Ry Rx 0   0|
		|0 0 0 1| |0    0    0    1| |0   0  0   1|

		|cos0 + RxRx(1-cos0)	 RxRy(1-cos0) - Rz(sin0) RxRz(1-cos0) + Ry(sin0) 0|
		|RxRy(1-cos0) + Rz(sin0) cos0 + RyRy(1-cos0)	 RyRz(1-cos0) - Rx(sin0) 0|
		|RxRz(1-c0s0) - Ry(sin0) RyRz(1-cos0) + Rx(sin0) cos0 + RzRz(1-cos0)     0|
		|0						 0						 0						 1|

		Transpose:
		|cos0 + RxRx(1-cos0)	 RxRy(1-cos0) + Rz(sin0) RxRz(1-c0s0) - Ry(sin0) 0|
		|RxRy(1-cos0) - Rz(sin0)  cos0 + RyRy(1-cos0)	 RyRz(1-cos0) + Rx(sin0) 0|
		|RxRz(1-cos0) + Ry(sin0) RyRz(1-cos0) - Rx(sin0) cos0 + RzRz(1-cos0)     0|
		|0						 0						 0						 1|
		*/
		float c = std::cosf(degree_);				//calculate once
		float s = std::sinf(degree_);				//calculate once

		Vector3 axis{ Normalized(axis_) };		//normalize the axis
		Vector3 paral{ (1 - c) * axis };		//pre-calculate (1-cos) * axis

		return Matrix44{
			c + axis.x * paral.x,			axis.x * paral.y - axis.z * s,  axis.x * paral.z + axis.y * s, 0.0f,
			axis.x * paral.y + axis.z * s,	c + axis.y * paral.y,			axis.y * paral.z - axis.x * s, 0.0f,
			axis.x * paral.z - axis.y * s,	axis.y * paral.z + axis.x * s,	c + axis.z * paral.z,		   0.0f,
			0.0f,							0.0f,							0.0f,						   1.0f
		};
	}
	Matrix44 BuildRotation4x4(const Quaternion& q)
	{
		float nQ = q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w;
		float s = nQ > 0.0f ? 2.0f / nQ : 0.0f;
		float xs = q.x * s;
		float ys = q.y * s;
		float zs = q.z * s;
		float wxs = q.w * xs;
		float wys = q.w * ys;
		float wzs = q.w * zs;
		float xxs = q.x * xs;
		float xys = q.x * ys;
		float xzs = q.x * zs;
		float yys = q.y * ys;
		float yzs = q.y * zs;
		float zzs = q.z * zs;
		return Matrix44{ 
			1.0f - yys - zzs,	xys - wzs,			xzs + wys,			0.0f,
			xys + wzs,			1.0f - xxs - zzs,	yzs - wxs,			0.0f,
			xzs - wys,			yzs + wxs,			1.0f - xxs - yys,	0.0f,
			0.0f,				0.0f,				0.0f,				1.0f 
		};
		//float xx = q.x * q.x;
		//float yy = q.y * q.y;
		//float zz = q.z * q.z;
		//float xy = q.x * q.y;
		//float xz = q.x * q.z;
		//float yz = q.y * q.z;
		//float wx = q.w * q.x;
		//float wy = q.w * q.y;
		//float wz = q.w * q.z;
		//return Matrix44{
		//	1.0f - 2.0f * (yy + zz),	2.0f * (xy - wz),			2.0f * (xz + wy),		0.0f,
		//	2.0f * (xy + wz),			1.0f - 2.0f * (xx + zz),	2.0f * (yz - wx),		0.0f,
		//	2.0f * (xz - wy),			2.0f * (yz + wx),			1.0f - 2.0f * (xx + yy),0.0f,
		//	0.0f,						0.0f,						0.0f,					1.0f
		//};
	}
	Matrix44 BuildRotation4x4(const Quaternion& q, const Vector3& translation_) {
		float nQ = q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w;
		float s = nQ > 0.0f ? 2.0f / nQ : 0.0f;
		float xs = q.x * s;
		float ys = q.y * s;
		float zs = q.z * s;
		float wxs = q.w * xs;
		float wys = q.w * ys;
		float wzs = q.w * zs;
		float xxs = q.x * xs;
		float xys = q.x * ys;
		float xzs = q.x * zs;
		float yys = q.y * ys;
		float yzs = q.y * zs;
		float zzs = q.z * zs;
		return Matrix44{ 1.0f - yys - zzs,	xys - wzs,			xzs + wys,	translation_.x,
			xys + wzs,			1.0f - xxs - zzs,	yzs - wxs,				translation_.y,
			xzs - wys,			yzs + wxs,			1.0f - xxs - yys,		translation_.z,
			0.0f,				0.0f,				0.0f,					1.0f };
	}

#pragma region Build TRS
	void BuildTRS(Matrix44& m, const Vector3& trans, const Vector3& scale, float angleZ_) {
		float c = std::cos(angleZ_);
		float s = std::sin(angleZ_);
#ifdef ROW_MAJOR
		m.m[0] = scale.x *c;
		m.m[1] = scale.x *s;
		m.m[4] = scale.y *-s;
		m.m[5] = scale.y *c;
		m.m[10] = scale.z;
		m.m[12] = trans.x;
		m.m[13] = trans.y;
		m.m[14] = trans.z;
#else
		m.m[0] = scale.x *c;
		m.m[1] = scale.x *s;
		m.m[4] = scale.y *-s;
		m.m[5] = scale.y *c;
		m.m[10] = scale.z;
		m.m[12] = trans.x;
		m.m[13] = trans.y;
		m.m[14] = trans.z;
#endif
	}
	void BuildTRS(Matrix44& m, const Vector3& translation_, const Vector3& scale_, const Vector3& angle_) {
		m = BuildTranslate4x4(translation_)
			* BuildRotation4x4(angle_.x, { 1.0f, 0.0f, 0.0f })
			* BuildRotation4x4(angle_.y, { 0.0f, 1.0f, 0.0f })
			* BuildRotation4x4(angle_.z, { 0.0f, 0.0f, 1.0f }) * BuildScale4x4(scale_);
	}
	void BuildTRS(Matrix44& m, const Vector3& translation_, const Vector3& scale_, const Vector3& axis_, float angle_) {
		m = BuildTranslate4x4(translation_) * BuildRotation4x4(angle_, axis_) * BuildScale4x4(scale_);
	}

	Matrix44 BuildTRS4x4(const Vector3& translation_, const Quaternion& quaternion_, const Vector3& scale_) {
		//return  BuildTranslate4x4(translation_) * BuildRotation4x4(quaternion_) * BuildScale4x4(scale_);
		float nQ = quaternion_.x * quaternion_.x + quaternion_.y * quaternion_.y + quaternion_.z * quaternion_.z + quaternion_.w * quaternion_.w;
		float s = nQ > 0.0f ? 2.0f / nQ : 0.0f;
		float xs = quaternion_.x * s;
		float ys = quaternion_.y * s;
		float zs = quaternion_.z * s;
		float wxs = quaternion_.w * xs;
		float wys = quaternion_.w * ys;
		float wzs = quaternion_.w * zs;
		float xxs = quaternion_.x * xs;
		float xys = quaternion_.x * ys;
		float xzs = quaternion_.x * zs;
		float yys = quaternion_.y * ys;
		float yzs = quaternion_.y * zs;
		float zzs = quaternion_.z * zs;
		return Matrix44{ 
			(1.0f - yys - zzs) * scale_.x,	(xys - wzs		 ) * scale_.y,	(xzs + wys		 ) * scale_.z,	translation_.x,
			(xys + wzs		 ) * scale_.x,	(1.0f - xxs - zzs) * scale_.y,	(yzs - wxs		 ) * scale_.z,	translation_.y,
			(xzs - wys		 ) * scale_.x,	(yzs + wxs		 )*  scale_.y,	(1.0f - xxs - yys) * scale_.z,	translation_.z,
			0.0f,							0.0f,							0.0f,							1.0f 
		};
	}
#pragma endregion

	Matrix44 BuildSkew4x4(const Vector3& v_)
	{
		return Matrix44{
			 0.0f,	-v_.z,	v_.y, 0.0f,
			 v_.z,	 0.0f, -v_.x, 0.0f,
			-v_.y,   v_.x,	0.0f, 0.0f,
			 0.0f,	 0.0f,	0.0f, 1.0f
		};
	}

	//ETC
	Matrix44 LookAtRH(const Vector3& eye, const Vector3& target, const Vector3& up) {
		//eye = position
		//center = target
		//up = camera up vector
		const Vector3 zaxis{ Normalized(target - eye) };     // The "forward" vector/direction.
		const Vector3 xaxis{ Normalized(Cross(zaxis, up)) }; // The "right" vector.
		const Vector3 yaxis{ Cross(xaxis, zaxis) };

		return Matrix44{ xaxis.x,  xaxis.y,   xaxis.z,	-Dot(xaxis, eye),
						 yaxis.x,  yaxis.y,   yaxis.z,	-Dot(yaxis, eye),
						 -zaxis.x, -zaxis.y,  -zaxis.z,	 Dot(zaxis, eye),
						 0.0f,	    0.0f,      0.0f,	 1.0f };

		//return Matrix44{ xaxis.x, yaxis.x,  zaxis.x, 0.0f,
		//				   xaxis.y, yaxis.y,  zaxis.y, 0.0f,
		//				   xaxis.z, yaxis.z,  zaxis.z, 0.0f,
		//				   -Dot(xaxis, eye),  -Dot(yaxis, eye), -Dot(zaxis, eye), 1.0f };
	}

	Matrix44 LookAtRHDir(const Vector3& eye, const Vector3& direction, const Vector3& up) {
		//eye = position
		//center = target
		//up = camera up vector
		const Vector3 zaxis{ Normalized(direction) };     // The "forward" vector/direction.
		const Vector3 xaxis{ Normalized(Cross(zaxis, up)) }; // The "right" vector.
		const Vector3 yaxis{ Cross(xaxis, zaxis) };

		return Matrix44{ xaxis.x,  xaxis.y,   xaxis.z, -Dot(xaxis, eye),
			yaxis.x,  yaxis.y,   yaxis.z, -Dot(yaxis, eye),
			-zaxis.x, -zaxis.y,  -zaxis.z,  Dot(zaxis, eye),
			0.0f,	    0.0f,      0.0f,     1.0f };

		//return Matrix44{ xaxis.x, yaxis.x,  zaxis.x, 0.0f,
		//				   xaxis.y, yaxis.y,  zaxis.y, 0.0f,
		//				   xaxis.z, yaxis.z,  zaxis.z, 0.0f,
		//				   -Dot(xaxis, eye),  -Dot(yaxis, eye), -Dot(zaxis, eye), 1.0f };
	}

	Matrix44 LookAtLH(const Vector3& eye, const Vector3& target, const Vector3& up) {

		//eye = position
		//center = target
		//up = camera up vector
		const Vector3 zaxis{ Normalized(target - eye) };     // The "forward" vector/direction.
		const Vector3 xaxis{ Normalized(Cross(up, zaxis)) }; // The "right" vector.
		const Vector3 yaxis{ Cross(zaxis, xaxis) };

		return Matrix44{ xaxis.x,  xaxis.y,   xaxis.z,   -Dot(xaxis, eye),
			yaxis.x,  yaxis.y,   yaxis.z,   -Dot(yaxis, eye),
			-zaxis.x, -zaxis.y,  -zaxis.z,  -Dot(zaxis, eye),
			0.0f,	    0.0f,      0.0f,     1.0f };

		//return Matrix44{ xaxis.x, yaxis.x,  zaxis.x, 0.0f,
		//				   xaxis.y, yaxis.y,  zaxis.y, 0.0f,
		//				   xaxis.z, yaxis.z,  zaxis.z, 0.0f,
		//				   -Dot(xaxis, eye),  -Dot(yaxis, eye), -Dot(zaxis, eye), 1.0f };
	}
	Matrix44 OrthRH(float left, float right, float bottom, float top, float near_, float far_) {

		return Matrix44{
			2.0f / (right - left),	0.0f,					0.0f,					-(right + left) / (right - left),
			0.0f,					2.0f / (top - bottom),	0.0f,					-(top + bottom) / (top - bottom),
			0.0f,					0.0f,					-2.0f / (far_ - near_),	-(far_ + near_) / (far_ - near_),
			0.0f,					0.0f,					0.0f,					1.0f
		};

		//return Matrix44{
		//	2.0f / (right - left),				0.0f,								0.0f,								0.0f,
		//	0.0f,								2.0f / (top - bottom),				0.0f,								0.0f,
		//	0.0f,								0.0f,								-2.0f / (far_ - near_),				0.0f,
		//	-(right + left) / (right - left),	-(top + bottom) / (top - bottom),	-(far_ + near_) / (far_ - near_),	1.0f
		//};
	}
	Matrix44 Orth(float left, float right, float bottom, float top) {
		return Matrix44{
			2.0f / (right - left),	0.0f,					0.0f,	-(right + left) / (right - left),
			0.0f,					2.0f / (top - bottom),	0.0f,	-(top + bottom) / (top - bottom),
			0.0f,					0.0f,					1.0f,	0.0f,
			0.0f,					0.0f,					0.0f,	1.0f
		};
	}

	Matrix44 PerspectiveRH(float fovRAD_, float aspect_, float near_, float far_) {
		const float tanHalfFovy = std::tan(fovRAD_ * 0.5f);
		const float fn = far_ - near_;
		return Matrix44{
			1.0f / (aspect_ * tanHalfFovy),	0.0f,					0.0f,					0.0f,
			0.0f,							1.0f / (tanHalfFovy),	0.0f,					0.0f,
			0.0f,							0.0f,					-(far_ + near_) / fn,   -(2.0f * far_ * near_) / fn,
			0.0f,							0.0f,					-1.0f,					1.0f
		};
	}

	Matrix44	FrustumRH(float left_, float right_, float bottom_, float top_, float near_, float far_) {
		return Matrix44{
			(2.0f * near_) / (right_ - left_),	0.0f,								(right_ + left_) / (right_ - left_),	0.0f,
			0.0f,								(2.0f * near_) / (top_ - bottom_),	(top_ + bottom_) / (top_ - bottom_),	0.0f,
			0.0f,								0.0f,								-(far_ + near_) / (far_ - near_),		-(2.0f * far_ * near_) / (far_ - near_),
			0.0f,								0.0f,								-1.0f,									1.0f
		};
	}

	Matrix44 ViewPortTransform(int width_, int height_) {
		float w = width_ * 0.5f;
		float h = height_ * 0.5f;
		return Matrix44{
			w,		0.0f,	0.0f,	w - 0.5f,
			0.0f,	-h,		0.0f,	h - 0.5f,
			0.0f,	0.0f,	0.5f,	0.5f,
			0.0f,	0.0f,	0.0f,	1.0f
		};
	}

	float Determinant(const Matrix44& matrix_, int dimension_) {
		if (dimension_ == 2) {
			if (matrix_.m[0] && matrix_.m[1])
				return matrix_.md[0][0] * matrix_.md[1][1] - matrix_.md[0][1] * matrix_.md[1][0];
			return 0;
		}
		else
		{
			Matrix44 copy{};
			float result = 0.0f;

			for (int k = 0; k < dimension_; ++k)
			{
				for (int i = 1; i < dimension_; ++i)
				{
					for (int j = 0; j < dimension_; ++j)
					{
						if (j < k)
							copy.md[i - 1][j] = matrix_.md[i][j];
						else if (j > k)
							copy.md[i - 1][j - 1] = matrix_.md[i][j];
					}
				}
				if (k & 1)
					result -= matrix_.m[k] * Determinant(copy, dimension_ - 1);
				else
					result += matrix_.m[k] * Determinant(copy, dimension_ - 1);
			}
			return result;
		}
	}

	Matrix44 GaussJordanInverse(const Matrix44& matrix_) {
		Vector4 idn[4]{ { 1.0f, 0.0f, 0.0f, 0.0f },
		{ 0.0f, 1.0f, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f, 1.0f } };
		Vector4 row[4]{ matrix_.GetRow(0),
			matrix_.GetRow(1),
			matrix_.GetRow(2),
			matrix_.GetRow(3) };

		for (int i = 0; i < 4; ++i) {
			float divider = 1.0f / row[i][i];
			row[i] *= divider;
			idn[i] *= divider;
			for (int j = 0; j < 4; ++j) {
				if (i == j) continue;
				if (row[j][i]) {
					Vector4 tmp = row[i];
					Vector4 tmd = idn[i];
					tmp *= row[j][i];
					tmd *= row[j][i];
					row[j] -= tmp;
					idn[j] -= tmd;
				}
			}
		}
		return Matrix44{
			idn[0][0], idn[0][1], idn[0][2], idn[0][3],
			idn[1][0], idn[1][1], idn[1][2], idn[1][3],
			idn[2][0], idn[2][1], idn[2][2], idn[2][3],
			idn[3][0], idn[3][1], idn[3][2], idn[3][3]
		};
	}

	Vector3 GetPosition(const Matrix44& matrix_)
	{
#ifdef ROW_MAJOR
		return Vector3{ matrix_.m03, matrix_.m13, matrix_.m23 };
#else
		return Vector3{ matrix_.m30, matrix_.m31, matrix_.m32 };
#endif

	}
	Vector3 GetScale(const Matrix44& matrix_)
	{
#ifdef ROW_MAJOR
		return Vector3{
			Vector3{ matrix_.m00, matrix_.m10, matrix_.m20 }.Length(),
			Vector3{ matrix_.m01, matrix_.m11, matrix_.m21 }.Length(),
			Vector3{ matrix_.m02, matrix_.m12, matrix_.m22 }.Length()
		};
#else
		return Vector3{
			Vector3{ matrix_.m00, matrix_.m01, matrix_.m02 }.Length(),
			Vector3{ matrix_.m10, matrix_.m11, matrix_.m12 }.Length(),
			Vector3{ matrix_.m20, matrix_.m21, matrix_.m22 }.Length()
		};
#endif
	}

	Quaternion GetRotation(const Matrix44& matrix_) {
		Matrix44 matrix = matrix_;
		Vector3 scale = 1.0f / GetScale(matrix_);

		matrix.md[0][0] *= scale.x;
		matrix.md[0][1] *= scale.x;
		matrix.md[0][2] *= scale.x;
		matrix.md[1][0] *= scale.y;
		matrix.md[1][1] *= scale.y;
		matrix.md[1][2] *= scale.y;
		matrix.md[2][0] *= scale.z;
		matrix.md[2][1] *= scale.z;
		matrix.md[2][2] *= scale.z;
		matrix.Transpose();

		return ToQuaternion(matrix);
	}
	Vector3	TransformPoint(const Matrix44& matrix_, const Vector3& point_) 
	{
		return Vector3{ matrix_ * Vector4{ point_, 1.0f } };
	}
	Vector3	TransformVector(const Matrix44& matrix_, const Vector3& vector_) 
	{
		return Vector3{ matrix_ * Vector4{ vector_, 0.0f } };
	}

#pragma region Hardcoded Inverse
	Matrix44 Inverse(const Matrix44& matrix_)
	{
#ifdef ROW_MAJOR
		Vector4 idn0{ 1.0f, 0.0f, 0.0f, 0.0f };
		Vector4 idn1{ 0.0f, 1.0f, 0.0f, 0.0f };
		Vector4 idn2{ 0.0f, 0.0f, 1.0f, 0.0f };
		Vector4 idn3{ 0.0f, 0.0f, 0.0f, 1.0f };

		Vector4 row0{ matrix_.GetRow(0) };
		Vector4 row1{ matrix_.GetRow(1) };
		Vector4 row2{ matrix_.GetRow(2) };
		Vector4 row3{ matrix_.GetRow(3) };

		Vector4 tmp{ row0 };
		Vector4 tmd{ idn0 };

		row0 /= tmp.x;
		idn0 /= tmp.x;

		if (row1.x) {
			tmp = row0;
			tmd = idn0;
			tmp *= row1.x;
			tmd *= row1.x;
			row1 -= tmp;
			idn1 -= tmd;
		}

		if (row2.x) {
			tmp = row0;
			tmd = idn0;

			tmp *= row2.x;
			tmd *= row2.x;
			row2 -= tmp;
			idn2 -= tmd;
		}

		if (row3.x) {
			tmp = row0;
			tmd = idn0;

			tmp *= row3.x;
			tmd *= row3.x;
			row3 -= tmp;
			idn3 -= tmd;
		}

		tmp = row1;
		tmd = idn1;
		row1 /= tmp.y;
		idn1 /= tmp.y;

		if (row0.y) {
			tmp = row1;
			tmd = idn1;

			tmp *= row0.y;
			tmd *= row0.y;
			row0 -= tmp;
			idn0 -= tmd;
		}

		if (row2.y) {
			tmp = row1;
			tmd = idn1;

			tmp *= row2.y;
			tmd *= row2.y;
			row2 -= tmp;
			idn2 -= tmd;
		}

		if (row3.y) {
			tmp = row1;
			tmd = idn1;

			tmp *= row3.y;
			tmd *= row3.y;
			row3 -= tmp;
			idn3 -= tmd;
		}

		tmp = row2;
		if (row2.z != 1.0f) {
			row2 /= tmp.z;
			idn2 /= tmp.z;
		}


		if (row0.z) {
			tmp = row2;
			tmd = idn2;

			tmp *= row0.z;
			tmd *= row0.z;
			row0 -= tmp;
			idn0 -= tmd;
		}

		if (row1.z) {
			tmp = row2;
			tmd = idn2;

			tmp *= row1.z;
			tmd *= row1.z;
			row1 -= tmp;
			idn1 -= tmd;
		}

		if (row3.z) {
			tmp = row2;
			tmd = idn2;

			tmp *= row3.z;
			tmd *= row3.z;
			row3 -= tmp;
			idn3 -= tmd;
		}
		tmp = row3;
		row3 /= tmp.w;
		idn3 /= tmp.w;

		if (row0.w) {
			tmp = row3;
			tmd = idn3;

			tmp *= row0.w;
			tmd *= row0.w;
			row0 -= tmp;
			idn0 -= tmd;
		}

		if (row1.w) {
			tmp = row3;
			tmd = idn3;

			tmp *= row1.w;
			tmd *= row1.w;
			row1 -= tmp;
			idn1 -= tmd;
		}

		if (row2.w) {
			tmp = row3;
			tmd = idn3;

			tmp *= row2.w;
			tmd *= row2.w;
			row2 -= tmp;
			idn2 -= tmd;
		}

		Matrix44 result{
			idn0.x, idn0.y, idn0.z, idn0.w,
			idn1.x, idn1.y, idn1.z, idn1.w,
			idn2.x, idn2.y, idn2.z, idn2.w,
			idn3.x, idn3.y, idn3.z, idn3.w
		};
#else
		Vector4 idn0{ 1.0f, 0.0f, 0.0f, 0.0f };
		Vector4 idn1{ 0.0f, 1.0f, 0.0f, 0.0f };
		Vector4 idn2{ 0.0f, 0.0f, 1.0f, 0.0f };
		Vector4 idn3{ 0.0f, 0.0f, 0.0f, 1.0f };

		Vector4 row0{ matrix_.GetRow(0) };
		Vector4 row1{ matrix_.GetRow(1) };
		Vector4 row2{ matrix_.GetRow(2) };
		Vector4 row3{ matrix_.GetRow(3) };

		Vector4 tmp{ row0 };
		Vector4 tmd{ idn0 };

		row0 /= tmp.x;
		idn0 /= tmp.x;

		if (row1.x) {
			tmp = row0;
			tmd = idn0;
			tmp *= row1.x;
			tmd *= row1.x;
			row1 -= tmp;
			idn1 -= tmd;
		}

		if (row2.x) {
			tmp = row0;
			tmd = idn0;

			tmp *= row2.x;
			tmd *= row2.x;
			row2 -= tmp;
			idn2 -= tmd;
		}

		if (row3.x) {
			tmp = row0;
			tmd = idn0;

			tmp *= row3.x;
			tmd *= row3.x;
			row3 -= tmp;
			idn3 -= tmd;
		}

		tmp = row1;
		tmd = idn1;
		row1 /= tmp.y;
		idn1 /= tmp.y;

		if (row0.y) {
			tmp = row1;
			tmd = idn1;

			tmp *= row0.y;
			tmd *= row0.y;
			row0 -= tmp;
			idn0 -= tmd;
		}

		if (row2.y) {
			tmp = row1;
			tmd = idn1;

			tmp *= row2.y;
			tmd *= row2.y;
			row2 -= tmp;
			idn2 -= tmd;
		}

		if (row3.y) {
			tmp = row1;
			tmd = idn1;

			tmp *= row3.y;
			tmd *= row3.y;
			row3 -= tmp;
			idn3 -= tmd;
		}

		tmp = row2;
		if (row2.z != 1.0f) {
			row2 /= tmp.z;
			idn2 /= tmp.z;
		}


		if (row0.z) {
			tmp = row2;
			tmd = idn2;

			tmp *= row0.z;
			tmd *= row0.z;
			row0 -= tmp;
			idn0 -= tmd;
		}

		if (row1.z) {
			tmp = row2;
			tmd = idn2;

			tmp *= row1.z;
			tmd *= row1.z;
			row1 -= tmp;
			idn1 -= tmd;
		}

		if (row3.z) {
			tmp = row2;
			tmd = idn2;

			tmp *= row3.z;
			tmd *= row3.z;
			row3 -= tmp;
			idn3 -= tmd;
		}
		tmp = row3;
		row3 /= tmp.w;
		idn3 /= tmp.w;

		if (row0.w) {
			tmp = row3;
			tmd = idn3;

			tmp *= row0.w;
			tmd *= row0.w;
			row0 -= tmp;
			idn0 -= tmd;
		}

		if (row1.w) {
			tmp = row3;
			tmd = idn3;

			tmp *= row1.w;
			tmd *= row1.w;
			row1 -= tmp;
			idn1 -= tmd;
		}

		if (row2.w) {
			tmp = row3;
			tmd = idn3;

			tmp *= row2.w;
			tmd *= row2.w;
			row2 -= tmp;
			idn2 -= tmd;
		}

		//return Matrix44{
		//	idn0.x, idn1.x, idn2.x, idn3.x,
		//	idn0.y, idn1.y, idn2.y, idn3.y,
		//	idn0.z, idn1.z, idn2.z, idn3.z,
		//	idn0.w, idn1.w, idn2.w, idn3.w
		//};

		return Matrix44{
			idn0.x, idn0.y, idn0.z, idn0.w,
			idn1.x, idn1.y, idn1.z, idn1.w,
			idn2.x, idn2.y, idn2.z, idn2.w,
			idn3.x, idn3.y, idn3.z, idn3.w
		};
#endif
	}
#pragma endregion

}//namespace zg
