
#include "stdafx.h"
#include <stdio.h>
#include "Quaternion.h"

namespace zg {

	Quaternion::Quaternion() noexcept
		:x(0.0f), y(0.0f), z(0.0f), w(1.0f)
	{}
	Quaternion::Quaternion(const Vector3& topurequaternion_) noexcept
		:x(topurequaternion_.x), y(topurequaternion_.y), z(topurequaternion_.z), w(0.0f)
	{}
	Quaternion::Quaternion(float x_, float y_, float z_, float w_) noexcept
		: x(x_), y(y_), z(z_), w(w_)
	{}
	Quaternion::Quaternion(const Vector3& axis_, float degree_) noexcept
		:x(0.0f), y(0.0f), z(0.0f), w(1.0f)
	{
		float rad = TO_RAD(degree_) * 0.5f;
		float s = std::sin(rad);
		x = axis_.x * s;
		y = axis_.y * s;
		z = axis_.z * s;
		w = std::cos(rad);
	}
	
	// ____________________________________________________________ setters
	void Quaternion::AddScaledVector(const Vector3& vector_, float f_) {
		Quaternion q(vector_.x * f_, vector_.y * f_, vector_.z * f_, 0.0f);
		q *= *this;
		x += q.x * 0.5f;
		y += q.y * 0.5f;
		z += q.z * 0.5f;
		w += q.w * 0.5f;
	}
	
	void Quaternion::Conjugate(){
		x = -x;	y = -y;	z = -z;
	}

	void Quaternion::Integrate(const Vector3& vector_, float f_) {
		AddScaledVector(vector_, f_);
		//Quaternion q(vector_.x * f_, vector_.y * f_, vector_.z * f_, 0.0f);
		//q *= *this;
		//x += q.x * 0.5f;
		//y += q.y * 0.5f;
		//z += q.z * 0.5f;
		//w += q.w * 0.5f;
	}
	void Quaternion::Invert() {
		float lengthSq = LengthSq();
		//ErrorIf(lengthSq <= EPSILON, "Quaternion - Division by zero.");
		lengthSq = 1.0f / lengthSq;
		x = -x * lengthSq;
		y = -y * lengthSq;
		z = -z * lengthSq;
		w =  w * lengthSq;
	}
	
	float Quaternion::Normalize() {
		float len = LengthSq();
		if (len == 0.0f) {
			x = y = z = 0.0f;
			w = 1.0f;
			return 0.0f;
		}
		len = 1.0f / sqrtf(len);
		x *= len; y *= len;
		z *= len; w *= len;
		return len;
	}
	
	void Quaternion::Set(float x_, float y_, float z_, float w_) {
		x = x_;	y = y_;	z = z_;	w = w_;
	}
	
	void Quaternion::SetFromEulerAngles(float RADIAN_x, float RADIAN_y, float RADIAN_z) {
		const float halfx = RADIAN_x * 0.5f;
		const float halfy = RADIAN_y * 0.5f;
		const float halfz = RADIAN_z * 0.5f;
		const Vector3 c{ std::cos(halfx), std::cos(halfy), std::cos(halfz) };
		const Vector3 s{ std::sin(halfx), std::sin(halfy), std::sin(halfz) };
		const float cosYcosZ = c.y * c.z;
		const float cosYsinZ = c.y * s.z;
		const float sinYsinZ = s.y * s.z;
		const float sinYcosZ = s.y * c.z;
		x = s.x * cosYcosZ - c.x * sinYsinZ;
		y = c.x * sinYcosZ + s.x * cosYsinZ;
		z = c.x * cosYsinZ - s.x * sinYcosZ;
		w = c.x * cosYcosZ + s.x * sinYsinZ;

		x = NEARZERO(x) ? 0.0f : x;
		y = NEARZERO(y) ? 0.0f : y;
		z = NEARZERO(z) ? 0.0f : z;
		w = NEARZERO(w) ? 1.0f : w;
	}
	
