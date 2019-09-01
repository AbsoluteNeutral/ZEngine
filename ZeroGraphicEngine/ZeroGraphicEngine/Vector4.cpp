/*****************************************************************************/
/*!
\file			Vector4.cpp
\brief			Vector4 Implmentation
*/
/*****************************************************************************/
#include "stdafx.h"
#include "Vector4.h"

namespace zg {
	//static
	const Vector4 Vector4::Zero{};

	Vector4::Vector4() noexcept
		:x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
	Vector4::Vector4(realtype x_, realtype y_, realtype z_, realtype w_) noexcept
		:x(x_), y(y_), z(z_), w(w_) {}
	Vector4::Vector4(const Vector3& v, realtype w_) noexcept
		: x(v.x), y(v.y), z(v.z), w(w_) {}
	Vector4::Vector4(const Vector2 &v) noexcept
		: x(v.x), y(v.y), z(0.0f), w(0.0f) {}
	Vector4::Vector4(const Vector3 &v) noexcept
		: x(v.x), y(v.y), z(v.z), w(0.0f) {}
	
	//setters
	realtype Vector4::Normalize() {
		realtype len = Length();
		if (len <= 0.0f) return len;
		realtype overlen = (1.0f / len);
		x *= overlen;
		y *= overlen;
		z *= overlen;
		w *= overlen;
		return len;
	}
	void Vector4::SetZero() { x = y = z = w = 0.0f; }
	void Vector4::SetOne()	{ x = y = z = w = 1.0f; }

	//getters
	Vector4 Vector4::Normalized() const {
		realtype len = Length();
		len = len < MACHINE_EPSILON ? 1.0f : (1.0f / len);
		return Vector4{ x * len, y * len, z * len, w * len };
	}
	realtype Vector4::Dot(const Vector4& v_) const { return x * v_.x + y * v_.y + z * v_.z + w * v_.w; }
	realtype Vector4::LengthSq()			 const { return x * x + y * y + z * z + +w * w; }
	realtype Vector4::Length()				 const { return sqrtf(LengthSq()); }
	
	bool Vector4::IsUnit() const {
		return ApproxEqual(LengthSq(), 1.0f);
	}
	bool Vector4::IsZero() const {
		return ApproxEqual(LengthSq(), 0.0f);
	}
	
	
	Vector4 Vector4::Min(const Vector4& v0_, const Vector4& v1_) {
		return Vector4{
			MIN2(v0_.x, v1_.x),
			MIN2(v0_.y, v1_.y),
			MIN2(v0_.z, v1_.z),
			MIN2(v0_.w, v1_.w)
		};
	}
	Vector4 Vector4::Max(const Vector4& v0_, const Vector4& v1_) {
		return Vector4{
			MAX2(v0_.x, v1_.x),
			MAX2(v0_.y, v1_.y),
			MAX2(v0_.z, v1_.z),
			MAX2(v0_.w, v1_.w)
		};
	}
	
	//operator
	realtype& Vector4::operator[]	(int index)		    { return (&x)[index]; }
	realtype  Vector4::operator[]	(int index) const   { return (&x)[index]; }
	Vector4&  Vector4::operator=	(const Vector2& v_) { x = v_.x;		y = v_.y;							return *this; }
	Vector4&  Vector4::operator=	(const Vector3& v_) { x = v_.x;		y = v_.y;	z = v_.z;				return *this; }
	Vector4&  Vector4::operator+=	(const Vector4& v_) { x += v_.x;	y += v_.y;	z += v_.z;	w += v_.w;	return *this; }
	Vector4&  Vector4::operator-=	(const Vector4& v_) { x -= v_.x;	y -= v_.y;	z -= v_.z;	w -= v_.w;	return *this; }
	Vector4&  Vector4::operator*=	(const Vector4& v_) { x *= v_.x;	y *= v_.y;	z *= v_.z;	w *= v_.w;	return *this; }
	Vector4&  Vector4::operator*=	(realtype f)	    { x *= f;		y *= f;		z *= f;		w *= f;		return *this; }
	Vector4&  Vector4::operator/=	(realtype f)	    { 
		realtype d = 1.0f / f;
		x *= d;	y *= d;	z *= d; w *= d;
		return *this;
	}
	bool Vector4::operator==(const Vector4& v_) const { return ((x == v_.x) && (y == v_.y) && (z == v_.z) && (w == v_.w)); }
	
