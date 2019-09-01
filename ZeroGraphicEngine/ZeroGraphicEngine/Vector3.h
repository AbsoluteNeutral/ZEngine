/*****************************************************************************/
/*!
\file		Vector3.h
\brief		Vector3 Implmentation
Uploaded: 18 May 2019
*/
/*****************************************************************************/
#ifndef _VECTOR3_H
#define _VECTOR3_H

#include "Reals.h"
#include <vector>

namespace zg {
	class Vector2;
	class Vector4;

	class ZG_API Vector3
	{
	public:
		realtype x, y, z;
	
		//constructor
		Vector3			()										noexcept;
		Vector3			(realtype x_, realtype y_)				noexcept;
		Vector3			(realtype x_, realtype y_, realtype z_) noexcept;
		explicit Vector3(const Vector2& v_, realtype z_)		noexcept;
		explicit Vector3(const Vector2& v_)						noexcept;
		explicit Vector3(const Vector4& v_)						noexcept;
	
		//setters
		realtype	Normalize	();
		void		Set			(realtype x_, realtype y_, realtype z_);
		void		SetZero		();
		void		Reset();
		//getters
		Vector3		Cross					(const Vector3& v)  const;
		realtype	Dot						(const Vector3& v)	const;
		Vector3		GetAbsVector			()					const;
		int			GetMinAxis				()					const;
		int			GetMaxAxis				()					const;
		Vector3		Normalized				()					const;
		Vector3		GetUnitOrthogonalVector	()					const;
		realtype	LengthSq				()					const;
		realtype	Length					()					const;
		bool		IsNan					()					const;
		bool		IsUnit					()					const;
		bool		IsZero					()					const;
		
		//static	
		static const Vector3 Zero;
		static const Vector3 One;
		static const Vector3 Vector3X;
		static const Vector3 Vector3Y;
		static const Vector3 Vector3Z;
		static Vector3 SmoothDamp(const Vector3& start, const Vector3& end, Vector3& velocity, realtype smoothTime, realtype maxSpeed, realtype deltaTime);
		//operator
		realtype& operator[](int index);
		realtype operator[]	(int index) const;
		Vector3& operator=	(const Vector2& v);
		Vector3& operator=	(const Vector4& v);
		Vector3& operator+=	(const Vector2& v);
		Vector3& operator+=	(const Vector3& v);
		Vector3& operator-=	(const Vector3& v);
		Vector3& operator*=	(realtype f);
		Vector3& operator/=	(realtype f);
		bool	 operator==	(const Vector3 &r) const;
	
	#ifdef USING_SOL2
		Vector3 operator- ()				 const;
		Vector3 operator- (const Vector2 &r) const;
		Vector3 operator- (const Vector3 &r) const;
		Vector3 operator- (realtype f)			 const;
		Vector3 operator+ (const Vector2 &r) const;
		Vector3 operator+ (const Vector3 &r) const;
		Vector3 operator+ (realtype f)			 const;
		Vector3 operator* (const Vector3 &r) const;
		Vector3 operator* (realtype f)			 const;
		Vector3 operator/ (realtype f)			 const;
		static void BindLua(sol::state& lua);
	#else
		ZG_API friend Vector3 operator- (const Vector3 &l);
		ZG_API friend Vector3 operator- (const Vector3 &l, const Vector2 &r);
		ZG_API friend Vector3 operator- (const Vector3 &l, const Vector3 &r);
		ZG_API friend Vector3 operator- (const Vector3 &l, realtype f);
		ZG_API friend Vector3 operator+ (const Vector3 &l, const Vector2 &r);
		ZG_API friend Vector3 operator+ (const Vector3 &l, const Vector3 &r);
		ZG_API friend Vector3 operator+ (const Vector3 &l, realtype f);
		ZG_API friend Vector3 operator* (const Vector3 &l, const Vector3 &r);
		ZG_API friend Vector3 operator* (const Vector3 &l, realtype f);
		ZG_API friend Vector3 operator/ (const Vector3 &l, realtype f);
		ZG_API friend Vector3 operator^ (const Vector3 &l, const Vector3 &r);
	#endif
		ZG_API friend Vector3 operator- (realtype f, const Vector3 &r);
		ZG_API friend Vector3 operator+ (realtype f, const Vector3 &r);
		ZG_API friend Vector3 operator* (realtype f, const Vector3 &l);
		ZG_API friend Vector3 operator/ (realtype f, const Vector3 &l);
		ZG_API friend std::ostream& operator<<(std::ostream& os, const Vector3& m);
	};

