
#include "stdafx.h"
#include "Vector2.h"

namespace zg {
	//static
	const Vector2 Vector2::Zero{};
	const Vector2 Vector2::VectorX = Vector2{ realtype(1.0), realtype(0.0) };
	const Vector2 Vector2::VectorY = Vector2{ realtype(0.0), realtype(1.0) };

	Vector2 Vector2::Min(const Vector2& v1, const Vector2& v2) {
		return Vector2{
			MIN2(v1.x, v2.x),
			MIN2(v1.y, v2.y)
		};
	}
	Vector2 Vector2::Max(const Vector2& v1, const Vector2& v2) {
		return Vector2{
			MAX2(v1.x, v2.x),
			MAX2(v1.y, v2.y)
		};
	}

	Vector2::Vector2()  noexcept
		:x(0.0f), y(0.0f)
	{}
	Vector2::Vector2(realtype x_, realtype y_ )  noexcept
		:x(x_), y(y_) 
	{}
	Vector2::Vector2(const Vector3& v1_) noexcept
		:x(v1_.x), y(v1_.y)
	{}
	//setters
	realtype Vector2::Normalize() {
		realtype len = Length();
		len = len < MACHINE_EPSILON ? 1.0f : (1.0f / len);
		x *= len;
		y *= len;
		return len;
	}
	void Vector2::Set(realtype x_, realtype y_) {
		x = x_; y = y_;
	}
	void Vector2::SetZero() { x = y = realtype(0.0); }
	
	//getters
	realtype Vector2::Dot(const Vector2 &p1) const { return x * p1.x + y * p1.y; }
	Vector2 Vector2::GetAbsVector()	const {
		return Vector2{ 
			x < realtype(0.0) ? -x : x,
			y < realtype(0.0) ? -y : y
		};
	}
	int	Vector2::GetMinAxis() const {
		return (x < y) ? 0 : 1;
	}						  
	int	Vector2::GetMaxAxis() const {
		return (x < y) ? 1 : 0;
	}
	realtype Vector2::LengthSq()			  const { return x * x + y * y; }
	realtype Vector2::Length()				  const { return sqrtf(LengthSq()); }
	Vector2 Vector2::Normalized() const {
		realtype len = Length();
		len = len == 0.0f ? 1.0f : (1.0f / len);
		return Vector2{ x * len, y * len };
	}
	Vector2 Vector2::GetUnitOrthogonalVector()	const {
		realtype len = Length();
		len = len == 0.0f ? 1.0f : (1.0f / len);
		return Vector2(-y * len, x * len);
	}
	bool Vector2::IsUnit() const {
		return ApproxEqual(LengthSq(), 1.0f);
	}
	bool Vector2::IsZero() const {
		return ApproxEqual(LengthSq(), 0.0f);
	}
	

	
	
	//operator
	Vector2& Vector2::operator= (const Vector3 &v)		{ x =  v.x;  y = v.y;   return *this; }
	Vector2& Vector2::operator+=(const Vector2 &v)		{ x += v.x;  y += v.y;  return *this; }
	Vector2& Vector2::operator-=(const Vector2 &v)		{ x -= v.x;  y -= v.y;  return *this; }
	Vector2& Vector2::operator*=(realtype f)				{ x *= f;    y *= f;	return *this; }
	Vector2& Vector2::operator/=(realtype f)				{ x /= f;    y /= f;	return *this; }
	bool Vector2::operator==(const Vector2 &r) const	{ return ((x == r.x) && (y == r.y)); }
	bool Vector2::operator!=(const Vector2 &r) const	{ return !operator==(r); }
	
	realtype& Vector2::operator[] (int index)				{	return (&x)[index]; }
	const realtype& Vector2::operator[] (int index) const	{	return (&x)[index]; }
	
