/*****************************************************************************/
/*!
\file			Color.cpp
\brief			Color value normalize
*/
/*****************************************************************************/
#include "stdafx.h"
#include "Color.h"

namespace zg {
	Color::Color() noexcept
		:r(1.0f), g(1.0f), b(1.0f), a(1.0f) 
	{}

	Color::Color(float r_, float g_, float b_) noexcept
		:r(r_), g(g_), b(b_), a(1.0f)
	{}

	Color::Color(float r_, float g_, float b_, float a_) noexcept
		:r(r_), g(g_), b(b_), a(a_)
	{}
	
	Color::Color(int r_, int g_, int b_) noexcept
		: r(r_ * INV_255F), g(g_ * INV_255F), b(b_ * INV_255F), a(1.0f)
	{}

	Color::Color(int r_, int g_, int b_, int a_) noexcept
		: r(r_ * INV_255F), g(g_ * INV_255F), b(b_ * INV_255F), a(a_ * INV_255F)
	{}
	//getters
	int Color::GetR() const { return static_cast<int>(std::roundf(r * 255.0f)); }
	int Color::GetG() const { return static_cast<int>(std::roundf(g * 255.0f)); }
	int Color::GetB() const { return static_cast<int>(std::roundf(b * 255.0f)); }
	int Color::GetA() const { return static_cast<int>(std::roundf(a * 255.0f)); }
	//setters
	void Color::Set(int r_, int g_, int b_)
	{
	  r = r_ * INV_255F;
	  g = g_ * INV_255F;
	  b = b_ * INV_255F;
	}
	void Color::Set(int r_, int g_, int b_, int a_){
	  r = r_ * INV_255F;
	  g = g_ * INV_255F;
	  b = b_ * INV_255F;
	  a = a_ * INV_255F;
	}
	//operators
	bool   Color::operator==(const Color& c_) const { return r == c_.r && g == c_.g && b == c_.b && a == c_.a; }
	Color& Color::operator+=(const Color& c_) { r += c_.r;	g += c_.g;	b += c_.b;	a += c_.a; return *this; }
	Color& Color::operator-=(const Color& c_) { r -= c_.r;	g -= c_.g;	b -= c_.b;	a -= c_.a; return *this; }
	Color& Color::operator*=(float f_)		  { r *= f_;	g *= f_;	b *= f_;	a *= f_;   return *this; }
	Color& Color::operator/=(float f_)		  { 
		float d = CHECK_NEG_ZERO(f_) ? 0.0f : 1.0f / f_;
		r *= d;	g *= d;	b *= d;	a *= d;
		return *this;
	}