	void Quaternion::SetFromEulerAngles(const Vector3& degree_) {
		float x_ = degree_.x;	//v.x, 
		float y_ = degree_.y;	//v.y, 
		float z_ = degree_.z;	//v.z  
		SetFromEulerAngles(TO_RAD(x_), TO_RAD(y_), TO_RAD(z_));
	}
	
	void Quaternion::Reset() {
		x = y = z = 0.0f;
		w = 1.0f;
	}
	// ____________________________________________________________ getters 
	float Quaternion::Dot(const Quaternion& q_) const {
		return x * q_.x + y * q_.y + z * q_.z + w * q_.w;
	}
	
	Vector3 Quaternion::GetAxis() const {
		float angle_rad = 1.0f / sin(acos(x));
		return Vector3{
			x * angle_rad,
			y * angle_rad,
			z * angle_rad
		};
	}
	
	Quaternion Quaternion::GetRotationAngleAxis(float& angle, Vector3& axis) const {
		//angle = std::acos(w) * 0.5f;	// Compute the roation angle
		//Vector3 rotationAxis(x, y, z);	// Compute the 3D rotation axis
		//rotationAxis.Normalize();		// Normalize the rotation axis
		//// Set the rotation axis values
		//axis.x = rotationAxis.x;
		//axis.y = rotationAxis.y;
		//axis.z = rotationAxis.z;
	
		float halfAngle = angle * 0.5f;
		float s = std::sin(halfAngle);
		Quaternion q;
		q.x = axis.x * s;
		q.y = axis.y * s;
		q.z = axis.z * s;
		q.w = std::cos(halfAngle);
		return q;
	}
	
	Quaternion Quaternion::GetConjugated() const{
		return Quaternion{ -x, -y, -z, w };
	}
	
	Quaternion Quaternion::GetExponent() const {
		float angle = Vector3{ x, y, z }.Length();
		Quaternion quaternion(x, y, z, 0.0f);
		if (fabsf(angle) > EPSILON) {
			quaternion.w = std::cos(angle);
			angle = std::sin(angle) / angle;
			quaternion.x *= angle;
			quaternion.y *= angle;
			quaternion.z *= angle;
		}
		return quaternion;
	}
	
	Quaternion Quaternion::GetInverted() const {
		float lengthSq = LengthSq();
		//ErrorIf(lengthSq <= EPSILON, "Quaternion - Division by zero.");
		lengthSq = 1.0f / lengthSq;
		return Quaternion(-x * lengthSq, -y * lengthSq, -z * lengthSq, w * lengthSq);
	}
	
	Matrix33 Quaternion::GetMatrix33() const {
		float nQ  = x * x + y * y + z * z + w * w;
		float s   = nQ > 0.0f ? 2.0f / nQ : 0.0f;
		float xs  = x * s;
		float ys  = y * s;
		float zs  = z * s;
		float wxs = w * xs;
		float wys = w * ys;
		float wzs = w * zs;
		float xxs = x * xs;
		float xys = x * ys;
		float xzs = x * zs;
		float yys = y * ys;
		float yzs = y * zs;
		float zzs = z * zs;
		return Matrix33{ 1.0f - yys - zzs,	xys - wzs,			xzs + wys,
						 xys + wzs,			1.0f - xxs - zzs,	yzs - wxs,
						 xzs - wys,			yzs + wxs,			1.0f - xxs - yys };
	}
	
	Matrix44 Quaternion::GetMatrix44() const {
		float nQ = x * x + y * y + z * z + w * w;
		float s = nQ > 0.0f ? 2.0f / nQ : 0.0f;
		float xs =  x * s;
		float ys =  y * s;
		float zs =  z * s;
		float wxs = w * xs;
		float wys = w * ys;
		float wzs = w * zs;
		float xxs = x * xs;
		float xys = x * ys;
		float xzs = x * zs;
		float yys = y * ys;
		float yzs = y * zs;
		float zzs = z * zs;
		return Matrix44{
			1.0f - yys - zzs,	xys - wzs,			xzs + wys,			0.0f,
			xys + wzs,			1.0f - xxs - zzs,	yzs - wxs,			0.0f,
			xzs - wys,			yzs + wxs,			1.0f - xxs - yys,	0.0f,
			0.0f,				0.0f,				0.0f,				1.0f	
		};
	}
	
