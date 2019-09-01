/*****************************************************************************/
/*!
\file			Reals.cpp
\brief			Math Implmentation
*/
/*****************************************************************************/
#include "stdafx.h"
#include "Reals.h"

namespace zg {
	float ToRad(float value) { return value * PI180; }
	float ToDeg(float value) { return value * INV_PI180; }

	ZG_API unsigned int GOBAL_RESOLUTION_WIDTH  = 1280;
	ZG_API unsigned int GOBAL_RESOLUTION_HEIGHT = 720;
	void SetResolution(RESOLUTION_SIZE size_) {
		//4::3
		switch (size_) {
		case R_640x480:		GOBAL_RESOLUTION_WIDTH = 640;  GOBAL_RESOLUTION_HEIGHT = 480;	break;
		case R_800x600:		GOBAL_RESOLUTION_WIDTH = 800;  GOBAL_RESOLUTION_HEIGHT = 600;	break;
		case R_960x720: 	GOBAL_RESOLUTION_WIDTH = 960;  GOBAL_RESOLUTION_HEIGHT = 720;	break;
		case R_1024x768: 	GOBAL_RESOLUTION_WIDTH = 1024; GOBAL_RESOLUTION_HEIGHT = 768;	break;
		case R_1280x960: 	GOBAL_RESOLUTION_WIDTH = 1280; GOBAL_RESOLUTION_HEIGHT = 960;	break;
		case R_1440x1050: 	GOBAL_RESOLUTION_WIDTH = 1440; GOBAL_RESOLUTION_HEIGHT = 1050;	break;
		case R_1440x1080: 	GOBAL_RESOLUTION_WIDTH = 1440; GOBAL_RESOLUTION_HEIGHT = 1080;	break;
		case R_1600x1200: 	GOBAL_RESOLUTION_WIDTH = 1600; GOBAL_RESOLUTION_HEIGHT = 1200;	break;
		case R_1856x1392: 	GOBAL_RESOLUTION_WIDTH = 1856; GOBAL_RESOLUTION_HEIGHT = 1392;	break;
		case R_1920x1440: 	GOBAL_RESOLUTION_WIDTH = 1920; GOBAL_RESOLUTION_HEIGHT = 1440;	break;
		case R_2048x1536: 	GOBAL_RESOLUTION_WIDTH = 2048; GOBAL_RESOLUTION_HEIGHT = 1536;	break;
			//16:10
		case R_1280x800: 	GOBAL_RESOLUTION_WIDTH = 1280; GOBAL_RESOLUTION_HEIGHT = 800;	break;
		case R_1440x900: 	GOBAL_RESOLUTION_WIDTH = 1440; GOBAL_RESOLUTION_HEIGHT = 900;	break;
		case R_1680x1050: 	GOBAL_RESOLUTION_WIDTH = 1680; GOBAL_RESOLUTION_HEIGHT = 1050;	break;
		case R_1920x1200: 	GOBAL_RESOLUTION_WIDTH = 1920; GOBAL_RESOLUTION_HEIGHT = 1200;	break;
		case R_2560x1600: 	GOBAL_RESOLUTION_WIDTH = 2560; GOBAL_RESOLUTION_HEIGHT = 1600;	break;
			//16:9
		case R_1024x576: 	GOBAL_RESOLUTION_WIDTH = 1024; GOBAL_RESOLUTION_HEIGHT = 576;	break;
		case R_1152x648: 	GOBAL_RESOLUTION_WIDTH = 1152; GOBAL_RESOLUTION_HEIGHT = 648;	break;
		case R_1280x720: 	GOBAL_RESOLUTION_WIDTH = 1280; GOBAL_RESOLUTION_HEIGHT = 720;	break;
		case R_1366x768: 	GOBAL_RESOLUTION_WIDTH = 1366; GOBAL_RESOLUTION_HEIGHT = 768;	break;
		case R_1600x900: 	GOBAL_RESOLUTION_WIDTH = 1600; GOBAL_RESOLUTION_HEIGHT = 900;	break;
		case R_1920x1080: 	GOBAL_RESOLUTION_WIDTH = 1920; GOBAL_RESOLUTION_HEIGHT = 1080;	break;
		case R_2560x1440: 	GOBAL_RESOLUTION_WIDTH = 2560; GOBAL_RESOLUTION_HEIGHT = 1440;	break;
		case R_3840x2160: 	GOBAL_RESOLUTION_WIDTH = 3840; GOBAL_RESOLUTION_HEIGHT = 2160;	break;
		case R_7680x4320:	GOBAL_RESOLUTION_WIDTH = 7680; GOBAL_RESOLUTION_HEIGHT = 4320;	break;
		}
	}
	void SetResolution(unsigned int* width_, unsigned int* height_, RESOLUTION_SIZE size_) {
		//4::3
		switch (size_) {
		case R_640x480:		*width_ = 640;  *height_ = 480;		break;
		case R_800x600:		*width_ = 800;  *height_ = 600;		break;
		case R_960x720: 	*width_ = 960;  *height_ = 720;		break;
		case R_1024x768: 	*width_ = 1024; *height_ = 768;		break;
		case R_1280x960: 	*width_ = 1280; *height_ = 960;		break;
		case R_1440x1050: 	*width_ = 1440; *height_ = 1050;	break;
		case R_1440x1080: 	*width_ = 1440; *height_ = 1080;	break;
		case R_1600x1200: 	*width_ = 1600; *height_ = 1200;	break;
		case R_1856x1392: 	*width_ = 1856; *height_ = 1392;	break;
		case R_1920x1440: 	*width_ = 1920; *height_ = 1440;	break;
		case R_2048x1536: 	*width_ = 2048; *height_ = 1536;	break;
			//16:10
		case R_1280x800: 	*width_ = 1280; *height_ = 800;		break;
		case R_1440x900: 	*width_ = 1440; *height_ = 900;		break;
		case R_1680x1050: 	*width_ = 1680; *height_ = 1050;	break;
		case R_1920x1200: 	*width_ = 1920; *height_ = 1200;	break;
		case R_2560x1600: 	*width_ = 2560; *height_ = 1600;	break;
			//16:9
		case R_1024x576: 	*width_ = 1024; *height_ = 576;		break;
		case R_1152x648: 	*width_ = 1152; *height_ = 648;		break;
		case R_1280x720: 	*width_ = 1280; *height_ = 720;		break;
		case R_1366x768: 	*width_ = 1366; *height_ = 768;		break;
		case R_1600x900: 	*width_ = 1600; *height_ = 900;		break;
		case R_1920x1080: 	*width_ = 1920; *height_ = 1080;	break;
		case R_2560x1440: 	*width_ = 2560; *height_ = 1440;	break;
		case R_3840x2160: 	*width_ = 3840; *height_ = 2160;	break;
		case R_7680x4320:	*width_ = 7680; *height_ = 4320;	break;
		}
	}

