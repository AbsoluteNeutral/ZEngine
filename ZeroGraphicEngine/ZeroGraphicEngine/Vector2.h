/*****************************************************************************/
/*!
\file		Vector2.h
\brief		Vector2 implementation
Uploaded: 18 May 2019
*/
/*****************************************************************************/
#ifndef _VECTOR2_H
#define _VECTOR2_H

#include "Reals.h"

namespace zg {
	class Vector3;
	class ZG_API Vector2
	{
	public:
		realtype x, y;
	
		//constructor
		Vector2				()							noexcept;
		Vector2				(realtype x_, realtype y_)	noexcept;
		explicit Vector2	(const Vector3& rhs)		noexcept;
	
		//static
		static const Vector2 Zero;
		static const Vector2 VectorX;
		static const Vector2 VectorY;
		static Vector2 Min(const Vector2& v1, const Vector2& v2);
		static Vector2 Max(const Vector2& v1, const Vector2& v2);
		
	
		//setters
		realtype	Normalize	();
		void		Set			(realtype x_, realtype y_);
		void		SetZero		();
	
		//getters
		realtype	Dot						(const Vector2 &p1) const;
		Vector2		GetAbsVector			()					const;
		int			GetMinAxis				()					const;
		int			GetMaxAxis				()					const;
		Vector2		Normalized			()					const;
		Vector2		GetUnitOrthogonalVector	()					const;
		bool		IsUnit					()					const;
		bool		IsZero					()					const;
		realtype	LengthSq				()					const;
		realtype	Length					()					const;
	
	
	
		//operator
		Vector2& operator=	(const Vector3 &v);
		Vector2& operator+=	(const Vector2 &v);
		Vector2& operator-=	(const Vector2 &v);
		Vector2& operator*=	(realtype f);
		Vector2& operator/=	(realtype f);
		bool     operator== (const Vector2 &r)		const;
		bool     operator!= (const Vector2 &r)		const;
	
		realtype& operator[] (int index);
		const realtype& operator[] (int index)			const;
	#ifdef USING_SOL2
		Vector2 operator- ()						const;
		Vector2 operator- (const Vector2 &r)		const;
		Vector2 operator- (realtype f)					const;
		Vector2 operator+ (const Vector2 &r)		const;
		Vector2 operator+ (realtype f)					const;
		Vector2 operator* (const Vector2 &r)		const;
		Vector2 operator* (realtype f)					const;
		Vector2 operator/ (realtype f)					const;
		Vector2 getDirection(const Vector2& pointB) const;
		static void BindLua(sol::state& lua);
	#else
		ZG_API friend Vector2 operator- (const Vector2 &l);
		ZG_API friend Vector2 operator- (const Vector2 &l, const Vector2 &r);
		ZG_API friend Vector2 operator- (const Vector2 &l, realtype f);
		ZG_API friend Vector2 operator+ (const Vector2 &l, const Vector2 &r);
		ZG_API friend Vector2 operator+ (const Vector2 &l, realtype f);
		ZG_API friend Vector2 operator* (const Vector2 &l, const Vector2 &r);
		ZG_API friend Vector2 operator* (const Vector2 &v, realtype f);
		ZG_API friend Vector2 operator/ (const Vector2 &v, realtype  f);
		ZG_API friend Vector2 getDirection(const Vector2& pointA, const Vector2& pointB);
	#endif
		ZG_API friend Vector2 operator- (realtype f, const Vector2 &r);
		ZG_API friend Vector2 operator+ (realtype f, const Vector2 &r);
		ZG_API friend Vector2 operator* (realtype f, const Vector2 &v);
		ZG_API friend std::ostream& operator<<(std::ostream& os, const Vector2& m);
	};
	
	ZG_API Vector2	ABS				(const Vector2& v_);
	ZG_API bool		ApproxEqual		(const Vector2& vec1, const Vector2& vec2, realtype epsilon);
	ZG_API Vector2  Direction		(realtype y_dir, realtype x_dir);
	ZG_API realtype	DistanceSq		(const Vector2& v0_, const Vector2& v1_);
	ZG_API realtype	Dot				(const Vector2& v0_, const Vector2& v1_);
	ZG_API realtype Length			(const Vector2& v_);
	ZG_API realtype LengthSq		(const Vector2& v_);
	ZG_API Vector2	Lerp			(const Vector2& start_, const Vector2& end_, float time_);
	ZG_API Vector2	nLerp			(const Vector2& start_, const Vector2& end_, float time_);
	ZG_API Vector2	LinearRand		(const Vector2& v0_, const Vector2& v1_);
	ZG_API realtype Magnitude		(const Vector2& v_);
	ZG_API realtype MagnitudeSq		(const Vector2& v_);
	ZG_API realtype Normalize		(Vector2& v_);
	ZG_API Vector2	Normalized		(const Vector2& v_);

}

#endif
