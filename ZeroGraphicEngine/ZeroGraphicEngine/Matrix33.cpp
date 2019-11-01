
#include "stdafx.h"
#include "Matrix33.h"

namespace zg {
	//static
	const Matrix33	Matrix33::Identity = Matrix33{	1.0f, 0.0f, 0.0f,
													0.0f, 1.0f, 0.0f,
													0.0f, 0.0f, 1.0f };
	const Matrix33	Matrix33::Zero{};

	Matrix33::Matrix33() noexcept
		: m{ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }
	{}
	Matrix33::Matrix33(float _00, float _01, float _02,
					   float _10, float _11, float _12,
					   float _20, float _21, float _22) noexcept
#ifdef ROW_MAJOR
		:m{ _00, _01, _02,
			_10, _11, _12,
			_20, _21, _22 }
#else
		: m{ _00, _10, _20,
			 _01, _11, _21,
			 _02, _12, _22 }
#endif
	{}

	//setters
	void Matrix33::Set(float _00, float _01, float _02,
					   float _10, float _11, float _12,
					   float _20, float _21, float _22) {
		m00 = _00;	m01 = _01;	 m02 = _02;
		m10 = _10;	m11 = _11;	 m12 = _12;
		m20 = _20;	m21 = _21;	 m22 = _22;
	}