	//float
	//Trigonometric
	float sin	(float x)			{ return std::sinf(x);		}
	float cos	(float x)			{ return std::cosf(x);		}
	float tan	(float x)			{ return std::tanf(x);		}
	float asin	(float x)			{ return std::asinf(x);		}
	float acos	(float x)			{ return std::acosf(x);		}
	float atan	(float x)			{ return std::atanf(x);		}
	float atan2	(float y, float x)	{ return std::atan2f(y, x); }

	//Hyperbolic 
	float sinh	(float x) {	return std::sinh(x);	}
	float cosh	(float x) {	return std::cosh(x);	}
	float tanh	(float x) {	return std::tanh(x);	}
	float asinh	(float x) {	return std::asinh(x);	}
	float acosh	(float x) {	return std::acosh(x);	}
	float atanh	(float x) {	return std::atanh(x);	}

	//Power
	float pow	(float x, float exp)	{ return std::powf(x, exp); }
	float sqrt	(float x)				{ return std::sqrtf(x); }

	//double
	//Exponential and logarithmic
	double sin	(double x)			 { return std::sin(x);		}
	double cos	(double x)			 { return std::cos(x);		}
	double tan	(double x)			 { return std::tan(x);		}
	double asin	(double x)			 { return std::asin(x);		}
	double acos	(double x)			 { return std::acos(x);		}
	double atan	(double x)			 { return std::atan(x);		}
	double atan2(double y, double x) { return std::atan2(y, x); }

	//Hyperbolic 
	double sinh	(double x) { return std::sinh(x);	}
	double cosh	(double x) { return std::cosh(x);	}
	double tanh	(double x) { return std::tanh(x);	}
	double asinh(double x) { return std::asinh(x);	}
	double acosh(double x) { return std::acosh(x);	}
	double atanh(double x) { return std::atanh(x);	}

	//Power
	double pow	(double x, double exp)	{ return std::pow(x, exp); }
	double sqrt	(double x)				{ return std::sqrt(x); }

#pragma region __________________________________________________ExtraExternal	
	float Absolute(float val)	{ return val < 0.0f ? -val : val;	}
	double Absolute(double val)	{ return val < 0.0 ? -val : val;		}