	#ifdef USING_SOL2
	Vector4 Vector4::operator-()				const	{ return Vector4{ -x,		-y,			-z,			-w		}; } //negate
	Vector4 Vector4::operator-(const Vector4 &r)const	{ return Vector4{ x - r.x,	y - r.y,	z - r.z,	w - r.w	}; } //minus
	Vector4 Vector4::operator-(realtype f)			const	{ return Vector4{ x - f,	y - f,		z - f,		w - f	}; } //minus
	Vector4 Vector4::operator+(const Vector4 &r)const	{ return Vector4{ x + r.x,  y + r.y,	z + r.z,	w + r.w	}; } //plus
	Vector4 Vector4::operator+(realtype f)			const	{ return Vector4{ x + f,    y + f,		z + f,		w + f	}; } //plus
	Vector4 Vector4::operator*(const Vector4 &r)const	{ return Vector4{ x * r.x ,	y*r.y,		z*r.z,		w * r.w }; } //multiply
	Vector4 Vector4::operator*(realtype f)			const	{ return Vector4{ x * f,	y*f,		z*f,		w * f	}; } //multiply
	Vector4 Vector4::operator/(realtype f)			const	{ 
		realtype d = 1.0f / f;
		return Vector4{ x*d, y*d, z*d, w*d };
	} //division
	void Vector4::BindLua(sol::state & lua)
	{
		lua.new_usertype<Vector4>("Vector4",
			sol::constructors<
			Vector4(),
			Vector4(realtype, realtype, realtype, realtype),
			Vector4(const Vector2&),
			Vector4(const Vector3&)>(),
			"x", &Vector4::x,
			"y", &Vector4::y,
			"z", &Vector4::z,
			"w", &Vector4::w,
			"zero", &Vector4::zero,
			"one", &Vector4::one,
			"Normalize", &Vector4::Normalize,
			"Normalized", &Vector4::Normalized,
			"Dot", &Vector4::Dot,
			"LengthSq", &Vector4::LengthSq,
			"Length", &Vector4::Length,
	
			sol::meta_function::addition, sol::overload(
				static_cast<Vector4(Vector4::*)(realtype) const>(&Vector4::operator+),
				static_cast<Vector4(Vector4::*)(const Vector4&) const>(&Vector4::operator+)
			),
	
			sol::meta_function::subtraction, sol::overload(
				static_cast<Vector4(Vector4::*)() const>(&Vector4::operator-),
				static_cast<Vector4(Vector4::*)(realtype) const>(&Vector4::operator-),
				static_cast<Vector4(Vector4::*)(const Vector4&) const>(&Vector4::operator-)
			),
	
			sol::meta_function::multiplication, sol::overload(
				static_cast<Vector4(Vector4::*)(realtype) const>(&Vector4::operator*),
				static_cast<Vector4(Vector4::*)(const Vector4&) const>(&Vector4::operator*)
			),
	
			sol::meta_function::division, &Vector4::operator/
	
			);
	}
	#else
	Vector4 operator-(const Vector4 &l)							{ return Vector4{ -l.x,		-l.y,		-l.z,		-l.w		}; } //negate
	Vector4 operator-(const Vector4 &l, const Vector4 &r)		{ return Vector4{ l.x - r.x,l.y - r.y,	l.z - r.z,	l.w - r.w	}; } //minus
	Vector4 operator-(const Vector4 &l, realtype f)				{ return Vector4{ l.x - f,	l.y - f,	l.z - f,	l.w - f		}; } //minus
	Vector4 operator+(const Vector4 &l, const Vector4 &r)		{ return Vector4{ l.x + r.x,l.y + r.y,	l.z + r.z,	l.w + r.w	}; } //plus
	Vector4 operator+(const Vector4 &l, realtype f)				{ return Vector4{ l.x + f,	l.y + f,	l.z + f,	l.w + f		}; } //plus
	Vector4 operator*(const Vector4 &l, const Vector4 &r)		{ return Vector4{ l.x*r.x ,	l.y*r.y,	l.z*r.z,	l.w*r.w		}; } //multiply
	Vector4 operator*(const Vector4 &l, realtype f)				{ return Vector4{ l.x*f,	l.y*f,		l.z*f,		l.w*f		}; } //multiply
	Vector4 operator/(const Vector4 &l, realtype f)				{ return Vector4{ l.x / f,	l.y / f,	l.z / f,	l.w / f		}; } //division
	#endif
	Vector4 operator-(realtype f, const Vector4 &r) { return Vector4{ f - r.x, f - r.y, f - r.z, f - r.w }; }
	Vector4 operator+(realtype f, const Vector4 &r) { return Vector4{ f + r.x, f + r.y, f + r.z, f + r.w }; }
	Vector4 operator*(realtype f, const Vector4 &r) { return Vector4{ f * r.x, f * r.y, f * r.z, f * r.w }; }
	std::ostream& operator<<(std::ostream& os, const Vector4& v){
		printf("{ %.6f %.6f %.6f %.6f }\n", v.x, v.y, v.z, v.w);
		//os << "{" << v.x << "," << v.y << "," << v.z << "," << v.w << "}\n";
		return os;
	}