	void Matrix33::SetIdentity() {
		int M = 3;
		float *b = *md;			/*begin of matrix*/
		float *e = *md + M * M; /*end of matrix  */
		float *e3 = *md + M;    /*end of each row*/
		while (b < e) {
			float *b1 = b + 1;  /*get next col of matrix*/
			float *b2 = b + M;  /*get next row of matrix*/
			while (b1 != e3) {
				*b1 = 0.0f;
				*b2 = 0.0f;
				++b1;           /*get next row of matrix*/
				b2 = b2 + M;    /*get next col of matrix*/
			}
			*b = 1.0f;
			b = b + M + 1;    /*get next diagonal matrix*/
			e3 = e3 + M;      /*get end of next row*/
		}
		//#version 2
		//m8 = m4 = m0 = 1.0f;
		//m1 = m2 = m3 =
		//m5 = m6 = m7 = 0.0f;
		//#version 1
		//m00 = 1.0f; m01 = 0.0f; m02 = 0.0f;
		//m10 = 0.0f; m11 = 1.0f; m12 = 0.0f;
		//m20 = 0.0f; m21 = 0.0f; m22 = 1.0f;
	}
	void Matrix33::SetZero() {
		float* b = m;
		float* e = m + 9;
		while (b != e) {
			*b = 0.0f; *(b + 1) = 0.0f; *(b + 2) = 0.0f; 
			b += 3;
		}
	}
	void Matrix33::Transpose() {
		int M = 3;
		float *b = *md;			/*begin of matrix*/
		float *e = *md + M * M; /*end of matrix  */
		float *e3 = *md + M;    /*end of each row*/
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
	Matrix33 Matrix33::GetAbsouleMatrix() const {
		return Matrix33{
			fabs(md[0][0]), 	fabs(md[0][1]),	 fabs(md[0][2]),
			fabs(md[1][0]), 	fabs(md[1][1]),	 fabs(md[1][2]),
			fabs(md[2][0]), 	fabs(md[2][1]),	 fabs(md[2][2])
		};
	}
	float Matrix33::GetDeterminant() const {
#ifdef ROW_MAJOR
		//float tmp = m[0] * (m[4] * m[8] - m[7] * m[5]) -
		//		      m[1] * (m[3] * m[8] - m[6] * m[5]) +
		//		      m[2] * (m[3] * m[7] - m[6] * m[4]);
		float tmp = md[0][0] * (md[1][1] * md[2][2] - md[2][1] * md[1][2]) -
					md[0][1] * (md[1][0] * md[2][2] - md[2][0] * md[1][2]) +
					md[0][2] * (md[1][0] * md[2][1] - md[2][0] * md[1][1]);
#else
		float tmp = md[0][0] * (md[1][1] * md[2][2] - md[1][2] * md[2][1]) -
					md[1][0] * (md[0][1] * md[2][2] - md[0][2] * md[2][1]) +
					md[2][0] * (md[0][1] * md[1][2] - md[0][2] * md[1][1]);
#endif
		return (tmp < EPSILON && tmp > -EPSILON) ? 0.0f : tmp;
	}
	Vector3 Matrix33::GetCol(unsigned index_) const {
#ifdef ROW_MAJOR
		return Vector3{ m[index_],
						m[4 + index_] ,
						m[8 + index_] };
#else
		unsigned i = index_ * 3;
		return Vector3{ m[i],
						m[i + 1],
						m[i + 2] };
#endif
	}
	Vector3 Matrix33::GetRow(unsigned index_) const {
#ifdef ROW_MAJOR
		unsigned i = index_ * 3;
		return Vector3{ m[i],
						m[i + 1],
						m[i + 2] };
#else
		return Vector3{ m[index_],
						m[4 + index_] ,
						m[8 + index_] };
#endif
	}

	float Matrix33::Trace() const {	return m00 + m11 + m22; }
	Matrix33 Matrix33::Transposed() const {
#ifdef ROW_MAJOR
		Matrix33 tmp{ *this };
		tmp.Transpose();
		return tmp;
#else
		return Matrix33{ *this };
#endif
	}

	//operator
	Matrix33::operator float*() { return m; }
	Matrix33& Matrix33::operator*=(float f_) {
		float* b = m;
		float* e = m + 9;
		while (b != e) {
			*b *= f_; *(b + 1) *= f_; *(b + 2) *= f_;
			b += 3;
		}
		//#version 1
		//m0 *= f;  m1 *= f; m2  *= f;
		//m3  *= f; m4 *= f; m5 *= f; 
		//m6  *= f; m7 *= f; m8 *= f;
		return *this;
	}
	Matrix33& Matrix33::operator+=(const Matrix33& matrix_) {
		const float* b1 = matrix_.m;
		float* b = m;
		float* e = m + 9;
		while (b != e) {
			*b += *b1;
			*(b + 1) += *(b1 + 1);
			*(b + 2) += *(b1 + 2);
			b += 3; b1 += 3;
		}
		//#version 1
		//m0 += matrix_.m0; m1 += matrix_.m1; m2 += matrix_.m2;
		//m3 += matrix_.m3; m4 += matrix_.m4; m5 += matrix_.m5;
		//m6 += matrix_.m6; m7 += matrix_.m7; m8 += matrix_.m8;
		return *this;
	}

	Matrix33& Matrix33::operator-= (const Matrix33& matrix_) {
		const float* b1 = matrix_.m;
		float* b = m;
		float* e = m + 9;
		while (b != e) {
			*b -= *b1;
			*(b + 1) -= *(b1 + 1);
			*(b + 2) -= *(b1 + 2);
			b += 3; b1 += 3;
		}
		//#version 1
		//m0 -= matrix_.m0; m1 -= matrix_.m1; m2 -= matrix_.m2;
		//m3 -= matrix_.m3; m4 -= matrix_.m4; m5 -= matrix_.m5;
		//m6 -= matrix_.m6; m7 -= matrix_.m7; m8 -= matrix_.m8;
		return *this;
	}

	
#ifdef USING_SOL2
	Matrix33 Matrix33::operator*(const Matrix33 &rhs) const {
#ifdef ROW_MAJOR
		return Matrix33{
			 m[0] * rhs.m[0] + m[1] * rhs.m[3] + m[2] * rhs.m[6],
			 m[0] * rhs.m[1] + m[1] * rhs.m[4] + m[2] * rhs.m[7],
			 m[0] * rhs.m[2] + m[1] * rhs.m[5] + m[2] * rhs.m[8],
			 m[3] * rhs.m[0] + m[4] * rhs.m[3] + m[5] * rhs.m[6],
			 m[3] * rhs.m[1] + m[4] * rhs.m[4] + m[5] * rhs.m[7],
			 m[3] * rhs.m[2] + m[4] * rhs.m[5] + m[5] * rhs.m[8],
			 m[6] * rhs.m[0] + m[7] * rhs.m[3] + m[8] * rhs.m[6],
			 m[6] * rhs.m[1] + m[7] * rhs.m[4] + m[8] * rhs.m[7],
			 m[6] * rhs.m[2] + m[7] * rhs.m[5] + m[8] * rhs.m[8]
		};
#else
		return Matrix33{
			rhs.m[0] * m[0] + rhs.m[1] * m[3] + rhs.m[2] * m[6],
			rhs.m[0] * m[1] + rhs.m[1] * m[4] + rhs.m[2] * m[7],
			rhs.m[0] * m[2] + rhs.m[1] * m[5] + rhs.m[2] * m[8],
			rhs.m[3] * m[0] + rhs.m[4] * m[3] + rhs.m[5] * m[6],
			rhs.m[3] * m[1] + rhs.m[4] * m[4] + rhs.m[5] * m[7],
			rhs.m[3] * m[2] + rhs.m[4] * m[5] + rhs.m[5] * m[8],
			rhs.m[6] * m[0] + rhs.m[7] * m[3] + rhs.m[8] * m[6],
			rhs.m[6] * m[1] + rhs.m[7] * m[4] + rhs.m[8] * m[7],
			rhs.m[6] * m[2] + rhs.m[7] * m[5] + rhs.m[8] * m[8]
		};
#endif
	}

	Vector2 Matrix33::operator*(const Vector2 &rhs) const {
#ifdef ROW_MAJOR
		return Vector2{
			rhs.x * m[0] + rhs.y * m[1] + m[2],
			rhs.x * m[3] + rhs.y * m[4] + m[5]
		};
#else
		return Vector2{
			rhs.x * m[0] + rhs.y * m[3] + m[6],
			rhs.x * m[1] + rhs.y * m[4] + m[7]
		};
#endif
	}

	Vector3 Matrix33::operator*(const Vector3 &rhs) const {
#ifdef ROW_MAJOR
		return Vector3{
			rhs.x * m[0] + rhs.y * m[1] + rhs.z * m[2],
			rhs.x * m[3] + rhs.y * m[4] + rhs.z * m[5],
			rhs.x * m[6] + rhs.y * m[7] + rhs.z * m[8]
		};
#else
		return Vector3{
			rhs.x * m[0] + rhs.y * m[3] + rhs.z * m[6],
			rhs.x * m[1] + rhs.y * m[4] + rhs.z * m[7],
			rhs.x * m[2] + rhs.y * m[5] + rhs.z * m[8]
		};
#endif
	}
	void Matrix33::BindLua(sol::state & lua)
	{
		lua.new_usertype<Matrix33>("Matrix33",
			sol::constructors<
			Matrix33(),
			Matrix33(float, float, float, float, float, float, float, float, float)
			>(),

			"GetDeterminant", &Matrix33::GetDeterminant,
			"GetTransposed", &Matrix33::GetTransposed,

			sol::meta_function::multiplication, sol::overload(

				static_cast<Matrix33(Matrix33::*)(const Matrix33&) const>(&Matrix33::operator*),
				static_cast<Vector2(Matrix33::*)(const Vector2&) const>(&Matrix33::operator*),
				static_cast<Vector3(Matrix33::*)(const Vector3&) const>(&Matrix33::operator*)
			)

			);
	}
#else
	Matrix33 operator*(const Matrix33& lhs, const Matrix33& rhs) {
#ifdef ROW_MAJOR
		return Matrix33{
			 lhs.m[0] * rhs.m[0] + lhs.m[1] * rhs.m[3] + lhs.m[2] * rhs.m[6],
			 lhs.m[0] * rhs.m[1] + lhs.m[1] * rhs.m[4] + lhs.m[2] * rhs.m[7],
			 lhs.m[0] * rhs.m[2] + lhs.m[1] * rhs.m[5] + lhs.m[2] * rhs.m[8],
			 lhs.m[3] * rhs.m[0] + lhs.m[4] * rhs.m[3] + lhs.m[5] * rhs.m[6],
			 lhs.m[3] * rhs.m[1] + lhs.m[4] * rhs.m[4] + lhs.m[5] * rhs.m[7],
			 lhs.m[3] * rhs.m[2] + lhs.m[4] * rhs.m[5] + lhs.m[5] * rhs.m[8],
			 lhs.m[6] * rhs.m[0] + lhs.m[7] * rhs.m[3] + lhs.m[8] * rhs.m[6],
			 lhs.m[6] * rhs.m[1] + lhs.m[7] * rhs.m[4] + lhs.m[8] * rhs.m[7],
			 lhs.m[6] * rhs.m[2] + lhs.m[7] * rhs.m[5] + lhs.m[8] * rhs.m[8]
		};
#else
		return Matrix33{
			rhs.m[0] * lhs.m[0] + rhs.m[1] * lhs.m[3] + rhs.m[2] * lhs.m[6],
			rhs.m[0] * lhs.m[1] + rhs.m[1] * lhs.m[4] + rhs.m[2] * lhs.m[7],
			rhs.m[0] * lhs.m[2] + rhs.m[1] * lhs.m[5] + rhs.m[2] * lhs.m[8],
			rhs.m[3] * lhs.m[0] + rhs.m[4] * lhs.m[3] + rhs.m[5] * lhs.m[6],
			rhs.m[3] * lhs.m[1] + rhs.m[4] * lhs.m[4] + rhs.m[5] * lhs.m[7],
			rhs.m[3] * lhs.m[2] + rhs.m[4] * lhs.m[5] + rhs.m[5] * lhs.m[8],
			rhs.m[6] * lhs.m[0] + rhs.m[7] * lhs.m[3] + rhs.m[8] * lhs.m[6],
			rhs.m[6] * lhs.m[1] + rhs.m[7] * lhs.m[4] + rhs.m[8] * lhs.m[7],
			rhs.m[6] * lhs.m[2] + rhs.m[7] * lhs.m[5] + rhs.m[8] * lhs.m[8]
		};
#endif
	}

	Vector2 operator*(const Matrix33& matrix_, const Vector2& v_) {
#ifdef ROW_MAJOR
		return Vector2{
			v_.x * matrix_.m0 + v_.y * matrix_.m1 + matrix_.m2,
			v_.x * matrix_.m3 + v_.y * matrix_.m4 + matrix_.m5
		};
#else
		return Vector2{
			v_.x * matrix_.m0 + v_.y * matrix_.m3 + matrix_.m6,
			v_.x * matrix_.m1 + v_.y * matrix_.m4 + matrix_.m7
		};
#endif
	}

	Vector3 operator*(const Matrix33 &matrix_, const Vector3 &v_) {
#ifdef ROW_MAJOR
		return Vector3{
			 matrix_.m0 * v_.x + matrix_.m1 * v_.y + matrix_.m2 * v_.z,
			 matrix_.m3 * v_.x + matrix_.m4 * v_.y + matrix_.m5 * v_.z,
			 matrix_.m6 * v_.x + matrix_.m7 * v_.y + matrix_.m8 * v_.z
		};
#else
		return Vector3{
			matrix_.m0  * v_.x + matrix_.m3 * v_.y + matrix_.m6 * v_.z,
			matrix_.m1  * v_.x + matrix_.m4 * v_.y + matrix_.m7 * v_.z,
			matrix_.m2  * v_.x + matrix_.m5 * v_.y + matrix_.m8 * v_.z
		};
#endif
	}
#endif

	std::ostream& operator<<(std::ostream& os, const Matrix33& matrix_) {
		printf("{ %.6f %.6f %.6f \n  %.6f %.6f %.6f\n  %.6f %.6f %.6f }\n",
			matrix_.m0, matrix_.m1, matrix_.m2,
			matrix_.m3, matrix_.m4, matrix_.m5,
			matrix_.m6, matrix_.m7, matrix_.m8
		);
		//os << lhs.m[0] << " " << lhs.m[1] << " " << lhs.m[2] << "\n"
		//   << lhs.m[3] << " " << lhs.m[4] << " " << lhs.m[5] << "\n"
		//   << lhs.m[6] << " " << lhs.m[7] << " " << lhs.m[8] << "}\n\n";
		return os;
	}


	//extern function
	void BuildScale(Matrix33& matrix_, float x_, float y_) {
		//matrix.md[0][0] = x; matrix.md[1][1] = y;
		matrix_ = Matrix33{ x_,		0.0f,	0.0f,
							0.0f,	y_,		0.0f,
							0.0f,	0.0f,	1.0f };
	}
	void BuildScale(Matrix33& matrix_, const Vector2& v_) { BuildScale(matrix_, v_.x, v_.y); }
	void BuildScale(Matrix33& matrix_, const Vector3& v_) { BuildScale(matrix_, v_.x, v_.y); }

	void BuildTranslate(Matrix33& matrix_, float x_, float y_) {
	//	matrix_.SetZero();
	//#ifdef ROW_MAJOR
	//	matrix_.m02 = x_; matrix_.m12 = y_;
	//#else //COL_MAJOR
	//	matrix_.m20 = x_; matrix_.m21 = y_;
	//#endif
		matrix_ = Matrix33{	1.0f, 0.0f, x_,
							0.0f, 1.0f, y_,
							0.0f, 0.0f, 1.0f };
	}
	void BuildTranslate(Matrix33& matrix_, const Vector2& v_) { BuildTranslate(matrix_, v_.x, v_.y);	}
	void BuildTranslate(Matrix33& matrix_, const Vector3& v_) { BuildTranslate(matrix_, v_.x, v_.y);	}

	void BuildTRS(Matrix33& matrix_, const Vector3& translation_, const Vector3& scale_, float degree_angleZ_) {
		float r = TO_RAD(degree_angleZ_);
		float c = std::cosf(r);
		float s = std::sinf(r);
		matrix_ = Matrix33{ scale_.x * c, scale_.y * -s, translation_.x,
							scale_.x * s, scale_.y * c,  translation_.y,
							0.0f,		  0.0f,			1.0f };
//#ifdef ROW_MAJOR
//		matrix_.md[0][0] = scale_.x * c;	matrix_.md[0][1] = scale_.y * -s;	matrix_.md[0][2] = translation_.x;
//		matrix_.md[1][0] = scale_.x * s;	matrix_.md[1][1] = scale_.y * c;	matrix_.md[1][2] = translation_.y;
//		matrix_.md[2][0] = 0.0f;			matrix_.md[2][1] = 0.0f;			matrix_.md[2][2] = 1.0f;
//#else
//		matrix_.md[0][0] = scale_.x * c;	matrix_.md[0][1] = scale_.x * s;	matrix_.md[0][2] = 0.0f;
//		matrix_.md[1][0] = scale_.y *-s;	matrix_.md[1][1] = scale_.y * c;	matrix_.md[1][2] = 0.0f;
//		matrix_.md[2][0] = translation_.x;  matrix_.md[2][1] = translation_.y;	matrix_.md[2][2] = 1.0f;
//#endif
	}

	Matrix33 BuildScale3x3(float x_, float y_) {
		return Matrix33{
			x_,	  0.0f, 0.0f,
			0.0f, y_,	0.0f,
			0.0f, 0.0f, 1.0f
		};
	}
	Matrix33 BuildScale3x3(const Vector2& v_) { return BuildScale3x3(v_.x, v_.y); }
	Matrix33 BuildScale3x3(const Vector3& v_) { return BuildScale3x3(v_.x, v_.y); }

	Matrix33 BuildTranslate3x3(float x_, float y_) {
		return Matrix33{
			1.0f, 0.0f, x_,
			0.0f, 1.0f, y_,
			0.0f, 0.0f, 1.0f
		};
	}
	Matrix33 BuildTranslate3x3(const Vector2& v_) {	return BuildTranslate3x3(v_.x, v_.y); }
	Matrix33 BuildTranslate3x3(const Vector3& v_) {	return BuildTranslate3x3(v_.x, v_.y); }

	Matrix33 BuildRotation3x3(const Quaternion& q) {
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
		return Matrix33{ 1.0f - yys - zzs,	xys - wzs,			xzs + wys,
			xys + wzs,			1.0f - xxs - zzs,	yzs - wxs,
			xzs - wys,			yzs + wxs,			1.0f - xxs - yys };
		//float xx = q.x * q.x;
		//float yy = q.y * q.y;
		//float zz = q.z * q.z;
		//float xy = q.x * q.y;
		//float xz = q.x * q.z;
		//float yz = q.y * q.z;
		//float wx = q.w * q.x;
		//float wy = q.w * q.y;
		//float wz = q.w * q.z;
		//return Matrix33{
		//	1.0f - 2.0f * (yy + zz),	2.0f * (xy - wz),			2.0f * (xz + wy),	
		//	2.0f * (xy + wz),			1.0f - 2.0f * (xx + zz),	2.0f * (yz - wx),		
		//	2.0f * (xz - wy),			2.0f * (yz + wx),			1.0f - 2.0f * (xx + yy)
		//};
	}
	void BuildSkew(Matrix33& matrix, const Vector3& v_) {
		matrix = Matrix33{
			0.0f,	-v_.z,	 v_.y,
			v_.z,	 0.0f,	 -v_.x,
			-v_.y,  v_.x,	0.0f
		};
	}
	Matrix33 BuildSkew3x3(const Vector3& v_){
		return Matrix33{
			0.0f,	-v_.z,	 v_.y,
			v_.z,	 0.0f,	 -v_.x,
			-v_.y,  v_.x,	 0.0f
		};
	}

	Matrix33 GaussJordanInverse(const Matrix33& matrix_) {
		Vector3 idn[3]{ { 1.0f, 0.0f, 0.0f },
						{ 0.0f, 1.0f, 0.0f },
						{ 0.0f, 0.0f, 1.0f } };
		Vector3 row[3]{ matrix_.GetRow(0),
			matrix_.GetRow(1),
			matrix_.GetRow(2) };

		for (int i = 0; i < 3; ++i) {
			float divider = row[i][i];
			row[i] /= divider;
			idn[i] /= divider;
			for (int j = 0; j < 3; ++j) {
				if (i == j) continue;
				if (row[j][i]) {
					Vector3 tmp = row[i];
					Vector3 tmd = idn[i];
					tmp *= row[j][i];
					tmd *= row[j][i];
					row[j] -= tmp;
					idn[j] -= tmd;
				}
			}
		}
		return Matrix33{
			idn[0][0], idn[0][1], idn[0][2],
			idn[1][0], idn[1][1], idn[1][2],
			idn[2][0], idn[2][1], idn[2][2]
		};
	}

	Vector2	TransformPoint	(const Matrix33& matrix_, const Vector2& vector_) {	return Vector2{ matrix_ * Vector3{ vector_, 1.0f } };	}
	Vector2	TransformVector	(const Matrix33& matrix_, const Vector2& vector_) {	return Vector2{ matrix_ * Vector3{ vector_, 0.0f } };	}

}//namespace zg
