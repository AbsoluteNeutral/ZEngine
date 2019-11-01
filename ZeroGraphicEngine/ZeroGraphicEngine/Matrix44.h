
#ifndef _MATRIX44_H
#define _MATRIX44_H

#include "Reals.h"
#include "Vector3.h"
#include "Quaternion.h"

#pragma warning(disable : 4201) //nameless struct union

namespace zg {
	class ZG_API Matrix44
	{
	public:
		union {
			struct
			{
				float m00, m01, m02, m03,
					  m10, m11, m12, m13,
					  m20, m21, m22, m23,
					  m30, m31, m32, m33;
			};
			struct
			{
				float m0,  m1,  m2,  m3,
					  m4,  m5,  m6,  m7,
					  m8,  m9,  m10, m11,
					  m12, m13, m14,  m15;
			};
			float m[16];
			float md[4][4];
		};
		Matrix44() noexcept;
		Matrix44(float _00, float _01, float _02, float _03,
				 float _10, float _11, float _12, float _13,
				 float _20, float _21, float _22, float _23,
				 float _30, float _31, float _32, float _33) noexcept;
	
		//static
		static const Matrix44 Identity;
		static const Matrix44 Zero;

		//setters
		void SetIdentity	();
		void SetZero		();
		void Transpose		();

		//getters
		float*		GetPtr		();
		Vector4		GetCol		(unsigned index_) const;
		Vector4		GetRow		(unsigned index_) const;
		float		Trace		() const;
		Matrix44	Transposed	() const;

		//operators
		operator float*();
		Matrix44& operator*=(float f_);
		Matrix44& operator-=(const Matrix44& matrix_);
		Matrix44& operator+=(const Matrix44& matrix_);
		

	#ifdef USING_SOL2
		Matrix44 operator+(const Matrix44 &rhs) const;
		Matrix44 operator*(const Matrix44 &rhs) const;
		Vector2 operator* (const Vector2& rhs)  const;
		Vector3 operator* (const Vector3& rhs)  const;
		zg::Vector4 operator* (const zg::Vector4& rhs)  const;
		static void BindLua(sol::state& lua);
	#else
		ZG_API friend Matrix44	operator-	(const Matrix44& lhs, const Matrix44& rhs);
		ZG_API friend Matrix44	operator+	(const Matrix44& lhs, const Matrix44& rhs);
		ZG_API friend Matrix44	operator*	(const Matrix44& lhs, const Matrix44& rhs);
		ZG_API friend Vector2	operator*	(const Matrix44& lhs, const Vector2& pt_);
		ZG_API friend Vector3	operator*	(const Matrix44& lhs, const Vector3& pt_);
		ZG_API friend Vector4	operator*	(const Matrix44& lhs, const Vector4& v_);
	#endif
		ZG_API friend std::ostream& operator<<(std::ostream& os, const Matrix44& matrix_);
	};

	//extern function
	ZG_API Matrix44		BuildTranslate4x4	(float x_, float y_, float z_);
	ZG_API Matrix44		BuildTranslate4x4	(const Vector3& v_);
	ZG_API Matrix44		BuildRotation4x4	(float degree_, const Vector3& axis_);
	ZG_API Matrix44		BuildRotation4x4	(const Quaternion& q_);
	ZG_API Matrix44		BuildRotation4x4	(const Quaternion& q_, const Vector3& translation_);
	ZG_API Matrix44		BuildScale4x4		(float x_);
	ZG_API Matrix44		BuildScale4x4		(float x_, float y_, float z_);
	ZG_API Matrix44		BuildScale4x4		(const Vector3& scale_);

	ZG_API void			BuildTRS			(Matrix44& m, const Vector3& translation_, const Vector3& scale_, float angleZ_);
	ZG_API void			BuildTRS			(Matrix44& m, const Vector3& translation_, const Vector3& scale_, const Vector3& angle_);
	ZG_API void			BuildTRS			(Matrix44& m, const Vector3& translation_, const Vector3& scale_, const Vector3& axis_, float angle_);
	ZG_API Matrix44		BuildTRS4x4			(const Vector3& translation_, const Quaternion& quaternion_, const Vector3& scale_);
	
	ZG_API Matrix44		BuildSkew4x4		(const Vector3& v_);
	//ETC
	ZG_API Matrix44		LookAtRH			(const Vector3& eye, const Vector3& target, const Vector3& up);
	ZG_API Matrix44		LookAtRHDir			(const Vector3& eye, const Vector3& direction, const Vector3& up);
	ZG_API Matrix44		LookAtLH			(const Vector3& eye, const Vector3& target, const Vector3& up);
	ZG_API Matrix44		OrthRH				(float left, float right, float bottom, float top, float near_, float far_);
	ZG_API Matrix44		Orth				(float left, float right, float bottom, float top);
	ZG_API Matrix44		PerspectiveRH		(float fovRAD_, float aspect_, float near_, float far_);
	ZG_API Matrix44		FrustumRH			(float left_, float right_, float bottom_, float top_, float near_, float far_);
	ZG_API Matrix44		ViewPortTransform	(int width_, int height_);
	
	ZG_API float		Determinant			(const Matrix44& matrix_, int dimension_);
	
	ZG_API Matrix44		GaussJordanInverse	(const Matrix44& matrix_);
	ZG_API Vector3		GetPosition			(const Matrix44& matrix_);
	ZG_API Vector3		GetScale			(const Matrix44& matrix_);
	ZG_API Quaternion	GetRotation			(const Matrix44& matrix_);
	ZG_API Vector3		TransformPoint		(const Matrix44& matrix_, const Vector3& point_);
	ZG_API Vector3		TransformVector		(const Matrix44& matrix_, const Vector3& vector_);
	
	//ZG_API Matrix44		Inverse				(const Matrix44& mat);
	

}//namespace zg

#endif