	Quaternion Quaternion::Normalized() const {
		float len = LengthSq();
		if (len == 0.0f)
			return Quaternion{};
		len = 1.0f / sqrtf(len);
		return Quaternion{ x * len, y * len, z * len, w * len };
	}
	
	Vector3 Quaternion::GetRotatedVector(const Vector3& vector_) const {
		return operator*(*this, vector_);
	}
	
	Vector3	Quaternion::GetVector3() const {
		return Vector3{ x, y, z };
	}
	
	float Quaternion::LengthSq() const { return x * x + y * y + z * z + w * w; }
	float Quaternion::Length()	 const { return sqrtf(x*x + y * y + z * z + w * w); }
	Vector3 Quaternion::ToEularDegree() const {
		float sqw = w * w;
		float sqx = x * x;
		float sqy = y * y;
		float sqz = z * z;
		float z_ = TO_DEG(atan2(2.0f * (x * y + z * w), (sqx - sqy - sqz + sqw)));  //z
		float x_ = TO_DEG(atan2(2.0f * (y * z + x * w), (-sqx - sqy + sqz + sqw))); //x
		float y_ = TO_DEG(asin(-2.0f * (x * z - y * w)));						   //y
		//z_ = (z_ > -EPSILON && z_ < EPSILON) ? 0.0f : z_;
		//x_ = (x_ > -EPSILON && x_ < EPSILON) ? 0.0f : x_;
		//y_ = (y_ > -EPSILON && y_ < EPSILON) ? 0.0f : y_;
		return Vector3{ x_, y_, z_ };
	}
	
	bool Quaternion::IsNan() const
	{
		return (x != x) || (y != y) || (z != z) || (w != w);
	}
	bool Quaternion::IsNearNan() const
	{
		return (NEARZERO(x) && NEARZERO(y) && NEARZERO(z) && NEARZERO(w));
	}
	// ____________________________________________________________ static
	Quaternion Quaternion::Identity{};

	// ____________________________________________________________ operator
	bool Quaternion::operator==(const Quaternion& q2_) const { return ((x == q2_.x) && (y == q2_.y) && (z == q2_.z) && (w == q2_.w)); }

	Quaternion& Quaternion::operator-=(const Quaternion& q2_){ x -= q2_.x;	y -= q2_.y;	z -= q2_.z;	w -= q2_.w;	return *this; }
	Quaternion& Quaternion::operator+=(const Quaternion& q2_){ x += q2_.x;	y += q2_.y; z += q2_.z; w += q2_.w; return *this; }
	Quaternion& Quaternion::operator*=(float f_)			 { x *= f_;		y *= f_;	z *= f_;	w *= f_;	return *this; }
	Quaternion& Quaternion::operator*=(const Quaternion& q2_){
		//Vector3 v1{ x, y, z };
		//Vector3 v2{ q2_.x, q2_.y, q2_.z };
		//Vector3 v3 = w * v2 + q2_.w * v1 + v1.Cross(v2);
		//x = v3.x;
		//y = v3.y;
		//z = v3.z;
		//w = w * q2_.w - v1.Dot(v2);
		Quaternion lhs{ *this };
		w = lhs.w * q2_.w - lhs.x * q2_.x - lhs.y * q2_.y - lhs.z * q2_.z;
		x = lhs.w * q2_.x + lhs.x * q2_.w + lhs.y * q2_.z - lhs.z * q2_.y;
		y = lhs.w * q2_.y - lhs.x * q2_.z + lhs.y * q2_.w + lhs.z * q2_.x;
		z = lhs.w * q2_.z + lhs.x * q2_.y - lhs.y * q2_.x + lhs.z * q2_.w;

		//if (NEARZERO(x) && NEARZERO(y) && NEARZERO(z) && NEARZERO(w))
		//{
		//	*this = lhs.GetConjugated();
		//}
		return *this;
	}
	Quaternion& Quaternion::operator/=(float f_) {
		ErrorIf(f_ <= EPSILON, "Quaternion - Division by zero.");
		float reciprocal = 1.0f / f_;
		x *= reciprocal;
		y *= reciprocal;
		z *= reciprocal;
		w *= reciprocal;
		return *this;
	}

