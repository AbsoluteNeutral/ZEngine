
#ifndef _MATRIX33_H
#define _MATRIX33_H

#include "Reals.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Quaternion.h"

#pragma warning(disable : 4201) //nameless struct union

namespace zg{
	class ZG_API Matrix33
	{
	public:
		union {
			struct {
				float m0, m1, m2,
					  m3, m4, m5,
					  m6, m7, m8;
			};
			struct {
				float m00, m01, m02,
					  m10, m11, m12,
					  m20, m21, m22;
			};
			float m[9];
			float md[3][3];
		};
		Matrix33() noexcept;
		Matrix33(float _00, float _01, float _02,
				 float _10, float _11, float _12,
				 float _20, float _21, float _22) noexcept;
	
		//static
		static const Matrix33	Identity;
		static const Matrix33	Zero;

		//setters
		void		Set			(float _00, float _01, float _02,
								 float _10, float _11, float _12,
								 float _20, float _21, float _22);
		void		SetIdentity		();
		void		SetZero			();
		void		Transpose		();
	
		//getters
		Matrix33	GetAbsouleMatrix() const;
		float		GetDeterminant	() const;
		Vector3		GetCol			(unsigned index_) const;
		Vector3		GetRow			(unsigned index_) const;
		float		Trace			() const;
		Matrix33	Transposed		() const;

		operator float*();
		Matrix33& operator*=(float f_);
		Matrix33& operator+=(const Matrix33& matrix_);
		Matrix33& operator-=(const Matrix33& matrix_);
		
	#ifdef USING_SOL2
		Matrix33 operator*(const Matrix33 &rhs) const;
		Vector2  operator*(const Vector2 &rhs)  const;
		Vector3  operator*(const Vector3 &rhs)  const;
	
		static void BindLua(sol::state& lua);
	#else
		ZG_API friend Matrix33 operator*(const Matrix33& matrix_, const Matrix33& rhs);
		ZG_API friend Vector2  operator*(const Matrix33& matrix_, const Vector2& v_);
		ZG_API friend Vector3  operator*(const Matrix33& matrix_, const Vector3& v_);
	#endif
		ZG_API friend std::ostream& operator<<(std::ostream& os, const Matrix33& m);
	};

	ZG_API void BuildScale		(Matrix33& matrix_, float x_, float y_);
	ZG_API void BuildScale		(Matrix33& matrix_, Vector2& v_);
	ZG_API void BuildScale		(Matrix33& matrix_, Vector3& v_);
	ZG_API void BuildTranslate	(Matrix33& matrix_, float x_, float y_);
	ZG_API void BuildTranslate	(Matrix33& matrix_, const Vector2& v_);
	ZG_API void BuildTranslate	(Matrix33& matrix_, const Vector3& v_);
	ZG_API void	BuildTRS		(Matrix33& matrix_, const Vector3& translation_, const Vector3& scale_, float degree_angleZ_);

	ZG_API Matrix33	BuildRotation3x3	(const Quaternion& q);
	ZG_API Matrix33	BuildScale3x3		(float x_, float y_);
	ZG_API Matrix33	BuildScale3x3		(const Vector2& v_);
	ZG_API Matrix33	BuildScale3x3		(const Vector3& v_);
	ZG_API Matrix33	BuildTranslate3x3	(float x_, float y_);
	ZG_API Matrix33	BuildTranslate3x3	(const Vector2& v_);
	ZG_API Matrix33	BuildTranslate3x3	(const Vector3& v_);


	ZG_API void		BuildSkew			(Matrix33& matrix_, const Vector3& v_);
	ZG_API Matrix33	BuildSkew3x3		(const Vector3& v_);
	ZG_API Matrix33	GaussJordanInverse	(const Matrix33& mat);
	ZG_API Vector2	TransformPoint		(const Matrix33& matrix_, const Vector2& v_);
	ZG_API Vector2	TransformVector		(const Matrix33& matrix_, const Vector2& v_);

} //namespace zg
#endif //_MATRIX33_H