	bool ApproxEqual(float a, float b, float epsilon) {
		return (std::fabs(a - b) < epsilon);
	}
	bool ApproxEqual(double a, double b, double epsilon) {
		return (std::fabs(a - b) < epsilon);
	}

	float Lerp(float start_, float end_, float time_) {
		// Imprecise start_, which does not guarantee v = v1 when t = 1, due to floating-point arithmetic error.
		// This form may be used when the hardware has a native fused multiply-add instruction.
		//return start_ + time_* (end_ - start_);

		// Precise method, which guarantees v = v1 when t = 1.
		float dttmp = 1.0f - time_;
		return dttmp * start_ + time_ * end_;
	}
	double Lerp(double start_, double end_, double time_) {
		//return start_ + time_* (end_ - start_);
		double dttmp = 1.0 - time_;
		return dttmp * start_ + time_ * end_;
	}

	// == Random generator
	float randomf(float value) {
		float bloat = value * 100.0f;
		return (rand() % (int)(bloat + 1) * 1.0f) / 100.0f;
	}
	// == Random Range generator
	float randomRangef(float min, float max) {
		float bloatmin = min * 10.0f;
		float bloatmax = max * 10.0f;
		return (bloatmin + rand() % (int)(bloatmax - bloatmin + 1)) / 10.0f;
	}
	int random1ToMax(int max) {
		return rand() % max + 1;
	}
	int randomRange(int min, int max) {
		max += 1;
		return rand() % (max - min) + min;
	}
	//http://www.rgba.org/articles/sfrand/sfrand.htm
	//Generates between -1 to 1
	static unsigned int mirand = 1;
	float sfrand() {
		unsigned int a;
		mirand *= 16807;
		a = (mirand & 0x007fffff) | 0x40000000;
		return(*((float*)&a) - 3.0f);
	}
	float sfrand(float min, float max) {
		return (sfrand() + 1) / 2 * (max - min) + min;
	}
	float QuinIn(float time, float begin, float final, float duration) {
		if (begin != final)
		{
			if (begin < 0)
			{
				final += begin * -1.0f;
			}
			else
				final -= begin;

			time /= duration;
			return final * time*time + begin;
		}
		else
			return begin;
	};
	float QuinOut(float time, float begin, float final, float duration) {
		if (begin != final)
		{
			if (begin < 0)
			{
				final += begin * -1.0f;
			}
			else
				final -= begin;

			float ts = (time /= duration)*time;
			float tc = ts * time;
			return begin + final * (tc*ts + -5 * ts*ts + 10 * tc + -10 * ts + 5 * time);
		}
		else
			return begin;
	}

	float BounceIn(float time, float begin, float final, float duration) {
		return final - BounceOut(duration - time, 0, final, duration) + begin;
	}

	float BounceOut(float time, float begin, float final, float duration) {
		if ((time /= duration) < (1 / 2.75f)) {
			return final * (7.5625f*time*time) + begin;
		}
		else if (time < (2 / 2.75f)) {
			float postFix = time -= (1.5f / 2.75f);
			return final * (7.5625f*(postFix)*time + .75f) + begin;
		}
		else if (time < (2.5 / 2.75)) {
			float postFix = time -= (2.25f / 2.75f);
			return final * (7.5625f*(postFix)*time + .9375f) + begin;
		}
		else {
			float postFix = time -= (2.625f / 2.75f);
			return final * (7.5625f*(postFix)*time + .984375f) + begin;
		}
	}

	float SmoothDampf(float current, float target, float currentVelocity, float smoothTime, float maxSpeed, float deltaTime)
	{
		smoothTime = MAX2(0.0001f, smoothTime);
		float num = 2.0f / smoothTime;
		float num2 = num * deltaTime;
		float num3 = 1.0f / (1.0f + num2 + 0.48f * num2 * num2 + 0.235f * num2 * num2 * num2);
		float num4 = current - target;
		float num5 = target;
		float num6 = maxSpeed * smoothTime;
		num4 = zg::Clamp(num4, -num6, num6);
		target = current - num4;
		float num7 = (currentVelocity + num * num4) * deltaTime;
		currentVelocity = (currentVelocity - num * num7) * num3;
		float num8 = target + (num4 + num7) * num3;
		if (num5 - current > 0.0f == num8 > num5)
		{
			num8 = num5;
			currentVelocity = (num8 - num5) / deltaTime;
		}
		return num8;
	}


#pragma endregion

}//namespace zg