	ZG_API Vector3		ABS					(const Vector3& v_);
	ZG_API bool			ApproxEqual			(const Vector3& v0_, const Vector3& v1_, realtype epsilon);
	ZG_API realtype		Angle				(const Vector3& v0_, const Vector3& v1_);
	ZG_API Vector3		ClampMagnitude		(Vector3 vector, realtype maxLength);
	ZG_API void			CopyVector3Array	(Vector3* dest_, const Vector3* source_, size_t sz_);
	ZG_API void			CopyVector3Array	(std::vector<Vector3>& dest_, const Vector3* source_, size_t sz_);
	ZG_API Vector3		Cross				(const Vector3& from_, const Vector3& to_);
	ZG_API realtype		Distance			(const Vector3& v0_, const Vector3& v1_);	//distance of Vector 3 and Vector 3
	ZG_API realtype		DistanceSq			(const Vector3& v0_, const Vector3& v1_);	//distanceSq of Vector 3 and Vector 3
	ZG_API realtype		DistanceSq			(const Vector2& v0_, const Vector3& v1_);	//distanceSq of Vector 2 and Vector 3
	ZG_API realtype		DistanceSq			(const Vector3& v0_, const Vector2& v1_);	//distanceSq of Vector 3 and Vector 2
	ZG_API realtype		Dot					(const Vector3& v0_, const Vector3& v1_);
	ZG_API Vector3		FromSpherical		(realtype r, realtype theta, realtype phi);
	ZG_API bool			IsParallel			(const Vector3& v0_, const Vector3& v1_);	// Return true if two vectors are parallel
	ZG_API bool			IsOrthogonal		(const Vector3& v0_, const Vector3& v1_);	// Return true if two vectors are Orthogonal
	ZG_API realtype		Length				(const Vector3& v_);
	ZG_API realtype		LengthSq			(const Vector3& v_);
	ZG_API Vector3		Lerp				(const Vector3& start_, const Vector3& end_, float time_);
	ZG_API Vector3		nLerp				(const Vector3& start_, const Vector3& end_, float time_);
	ZG_API Vector3		LerpDelta			(const Vector3& start_, Vector3& deltaOf_end_start_, float time_);
	ZG_API Vector3		nLerpDelta			(const Vector3& start_, Vector3& deltaOf_end_start_, float time_);
	ZG_API Vector3		LinearRand			(const Vector3& v0_, const Vector3& v1_);
	ZG_API realtype		Magnitude			(const Vector3& v_);
	ZG_API realtype		MagnitudeSq			(const Vector3& v_);
	ZG_API Vector3		MoveTowards			(const Vector3& current, const Vector3& target, realtype maxDistanceDelta);
	ZG_API Vector3		Min					(const Vector3& v0_, const Vector3& v1_);
	ZG_API Vector3		Max					(const Vector3& v0_, const Vector3& v1_);
	ZG_API realtype		Normalize			(Vector3& v_);
	ZG_API Vector3		Normalized			(const Vector3& v_);
	ZG_API Vector3		Project				(const Vector3& v0_, const Vector3& v1_);
	ZG_API Vector3		ProjectOnPlane		(const Vector3& vector, const Vector3& planeNormal);
	ZG_API Vector3		Reflect				(const Vector3& vector, const Vector3& planeNormal);
	ZG_API Vector3		Reject				(const Vector3& a, const Vector3& b);
	ZG_API Vector3		Slerp				(Vector3 v0_, Vector3 v1_, realtype time_);
	ZG_API Vector3		SlerpClamp			(Vector3 v0_, Vector3 v1_, realtype time_);
	ZG_API void			ToSpherical			(const Vector3& vector, realtype &rad, realtype &theta, realtype &phi);
	ZG_API Vector3		Orthogonal			(const Vector3& v);

	
	//Computes scalar triple product
	ZG_API float	ScalarTripleProduct		(const Vector3& a, const Vector3& b, const Vector3& c);	
	//Barycentric Of Line, Triangle, Tetrahedron
	ZG_API bool		BarycentricCoordinates	(const Vector3& point, const Vector3& a, const Vector3& b, float& u, float& v, float epsilon);
	ZG_API bool		BarycentricCoordinates	(const Vector3& point, const Vector3& a, const Vector3& b, const Vector3& c, float& u, float& v, float& w, float epsilon);
	ZG_API bool		BarycentricCoordinates	(const Vector3& point, const Vector3& a, const Vector3& b, const Vector3& c, const Vector3& d, float& u, float& v, float& w, float& t, float epsilon = 0.0f);

	ZG_API Vector3	ComputeClosestPointOnSegment	(const Vector3& segPointA,	const Vector3& segPointB,	const Vector3& pointC);
	ZG_API float	ComputePlaneSegmentIntersection (const Vector3& segA,		const Vector3& segB,		realtype planeD, const Vector3& planeNormal);
	ZG_API float	ComputePointToLineDistance		(const Vector3& linePointA, const Vector3& linePointB,	const Vector3& point);

	//Generate Basis
	ZG_API void		GenerateOrthonormalBasis	(const zg::Vector3& w, zg::Vector3* u, zg::Vector3* v);

}	//namespace zg

#endif //_VECTOR3_H
