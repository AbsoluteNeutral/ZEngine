/*****************************************************************************/
/*!
\file		Vector4.h
\brief		Vector4 Implmentation
Uploaded: 18 May 2019
*/
/*****************************************************************************/
#ifndef _VECTOR4_H
#define _VECTOR4_H

#include "Reals.h"

namespace zg {
	class Vector2;
	class Vector3;

	class ZG_API Vector4
	{
	public:
		realtype x, y, z, w;
		Vector4			()											noexcept;
		Vector4			(realtype x_, realtype y_, realtype z_, realtype w_)	noexcept;
		Vector4			(const Vector3& vector3, realtype w_)			noexcept;
		explicit Vector4(const Vector2 &v)							noexcept;
		explicit Vector4(const Vector3 &v)							noexcept;
	
		//static
		static const Vector4 Zero;
		static Vector4 Min(const Vector4& v1, const Vector4& v2);
		static Vector4 Max(const Vector4& v1, const Vector4& v2);

		//setters
		realtype	Normalize();
		void		SetZero();
		void		SetOne();
		
		//getters
		Vector4		Normalized()			const;
		realtype	Dot(const Vector4& v_)	const;
		bool		IsUnit()				const;
		bool		IsZero()				const;
		realtype	LengthSq()				const;
		realtype	Length()				const;

		//operator
		realtype& operator[]	(int index);
		realtype  operator[]	(int index)		  const;
		Vector4& operator=		(const Vector2& v_);
		Vector4& operator=		(const Vector3& v_);
		Vector4& operator+=		(const Vector4& v_);
		Vector4& operator-=		(const Vector4& v_);
		Vector4& operator*=		(const Vector4& v_);
		Vector4& operator*=		(realtype f);
		Vector4& operator/=		(realtype f);
		bool	 operator==		(const Vector4& v_) const;
	
	#ifdef USING_SOL2
		Vector4 operator- ()				const;
		Vector4 operator- (const Vector4 &r)const;
		Vector4 operator- (realtype f)			const;
		Vector4 operator+ (const Vector4 &r)const;
		Vector4 operator+ (realtype f)			const;
		Vector4 operator* (const Vector4 &r)const;
		Vector4 operator* (realtype f)			const;
		Vector4 operator/ (realtype f)			const;
	
		static void BindLua(sol::state& lua);
	#else
		ZG_API friend Vector4 operator- (const Vector4 &l);
		ZG_API friend Vector4 operator- (const Vector4 &l, const Vector4 &r);
		ZG_API friend Vector4 operator- (const Vector4 &l, realtype f);
		ZG_API friend Vector4 operator+ (const Vector4 &l, const Vector4 &r);
		ZG_API friend Vector4 operator+ (const Vector4 &l, realtype f);
		ZG_API friend Vector4 operator* (const Vector4 &l, const Vector4 &r);
		ZG_API friend Vector4 operator* (const Vector4 &l, realtype f);
		ZG_API friend Vector4 operator/ (const Vector4 &l, realtype f);
	#endif
		ZG_API friend Vector4 operator- (realtype f, const Vector4 &r);
		ZG_API friend Vector4 operator+ (realtype f, const Vector4 &r);
		ZG_API friend Vector4 operator* (realtype f, const Vector4 &l);
		ZG_API friend std::ostream& operator<<(std::ostream& os, const Vector4& m);
	};

	ZG_API Vector4	ABS				(const Vector4& v_);
	ZG_API bool		ApproxEqual		(const Vector4& v0_, const Vector4& v1_, realtype epsilon);
	ZG_API realtype	DistanceSq		(const Vector4& v0_, const Vector4& v1_);
	ZG_API realtype	Dot				(const Vector4& v0_, const Vector4& v1_);
	ZG_API realtype Length			(const Vector4& v_);
	ZG_API realtype LengthSq		(const Vector4& v_);
	ZG_API Vector4	Lerp			(const Vector4& start_, const Vector4& end_, float time_);
	ZG_API Vector4	nLerp			(const Vector4& start_, const Vector4& end_, float time_);
	ZG_API Vector4	LinearRand		(const Vector4& v0_, const Vector4& v1_);
	ZG_API realtype Magnitude		(const Vector4& v_);
	ZG_API realtype MagnitudeSq		(const Vector4& v_);
	ZG_API realtype Normalize		(Vector4& v_);
	ZG_API Vector4	Normalized		(const Vector4& v_);

}	//namespace zg

#endif //_VECTOR4_H