	#ifdef USING_SOL2
	Quaternion Quaternion::operator-()						const	{ return Quaternion{ -x, -y, -z, w }; }
	Quaternion Quaternion::operator-(const Quaternion& q2_) const	{ return Quaternion{ x - q2_.x,	y - q2_.y,	z - q2_.z,	w - q2_.w }; }
	Quaternion Quaternion::operator+(const Quaternion& q2_) const	{ return Quaternion{ x + q2_.x,	y + q2_.y,	z + q2_.z,	w + q2_.w }; }
	Quaternion Quaternion::operator*(float f)			    const	{ return Quaternion{ x * f,		y * f,		z * f,		w * f	  }; }
	Vector3 Quaternion::operator*(const Vector3& q2_)	const {
		const float prodX = w * q2_.x + y * q2_.z - z * q2_.y;
	    const float prodY = w * q2_.y + z * q2_.x - x * q2_.z;
	    const float prodZ = w * q2_.z + x * q2_.y - y * q2_.x;
	    const float prodW = -x * q2_.x - y * q2_.y - z * q2_.z;
	    return Vector3(w * prodX - prodY * z + prodZ * y - prodW * x,
	                   w * prodY - prodZ * x + prodX * z - prodW * y,
	                   w * prodZ - prodX * y + prodY * x - prodW * z);
	}
	Quaternion Quaternion::operator*(const Quaternion& q2_) const {
		return Quaternion{
			w * q2_.x + x * q2_.w + y * q2_.z - z * q2_.y,
			w * q2_.y + y * q2_.w + z * q2_.x - x * q2_.z,
			w * q2_.z + z * q2_.w + x * q2_.y - y * q2_.x,
			w * q2_.w - x * q2_.x - y * q2_.y - z * q2_.z
		};
	}
	Quaternion Quaternion::operator/(float q2_) const {
		ErrorIf(q2_ <= EPSILON, "Quaternion - Division by zero.");
		float reciprocal = 1.0f / q2_;
		return Quaternion{
			x * reciprocal,
			y * reciprocal,
			z * reciprocal,
			w * reciprocal
		};
	}
	void Quaternion::BindLua(sol::state & lua)
	{
		lua.new_usertype<Quaternion>("Quaternion",
			sol::constructors<
			Quaternion(),
			Quaternion(float, float, float, float),
			Quaternion(const Vector3&, float)>(),
	
			"x", &Quaternion::x,
			"y", &Quaternion::y,
			"z", &Quaternion::z,
			"w", &Quaternion::w,
	
			"SetToZero", &Quaternion::SetToZero,
			"Normalize", &Quaternion::Normalize,
			"Normalized", &Quaternion::Normalized,
			"Dot", &Quaternion::Dot,
			"LengthSq", &Quaternion::LengthSq,
			"Length", &Quaternion::Length,
			"Conjugate", &Quaternion::Conjugate,
			"GetConjugated", &Quaternion::GetConjugated,
			"Invert", &Quaternion::Invert,
			"GetInverted", &Quaternion::GetInverted,
			"GetExponent", &Quaternion::GetExponent,
			"GetAxis", &Quaternion::GetAxis,
	
			"EulerToQ", sol::overload(
				static_cast<void (Quaternion::*)(float, float, float)>(&Quaternion::ToQuaternion),
				static_cast<void (Quaternion::*)(const Vector3&)>(&Quaternion::ToQuaternion)),
	
			"Degree", &Quaternion::Degree,
			"GetRotatedVector", &Quaternion::GetRotatedVector,
			"Integrate", &Quaternion::Integrate,
			"AddScaledVector", &Quaternion::AddScaledVector,
			sol::meta_function::addition, &Quaternion::operator+,
			sol::meta_function::subtraction, 
	      static_cast<Quaternion(Quaternion::*)(const Quaternion&) const>(&Quaternion::operator-),
			sol::meta_function::multiplication, sol::overload(
				static_cast<Quaternion(Quaternion::*)(float) const>(&Quaternion::operator*),
				static_cast<Quaternion(Quaternion::*)(const Quaternion&) const>(&Quaternion::operator*)
	
			),
			sol::meta_function::division, &Quaternion::operator/,
	    sol::meta_function::unary_minus, 
	      static_cast<Quaternion(Quaternion::*)()const>(&Quaternion::operator-)
			);
	
	
		lua["Quaternion_Identity"] = Identity;
		lua["Quaternion_Slerp"] = Slerp;
		lua["Quaternion_FromToRotation"] = FromToRotation;
		lua["Quaternion_LookRotation"] = LookRotation;
	}
	#else
	Quaternion operator-(const Quaternion& q1_)						   { return Quaternion{ -q1_.x, -q1_.y, -q1_.z, q1_.w }; }
	Quaternion operator-(const Quaternion& q1_, const Quaternion& q2_) { return Quaternion{ q1_.x - q2_.x,	q1_.y - q2_.y,	q1_.z - q2_.z,	q1_.w - q2_.w }; }
	Quaternion operator+(const Quaternion& q1_, const Quaternion& q2_) { return Quaternion{ q1_.x + q2_.x,	q1_.y + q2_.y,	q1_.z + q2_.z,	q1_.w + q2_.w }; }
	Vector3	operator*(const Vector3& v_, const Quaternion& q1_)		   { return operator*(q1_, v_); }
	Vector3 operator*(const Quaternion& q1_, const Vector3& v_) {
		//#version 1
		//Quaternion tempVec{ vector.x, vector.y, vector.z, 0.0f };
		//Quaternion result(*this);
		//result *= tempVec;
		//result *= GetConjugated();
		//return Vector3{ result.x, result.y, result.z };

		//#version 2
		const float prodX = q1_.w * v_.x + q1_.y * v_.z - q1_.z * v_.y;
		const float prodY = q1_.w * v_.y + q1_.z * v_.x - q1_.x * v_.z;
		const float prodZ = q1_.w * v_.z + q1_.x * v_.y - q1_.y * v_.x;
		const float prodW = -q1_.x * v_.x - q1_.y * v_.y - q1_.z * v_.z;
		return Vector3(q1_.w * prodX - prodY * q1_.z + prodZ * q1_.y - prodW * q1_.x,
					   q1_.w * prodY - prodZ * q1_.x + prodX * q1_.z - prodW * q1_.y,
					   q1_.w * prodZ - prodX * q1_.y + prodY * q1_.x - prodW * q1_.z);
	}
	Quaternion operator*(const Quaternion& q1_, float f_)	{ return Quaternion{ q1_.x * f_,	q1_.y * f_,	q1_.z * f_,	q1_.w * f_ }; }
	Quaternion operator*(float f_, const Quaternion& q1_)	{ return operator*(q1_, f_); }
	Quaternion operator*(const Quaternion& q1_, const Quaternion& q2_) {
		//#version 1
		Quaternion tmp{ q1_ };
		tmp *= q2_;
		return tmp;
		//#version 2
		//return Quaternion{
		//	q1_.w * q2_.x + q1_.x * q2_.w + q1_.y * q2_.z - q1_.z * q2_.y,
		//	q1_.w * q2_.y - q1_.x * q2_.z + q1_.y * q2_.w + q1_.z * q2_.x,
		//	q1_.w * q2_.z + q1_.x * q2_.y - q1_.y * q2_.x + q1_.z * q2_.w,
		//	q1_.w * q2_.w - q1_.x * q2_.x - q1_.y * q2_.y - q1_.z * q2_.z
		//};
	}