	#ifdef USING_SOL2
	Vector2 Vector2::operator-()				const	{ return Vector2{ -x,		-y		}; }	//negate
	Vector2 Vector2::operator-(const Vector2 &r)const	{ return Vector2{ x - r.x,	y - r.y }; }	//minus
	Vector2 Vector2::operator-(realtype f)			const	{ return Vector2{ x - f,	y - f	}; }	//minus
	Vector2 Vector2::operator+(const Vector2 &r)const	{ return Vector2{ x + r.x,	y + r.y }; }	//plus
	Vector2 Vector2::operator+(realtype f)			const	{ return Vector2{ x + f,	y + f	}; }	//plus
	Vector2 Vector2::operator*(const Vector2 &r)const	{ return Vector2{ x * r.x,	y * r.y }; }	//multiply
	Vector2 Vector2::operator*(realtype f)			const	{ return Vector2{ x * f,	y * f	}; }	//multiply
	Vector2 Vector2::operator/(realtype f)			const	{ return Vector2{ x / f,	y / f	}; }	//division
	
	// Returns the direction vector from pointA to point B
	Vector2 Vector2::getDirection(const Vector2& pointB) const{
		realtype oppOverAdj = (pointB.y - y) / (pointB.x - x);
		realtype angle = atanf(oppOverAdj);
	
		// Calculate direction vector
		Vector2 dir = { std::cos(angle), std::sin(angle) };
	
		// 90 deg = 1.5708 rad
		if ((angle <= 0.0f && angle > -1.5708f && pointB.x < x) || 
			(angle >= 0.0f && angle <  1.5708f && pointB.y < y)){
			dir.x *= -1;
			dir.y *= -1;
		}
		return dir;
	}
	void Vector2::BindLua(sol::state& lua)
	{
		lua.new_usertype<Vector2>("Vector2",
			sol::constructors<
			Vector2(),
			Vector2(realtype, realtype),
			Vector2(const Vector3&)
			>(),
			"x", &Vector2::x,
			"y", &Vector2::y,
			"SetToZero", &Vector2::SetToZero,
			"SetToOne", &Vector2::SetToOne,
			"Normalize", &Vector2::Normalize,
			"Normalized", &Vector2::Normalized,
			"Dot", &Vector2::Dot,
			"LengthSq", &Vector2::LengthSq,
			"Length", &Vector2::Length,
	
			sol::meta_function::addition, sol::overload(
				static_cast<Vector2(Vector2::*)(realtype) const>(&Vector2::operator+),
				static_cast<Vector2(Vector2::*)(const Vector2&) const>(&Vector2::operator+)
			),
	
			sol::meta_function::subtraction, sol::overload(
				static_cast<Vector2(Vector2::*)() const>(&Vector2::operator-),
				static_cast<Vector2(Vector2::*)(realtype) const>(&Vector2::operator-),
				static_cast<Vector2(Vector2::*)(const Vector2&) const>(&Vector2::operator-)
			),
	
			sol::meta_function::multiplication, sol::overload(
				static_cast<Vector2(Vector2::*)(realtype) const>(&Vector2::operator*),
				static_cast<Vector2(Vector2::*)(const Vector2&) const>(&Vector2::operator*)
			),
	
			sol::meta_function::division, &Vector2::operator/,
	
			"getDirection", &Vector2::getDirection
	
			);
	}
	#else	//friend
	Vector2 operator-(const Vector2 &l)						{ return Vector2{ -l.x,		-l.y		}; } //negate
	Vector2 operator-(const Vector2 &l, const Vector2 &r)	{ return Vector2{ l.x - r.x,l.y - r.y	}; } //minus
	Vector2 operator-(const Vector2 &l, realtype f)			{ return Vector2{ l.x - f,	l.y - f		}; } //minus
	Vector2 operator+(const Vector2 &l, const Vector2 &r)	{ return Vector2{ l.x + r.x,l.y + r.y	}; } //plus
	Vector2 operator+(const Vector2 &l, realtype f)			{ return Vector2{ l.x + f,	l.y + f		}; } //plus
	Vector2 operator*(const Vector2 &l, const Vector2 &r)	{ return Vector2{ l.x* r.x,	l.y * r.y	}; } //multiply
	Vector2 operator*(const Vector2 &v, realtype f)			{ return Vector2{ v.x* f,	v.y * f		}; } //multiply						
	Vector2 operator/(const Vector2 &v, realtype f)			{ return Vector2{ v.x / f,  v.y / f		}; } //division
	