	#ifdef USING_SOL2
		Color Color::operator-()				const { return Color{ -r,		-g,			-b,			-a }; }
		Color Color::operator-(float f_)		const { return Color{ r - f_,	g - f_,		b - f_,		a - f_ }; }
		Color Color::operator-(const Color& c_)	const { return Color{ r - c_.r, g - c_.g,	b - c_.b,	a - c_.a }; }
		Color Color::operator+(float f_)		const { return Color{ r + f_,	g + f_,		b + f_,		a + f_ }; }
		Color Color::operator+(const Color& c_)	const { return Color{ r + c_.r, g + c_.g,	b + c_.b,	a + c_.a }; }
		Color Color::operator*(const Color& c_)	const { return Color{ r * c_.r, g * c_.g,	b * c_.b,	a * c_.a }; }
		Color Color::operator*(float f_)		const { return Color{ r * f_,	g * f_,		b * f_,		a * f_ }; }
		Color Color::operator/(float f_)		const { Color tmp{ *this }; return tmp /= f_; }
		void Color::BindLua(sol::state & lua){
			lua.new_usertype<Color>("Color",
				sol::constructors<
					Color(), 
					Color(float, float, float, float), 
					Color(int, int, int, int)>(),
				"r", &Color::r,
				"g", &Color::g,
				"b", &Color::b,
				"a", &Color::a,
				"GetR", &Color::GetR,
				"GetG", &Color::GetG,
				"GetB", &Color::GetB,
				"GetA", &Color::GetA,
		
				sol::meta_function::addition, sol::overload(
					static_cast<Color(Color::*)(float) const>(&Color::operator+),
					static_cast<Color(Color::*)(const Color&) const>(&Color::operator+)
				),
				sol::meta_function::subtraction, sol::overload(
					static_cast<Color(Color::*)() const>(&Color::operator-),
					static_cast<Color(Color::*)(float) const>(&Color::operator-),
					static_cast<Color(Color::*)(const Color&) const>(&Color::operator-)
				),
				sol::meta_function::multiplication, sol::overload(
					static_cast<Color(Color::*)(float) const>(&Color::operator*),
					static_cast<Color(Color::*)(const Color&) const>(&Color::operator*)
				),
				sol::meta_function::division, &Color::operator/
		
				);
		}
	#else
		Color operator-(const Color& c0_)					{ return Color{ -c0_.r,			-c0_.g,			-c0_.b,			-c0_.a			}; } //negate
		Color operator-(const Color& c0_, const Color& c1_)	{ return Color{ c0_.r - c1_.r,	c0_.g - c1_.g,	c0_.b - c1_.b,	c0_.a - c1_.a	}; } //minus
		Color operator-(const Color& c0_, float c_)			{ return Color{ c0_.r - c_,		c0_.g - c_,		c0_.b - c_,		c0_.a - c_		}; } //minus
		Color operator-(float c_, const Color& c0_)			{ return Color{ c0_.r - c_,		c0_.g - c_,		c0_.b - c_,		c0_.a - c_		}; } //minus
		Color operator+(const Color& c0_, const Color& c1_)	{ return Color{ c0_.r + c1_.r,	c0_.g + c1_.g,	c0_.b + c1_.b,	c0_.a + c1_.a	}; } //plus
		Color operator+(const Color& c0_, float c_)			{ return Color{ c0_.r + c_,		c0_.g + c_,		c0_.b + c_,		c0_.a + c_		}; } //plus
		Color operator+(float c_, const Color& c0_)			{ return Color{ c0_.r + c_,		c0_.g + c_,		c0_.b + c_,		c0_.a + c_		}; } //plus
		Color operator*(const Color& c0_, const Color& c1_)	{ return Color{ c0_.r * c1_.r ,	c0_.g * c1_.g,	c0_.b * c1_.b,	c0_.a * c1_.a	}; } //multiply
		Color operator*(const Color& c0_, float c_)			{ return Color{ c0_.r * c_,		c0_.g * c_,		c0_.b * c_,		c0_.a * c_		}; } //multiply
		Color operator*(float c_, const Color& c0_)			{ return Color{ c0_.r * c_,		c0_.g * c_,		c0_.b * c_,		c0_.a * c_		}; } //multiply
		Color operator/(const Color& c0_, float c_)			{ Color tmp{ c0_ }; return tmp /= c_;											   } //division
	#endif
	
	std::ostream& operator<<(std::ostream& os, const Color& v) {
		printf("{ %.6f %.6f %.6f %.6f }\n", v.r, v.g, v.b, v.a);
		return os;
	}
	
	Color Lerp(const Color& start_, const Color& end_, float time_) {
		// Imprecise method, which does not guarantee v = v1 when t = 1, due to floating-point arithmetic error.
		// This form may be used when the hardware has a native fused multiply-add instruction.
		//return Color{
		//	start_.r + time_ * (end_.r - start_.r),
		//	start_.g + time_ * (end_.g - start_.g),
		//	start_.b + time_ * (end_.b - start_.b),
		//	start_.a + time_ * (end_.a - start_.a),
		//};

		// Precise method, which guarantees v = v1 when t = 1.
		realtype dttmp = 1.0f - time_;
		return Color{
			dttmp * start_.r + time_ * end_.r,
			dttmp * start_.g + time_ * end_.g,
			dttmp * start_.b + time_ * end_.b,
			dttmp * start_.a + time_ * end_.a
		};
	}

} //namespace zg