	Quaternion operator/(const Quaternion& q1_, float f_) {
		float reciprocal = 1.0f / f_;
		return Quaternion{
			q1_.x * reciprocal,
			q1_.y * reciprocal,
			q1_.z * reciprocal,
			q1_.w * reciprocal
		};
	}
	#endif
	
	std::ostream& operator<<(std::ostream& os, const Quaternion& q_) {
		printf("{ %.6f %.6f %.6f %.6f }\n", q_.x, q_.y, q_.z, q_.w);
		//os << "{" << v.x << "," << v.y << "," << v.z << "," << v.w << "}\n";
		return os;
	}
	float Dot(const Quaternion& q0_, const Quaternion& q1_) {
		return q0_.Dot(q1_);
	}
	Vector3 ToEularDegree(const Quaternion& q_) {
		return q_.ToEularDegree();
	}

	Quaternion ToQuaternion(float RADIAN_x, float RADIAN_y, float RADIAN_z) {
		Quaternion q;
		q.SetFromEulerAngles(RADIAN_x, RADIAN_y, RADIAN_z);
		return q;
	}

	Quaternion ToQuaternion(const Vector3& degree_) {
		Quaternion q;
		q.SetFromEulerAngles(degree_);
		return q;
	}



	Quaternion FromToRotation(const Vector3& fromVector, const Vector3& toVector) {
		float dot = fromVector.Dot(toVector);
		float k = sqrt(fromVector.LengthSq() * toVector.LengthSq());
		if (fabs(dot / k + 1) < EPSILON)
		{
			Vector3 ortho = zg::Orthogonal(fromVector);
			return Quaternion(ortho.Normalized(), 0);
		}
		Vector3 cross = fromVector.Cross(toVector);
		return Quaternion(cross, dot + k).Normalized();
	}