	// Returns the direction vector from pointA to point B
	Vector2 getDirection(const Vector2& pointA, const Vector2& pointB)
	{
		realtype oppOverAdj = (pointB.y - pointA.y) / (pointB.x - pointA.x);
		realtype angle = atanf(oppOverAdj);
	
		// Calculate direction vector
		Vector2 dir = { std::cos(angle), std::sin(angle) };
	
		// 90 deg = 1.5708 rad
		if ((angle <= 0.0f && angle > -1.5708f && pointB.x < pointA.x)
		|| (angle >= 0.0f && angle <  1.5708f && pointB.y < pointA.y))
		{
			dir.x *= -1;
			dir.y *= -1;
		}
	
		return dir;
	}
	#endif
	
	//friend
	Vector2 operator-(realtype f, const Vector2 &r) { return Vector2{ f - r.x, f - r.y }; }
	Vector2 operator+(realtype f, const Vector2 &r) { return Vector2{ f + r.x, f + r.y }; }
	Vector2 operator*(realtype f, const Vector2 &r) { return Vector2{ f * r.x, f * r.y }; }
	std::ostream& operator<<(std::ostream& os, const Vector2& v) {
		printf("{ %.6f %.6f }\n", v.x, v.y);
		//os << " {" << v.x << "," << v.y << "} " << std::endl;
		return os;
	}

	//extern function
	Vector2 ABS(const Vector2 &v) {
		return Vector2{ 
			v.x < realtype(0.0) ? -v.x : v.x, 
			v.y < realtype(0.0) ? -v.y : v.y 
		};
	}
	bool ApproxEqual(const Vector2& vec1, const Vector2& vec2, realtype epsilon) {
		return ApproxEqual(vec1.x, vec2.x, epsilon)
			&& ApproxEqual(vec1.y, vec2.y, epsilon);
	}

	float DistanceSq(const Vector2& v0_, const Vector2& v1_) {
		return (v0_.x - v1_.x) * (v0_.x - v1_.x) +
			   (v0_.y - v1_.y) * (v0_.y - v1_.y);
	}

	Vector2 Direction(realtype y_dir, realtype x_dir) {
		return Normalized(Vector2{ std::cos(y_dir), std::sin(x_dir) });
	}

	realtype	Dot			(const Vector2& v0_, const Vector2& v1_) { return v0_.Dot(v1_);		}
	float		Length		(const Vector2& v_)						 { return v_.Length();		}
	float		LengthSq	(const Vector2& v_)						 { return v_.LengthSq();	}

	Vector2 Lerp(const Vector2& start_, const Vector2& end_, float time_) {
		// Imprecise method, which does not guarantee v = v1 when t = 1, due to floating-point arithmetic error.
		// This form may be used when the hardware has a native fused multiply-add instruction.
		//return start_ + realtype(time_) * (end_ - start_);

		// Precise method, which guarantees v = v1 when t = 1.
		Vector2 v0 = (realtype(1) - realtype(time_)) * start_;
		Vector2 v1 = realtype(time_) * end_;
		return v0 + v1;
	}
	Vector2 nLerp(const Vector2& start_, const Vector2& end_, float time_) {
		return Lerp(start_, end_, time_).Normalized();
	}

	Vector2 LinearRand(const Vector2& v0_, const Vector2& v1_) {
		return Vector2(sfrand(v0_.x, v1_.x), sfrand(v0_.y, v1_.y));
	}

	realtype	Magnitude	(const Vector2& v_)	{ return Length(v_);		}
	realtype	MagnitudeSq	(const Vector2& v_)	{ return LengthSq(v_);		}
	realtype	Normalize	(Vector2& v)		{ return v.Normalize();		}
	Vector2		Normalized	(const Vector2& v)	{ return v.Normalized();	}

}