	Vector4 ABS(const Vector4 &v) {
		return Vector4{ v.x < 0.0f ? -v.x : v.x, 
						v.y < 0.0f ? -v.y : v.y,
						v.z < 0.0f ? -v.z : v.z, 
						v.w < 0.0f ? -v.w : v.w };
	}

	bool ApproxEqual(const Vector4& v0_, const Vector4& v1_, realtype epsilon) {
		return ApproxEqual(v0_.x, v1_.x, epsilon)
			&& ApproxEqual(v0_.y, v1_.y, epsilon)
			&& ApproxEqual(v0_.z, v1_.z, epsilon)
			&& ApproxEqual(v0_.w, v1_.w, epsilon);
	}

	float DistanceSq(const Vector4& v0_, const Vector4& v1_) {
		return
			(v0_.x - v1_.x) * (v0_.x - v1_.x) +
			(v0_.y - v1_.y) * (v0_.y - v1_.y) +
			(v0_.z - v1_.z) * (v0_.z - v1_.z) +
			(v0_.w - v1_.w) * (v0_.w - v1_.w);
	}

	realtype	Dot			(const Vector4& v0_, const Vector4& v1_) { return v0_.Dot(v1_);		}
	float		Length		(const Vector4& v_)						 { return v_.Length();		}
	float		LengthSq	(const Vector4& v_)						 { return v_.LengthSq();	}

	Vector4 Lerp(const Vector4& start_, const Vector4& end_, float time_) {
		// Imprecise method, which does not guarantee v = v1 when t = 1, due to floating-point arithmetic error.
		// This form may be used when the hardware has a native fused multiply-add instruction.
		//return start_ + realtype(time_) * (end_ - start_);

		// Precise method, which guarantees v = v1 when t = 1.
		Vector4 v0 = (realtype(1) - realtype(time_)) * start_;
		Vector4 v1 = realtype(time_) * end_;
		return v0 + v1;
	}
	Vector4 nLerp(const Vector4& start_, const Vector4& end_, float time_) {
		return Lerp(start_, end_, time_).Normalized();
	}

	Vector4 LinearRand(const Vector4& v0_, const Vector4& v1_) {
		return Vector4(sfrand(v0_.x, v1_.x), sfrand(v0_.y, v1_.y), sfrand(v0_.z, v1_.z), sfrand(v0_.w, v1_.w));
	}

	realtype	Magnitude	(const Vector4& v_)	{ return Length(v_);		}
	realtype	MagnitudeSq	(const Vector4& v_)	{ return LengthSq(v_);		}
	realtype	Normalize	(Vector4& v)		{ return v.Normalize();		}
	Vector4		Normalized	(const Vector4& v)	{ return v.Normalized();	}

}	//namespace zg