	Quaternion LookRotation(const Vector3& forward, const Vector3& up, const Vector3& front) {
		UNREFERENCED_PARAMETER(front);
		Vector3 vector = forward.Normalized();
		Vector3 vector2 = Cross(up, vector);
		Vector3 vector3 = Cross(vector, vector2);
		float m00 = vector2.x;
		float m01 = vector2.y;
		float m02 = vector2.z;
		float m10 = vector3.x;
		float m11 = vector3.y;
		float m12 = vector3.z;
		float m20 = vector.x;
		float m21 = vector.y;
		float m22 = vector.z;

		float num8 = (m00 + m11) + m22;
		Quaternion quaternion{};// = FQuat();
		if (num8 > 0.0f)
		{
			float num = (float)std::sqrt(num8 + 1.0f);
			quaternion.w = num * 0.5f;
			num = 0.5f / num;
			quaternion.x = (m12 - m21) * num;
			quaternion.y = (m20 - m02) * num;
			quaternion.z = (m01 - m10) * num;

			return quaternion;
		}
		if ((m00 >= m11) && (m00 >= m22))
		{
			float num7 = (float)std::sqrt(((1.0f + m00) - m11) - m22);
			float num4 = 0.5f / num7;
			quaternion.x = 0.5f * num7;
			quaternion.y = (m01 + m10) * num4;
			quaternion.z = (m02 + m20) * num4;
			quaternion.w = (m12 - m21) * num4;

			return quaternion;
		}
		if (m11 > m22)
		{
			float num6 = (float)std::sqrt(((1.0f + m11) - m00) - m22);
			float num3 = 0.5f / num6;
			quaternion.x = (m10 + m01) * num3;
			quaternion.y = 0.5f * num6;
			quaternion.z = (m21 + m12) * num3;
			quaternion.w = (m20 - m02) * num3;

			return quaternion;
		}
		float num5 = (float)std::sqrt(((1.0f + m22) - m00) - m11);
		float num2 = 0.5f / num5;
		quaternion.x = (m20 + m02) * num2;
		quaternion.y = (m21 + m12) * num2;
		quaternion.z = 0.5f * num5;
		quaternion.w = (m01 - m10) * num2;

		return quaternion;

		//// Normalize inputs
		//Vector3 forward = forward.Normalized();
		//Vector3 upwards = up.Normalized();
		//// Don't allow zero vectors
		//if (forward.LengthSq() < EPSILON || upwards.LengthSq() < EPSILON)
		//	return Quaternion::Identity();
		//// Handle alignment with up direction
		//if (1 - fabs( forward.Dot(upwards) ) < EPSILON)
		//	return FromToRotation(front, forward);
		//// Get orthogonal vectors
		//Vector3 right = Cross(upwards, forward).Normalized();
		//upwards = Cross(forward, right);
		//// Calculate rotation
		//Quaternion quaternion;
		//float radicand = right.x + upwards.y + forward.z;
		//if (radicand > 0)
		//{
		//	quaternion.w = sqrt(1.0f + radicand) * 0.5f;
		//	float recip = 1.0f / (4.0f * quaternion.w);
		//	quaternion.x = (upwards.z - forward.y) * recip;
		//	quaternion.y = (forward.x - right.z) * recip;
		//	quaternion.z = (right.y - upwards.x) * recip;
		//}
		//else if (right.x >= upwards.y && right.x >= forward.z)
		//{
		//	quaternion.x = sqrt(1.0f + right.x - upwards.y - forward.z) * 0.5f;
		//	float recip = 1.0f / (4.0f * quaternion.x);
		//	quaternion.w = (upwards.z - forward.y) * recip;
		//	quaternion.z = (forward.x + right.z) * recip;
		//	quaternion.y = (right.y + upwards.x) * recip;
		//}
		//else if (upwards.y > forward.z)
		//{
		//	quaternion.y = sqrt(1.0f - right.x + upwards.y - forward.z) * 0.5f;
		//	float recip = 1.0f / (4.0f * quaternion.y);
		//	quaternion.z = (upwards.z + forward.y) * recip;
		//	quaternion.w = (forward.x - right.z) * recip;
		//	quaternion.x = (right.y + upwards.x) * recip;
		//}
		//else
		//{
		//	quaternion.z = sqrt(1.0f - right.x - upwards.y + forward.z) * 0.5f;
		//	float recip = 1.0f / (4.0f * quaternion.z);
		//	quaternion.y = (upwards.z + forward.y) * recip;
		//	quaternion.x = (forward.x + right.z) * recip;
		//	quaternion.w = (right.y - upwards.x) * recip;
		//}
		//return quaternion;
	}

