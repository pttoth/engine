/*
 * -------------------------------------------------------------------------
 * FILE:    float1.h
 * AUTHOR:  Dr. Szécsi László - szecsi@iit.bme.hu
 * -------------------------------------------------------------------------
 */
#pragma once

#include <math.h>

#define DEFINE_FLOAT1_METHOD(FLOAT1_METHOD_NAME, FLOAT1_METHOD_RETURN_TYPE)	\
	FLOAT1_METHOD_RETURN_TYPE FLOAT1_METHOD_NAME () const	\
	{	\
		return FLOAT1_METHOD_RETURN_TYPE ( :: FLOAT1_METHOD_NAME (x) );	\
	}

#define DEFINE_FLOAT1_METHOD_DIADIC(FLOAT1_METHOD_NAME, FLOAT1_METHOD_RETURN_TYPE)	\
	FLOAT1_METHOD_RETURN_TYPE FLOAT1_METHOD_NAME (const float1& o) const	\
	{	\
		return FLOAT1_METHOD_RETURN_TYPE ( :: FLOAT1_METHOD_NAME (x, o.x) );	\
	}

#define DEFINE_FLOAT1_METHOD_TRIADIC(FLOAT1_METHOD_NAME, FLOAT1_METHOD_RETURN_TYPE)	\
	FLOAT1_METHOD_RETURN_TYPE FLOAT1_METHOD_NAME (const float1& a, const float1& b) const	\
	{	\
		return FLOAT1_METHOD_RETURN_TYPE ( :: FLOAT1_METHOD_NAME (x, a.x, b.x) );	\
	}

namespace pttoth { namespace math {

class float2;
class float3;
class float4;

class float1
{
public:

	union{
		struct {
			float x;
		};

		float v[1];

		float2swizzle<1, float2, 0, 0> xx;
		float3swizzle<1, float3, 0, 0, 0> xxx;
		float4swizzle<1, float4, 0, 0, 0, 0> xxxx;

	};

	operator float() const
	{
		return x;
	}

	float1():x(0.0f){}

	float1(float f):x(f){}

	float1(float x, float y, float z, float w):x(x){y; z; w;}

	float1(int1 i):x((float)i.x){}

	float1& operator+=(const float1& o)
	{
		x += o.x;
	}

	float1& operator-=(const float1& o)
	{
		x -= o.x;
	}

	float1& operator*=(const float1& o)
	{
		x *= o.x;
	}

	float1& operator/=(const float1& o)
	{
		x /= o.x;
	}

	float1& operator%=(const float1& o)
	{
		x = fmodf(x, o.x);
	}

	bool1 operator==(const float1& o) const
	{
		return bool1(x == o.x);
	}

	bool1 operator!=(const float1& o) const
	{
		return bool1(x != o.x);
	}

	bool1 operator<(const float1& o) const
	{
		return bool1(x < o.x);
	}

	bool1 operator<=(const float1& o) const
	{
		return bool1(x <= o.x);
	}

	bool1 operator>(const float1& o) const
	{
		return bool1(x > o.x);
	}

	bool1 operator>=(const float1& o) const
	{
		return bool1(x >= o.x);
	}

	float1 operator+(const float1& o) const
	{
		return float1(x + o.x);
	}

	float1 operator-(const float1& o) const
	{
		return float1(x - o.x);
	}

	float1 operator*(const float1& o) const
	{
		return float1(x * o.x);
	}

	float1 operator/(const float1& o) const
	{
		return float1(x / o.x);
	}

	float1 operator%(const float1& o) const
	{
		return float1(fmodf(x, o.x));
	}

	float1 operator+() const
	{
		return float1(+x);
	}

	float1 operator-() const
	{
		return float1(-x);
	}

	float operator[](unsigned int i) const
	{
		if(i < 0 || i > 0)
			throw std::range_error("float1 index out of range.");
		return v[i];
	}

	float& operator[](unsigned int i)
	{
		if(i < 0 || i > 0)
			throw std::range_error("float1 index out of range.");
		return v[i];
	}

	DEFINE_FLOAT1_METHOD(abs, float1)
	DEFINE_FLOAT1_METHOD(acos, float1)
	DEFINE_FLOAT1_METHOD(asin, float1)
	DEFINE_FLOAT1_METHOD(atan, float1)
	DEFINE_FLOAT1_METHOD(ceil, int1)
	DEFINE_FLOAT1_METHOD(cos, float1)
	DEFINE_FLOAT1_METHOD(cosh, float1)
	DEFINE_FLOAT1_METHOD(exp, float1)
	DEFINE_FLOAT1_METHOD(floor, int1)

	float1 exp2() const
	{
		return float1( ::pow(2.0f, x));
	}

	DEFINE_FLOAT1_METHOD_DIADIC(atan2, float1)
	DEFINE_FLOAT1_METHOD_DIADIC(fmod, float1)

	float distance(const float1& o) const
	{
		return (*this - o).length();
	}

	float dot(const float1& o) const
	{
		return x * o.x;
	}

	float1 frac() const
	{
		return *this - trunc();
	}
/*
	bool1 isfinite() const
	{
		return bool1( ::_finite(x));
	}

	bool1 isinf() const
	{
		return bool1( !::_finite(x));
	}

	bool1 isnan() const
	{
		return bool1( ::_isnan(x));
	}
*/
	float length() const
	{
		return sqrtf( this->dot(*this));
	}

	float lengthSquared() const
	{
		return this->dot(*this);
	}

	float1 lerp(const float1& a, const float1& b) const
	{
		return a * (float1(1) - *this) + b * *this;
	}

	DEFINE_FLOAT1_METHOD(log, float1)
	DEFINE_FLOAT1_METHOD(log10, float1)

	float1 log2() const
	{
		return float1( ::log(x) / ::log(2.0f));
	}

	float1 max(const float1& o) const
	{
		return float1( (x>o.x)?x:o.x);
	}

	float1 min(const float1& o) const
	{
		return float1( (x<o.x)?x:o.x);
	}

	float1 normalize() const
	{
		return *this / float1(length());
	}

	DEFINE_FLOAT1_METHOD_DIADIC(pow, float1)

	int1 round() const
	{
		return int1( (int)(x+0.5f));
	}

	float1 rsqrt() const
	{
		return float1( 1.0f / ::sqrtf(x));
	}

	float1 saturate() const
	{
		return clamp( float1(0), float1(1));
	}

	float1 sign() const
	{
		return float1( (x>0.0f)?1.0f:((x<0.0f)?-1.0f:0.0f));
	}

	DEFINE_FLOAT1_METHOD(sin, float1)
	DEFINE_FLOAT1_METHOD(sinh, float1)

	float1 smoothstep(const float1& a, const float1& b) const
	{
		float r = ((*this - a)/(b - a)).saturate(); 
		return r*r*(3 - r*2);
	}

	float1 sqrt() const
	{
		return float1( ::sqrtf(x));
	}

	float1 step(const float1& o) const
	{
		return float1( (x>=o.x)?1.0f:0.0f);
	}

	DEFINE_FLOAT1_METHOD(tan, float1)

	int1 trunc() const
	{
		return int1( (int)(x));
	}

	float1 clamp(const float1& low, const float1& high) const
	{
		return float1( (x<low.x)?low.x:((x>high.x)?high.x:x));
	}

	static float1 random(float lower=0.0f, float upper=1.0f)
	{
		float range = upper - lower;
		return float1(
			rand() * range / RAND_MAX + lower);
	}

	static const float1 zero;
	static const float1 xUnit;
	static const float1 one;

};

}}