	Quaternion nLerp(const Quaternion& start_, const Quaternion& end_, float time_)
	{
	  //float oneMinusAlpha = 1.0f - time_;
	  //Quaternion quaternion(start_ * oneMinusAlpha + end_ * time_);
	  //return quaternion.Normalized();
		time_ = Dot(start_, end_) > 0 ? time_ : -time_;

		Quaternion v0 = (1.0f - time_) * start_;
		Quaternion v1 = time_ * end_;
		return (v0 + v1).Normalized();
	}

	Quaternion Slerp(const Quaternion& q1_, const Quaternion& q2_, float time_) {
		float invert = 1.0f;

		// Compute cos(theta) using the quaternion scalar product
		float cosineTheta = q1_.Dot(q2_);

		// Take care of the sign of cosineTheta
		if (cosineTheta < float(0.0)) {
			cosineTheta = -cosineTheta;
			invert = -1.0f;
		}

		// Because of precision, if cos(theta) is nearly 1,
		// therefore theta is nearly 0 and we can write
		// sin((1-t)*theta) as (1-t) and sin(t*theta) as t
		if (1 - cosineTheta < EPSILON)
		{
			return q1_ * (float(1.0f) - time_) + q2_ * (time_ * invert);
		}

		// Compute the theta angle
		float theta = std::acos(cosineTheta);

		// Compute sin(theta)
		float sineTheta = std::sin(theta);

		// Compute the two coefficients that are in the spherical linear interpolation formula
		float coeff1 = std::sin((float(1.0f) - time_)*theta) / sineTheta;
		float coeff2 = std::sin(time_ * theta) / sineTheta * invert;

		// Compute and return the interpolated quaternion
		return q1_ * coeff1 + q2_ * coeff2;
	}

	Quaternion ToQuaternion(const Matrix44& rot_matrix_)
	{
		Quaternion q;
		//q.w = sqrt(1.0f + rot_matrix_.md[0][0] + rot_matrix_.md[1][1] + rot_matrix_.md[2][2]) * 0.5f;
		//float w4 = 1.0f / (4.0f * q.w);
		//q.x = (rot_matrix_.md[2][1] - rot_matrix_.md[1][2]) * w4;
		//q.y = (rot_matrix_.md[0][2] - rot_matrix_.md[2][0]) * w4;
		//q.z = (rot_matrix_.md[1][0] - rot_matrix_.md[0][1]) * w4;

		float tr = rot_matrix_.md[0][0] + rot_matrix_.md[1][1] + rot_matrix_.md[2][2];
		float s = 0.0f;
		if (tr >= 0)
		{
			s = sqrtf(tr + 1.0f);
			q.w = s * 0.5f;
			q.x = (rot_matrix_.md[2][1] - rot_matrix_.md[1][2]) * q.w;
			q.y = (rot_matrix_.md[0][2] - rot_matrix_.md[2][0]) * q.w;
			q.z = (rot_matrix_.md[1][0] - rot_matrix_.md[0][1]) * q.w;
		}
		else
		{
			int i = 0;
			if (rot_matrix_.md[1][1] > rot_matrix_.md[0][0]) i = 1;
			if (rot_matrix_.md[2][2] > rot_matrix_.md[i][i]) i = 2;

			switch (i)
			{
			case 0:
				s = sqrtf(rot_matrix_.md[0][0] - rot_matrix_.md[1][1] + rot_matrix_.md[2][2] + 1.0f);
				q.x = 0.5f * s;
				s = 0.5f / s;
				q.y = (rot_matrix_.md[0][1] + rot_matrix_.md[1][0]) * s;
				q.z = (rot_matrix_.md[2][0] + rot_matrix_.md[0][2]) * s;
				q.w = (rot_matrix_.md[2][1] - rot_matrix_.md[1][2]) * s;
				break;
			case 1:
				s = sqrtf(rot_matrix_.md[1][1] - rot_matrix_.md[2][2] + rot_matrix_.md[0][0] + 1.0f);
				q.y = 0.5f * s;
				s = 0.5f / s;
				q.z = (rot_matrix_.md[1][2] + rot_matrix_.md[2][1]) * s;
				q.x = (rot_matrix_.md[0][1] + rot_matrix_.md[1][0]) * s;
				q.w = (rot_matrix_.md[0][2] - rot_matrix_.md[2][0]) * s;
				break;
			case 2:
				s = sqrtf(rot_matrix_.md[2][2] - rot_matrix_.md[0][0] + rot_matrix_.md[1][1] + 1.0f);
				q.z = 0.5f * s;
				s = 0.5f / s;
				q.x = (rot_matrix_.md[2][0] + rot_matrix_.md[0][2]) * s;
				q.y = (rot_matrix_.md[1][2] + rot_matrix_.md[2][1]) * s;
				q.w = (rot_matrix_.md[1][0] - rot_matrix_.md[0][1]) * s;
				break;
			}
		}
		return q;
	}

} //namespace zg
