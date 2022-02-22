/*
 * -------------------------------------------------------------------------
 * FILE:    float2.h
 * AUTHOR:  Dr. Szécsi László - szecsi@iit.bme.hu
 * -------------------------------------------------------------------------
 */
 #pragma once

#include <math.h>

#define DEFINE_FLOAT2_METHOD(FLOAT2_METHOD_NAME, FLOAT2_METHOD_RETURN_TYPE)	\
	FLOAT2_METHOD_RETURN_TYPE FLOAT2_METHOD_NAME () const	\
	{	\
		return FLOAT2_METHOD_RETURN_TYPE ( :: FLOAT2_METHOD_NAME (x), :: FLOAT2_METHOD_NAME (y));	\
	}

#define DEFINE_FLOAT2_METHOD_DIADIC(FLOAT2_METHOD_NAME, FLOAT2_METHOD_RETURN_TYPE)	\
	FLOAT2_METHOD_RETURN_TYPE FLOAT2_METHOD_NAME (const float2& o) const	\
	{	\
		return FLOAT2_METHOD_RETURN_TYPE ( :: FLOAT2_METHOD_NAME (x, o.x), :: FLOAT2_METHOD_NAME (y, o.y));	\
	}

#define DEFINE_FLOAT2_METHOD_TRIADIC(FLOAT2_METHOD_NAME, FLOAT2_METHOD_RETURN_TYPE)	\
	FLOAT2_METHOD_RETURN_TYPE FLOAT2_METHOD_NAME (const float2& a, const float2& b) const	\
	{	\
		return FLOAT2_METHOD_RETURN_TYPE ( :: FLOAT2_METHOD_NAME (x, a.x, b.x), :: FLOAT2_METHOD_NAME (y, a.y, b.y));	\
	}

namespace pttoth { namespace math {

class float2;
class float3;
class float4;

class float2
{
public:

	union{
		struct {
			float x;
			float y;
		};

		float v[2];

		float2swizzle<2, float2, 0, 0> xx;
		float2swizzle<2, float2, 0, 1> xy;
		float2swizzle<2, float2, 1, 0> yx;
		float2swizzle<2, float2, 1, 1> yy;

		float3swizzle<2, float3, 0, 0, 0> xxx;
		float3swizzle<2, float3, 0, 0, 1> xxy;
		float3swizzle<2, float3, 0, 1, 0> xyx;
		float3swizzle<2, float3, 0, 1, 1> xyy;
		float3swizzle<2, float3, 1, 0, 0> yxx;
		float3swizzle<2, float3, 1, 0, 1> yxy;
		float3swizzle<2, float3, 1, 1, 0> yyx;
		float3swizzle<2, float3, 1, 1, 1> yyy;

		float4swizzle<2, float4, 0, 0, 0, 0> xxxx;
		float4swizzle<2, float4, 0, 0, 0, 1> xxxy;
		float4swizzle<2, float4, 0, 0, 1, 0> xxyx;
		float4swizzle<2, float4, 0, 0, 1, 1> xxyy;
		float4swizzle<2, float4, 0, 1, 0, 0> xyxx;
		float4swizzle<2, float4, 0, 1, 0, 1> xyxy;
		float4swizzle<2, float4, 0, 1, 1, 0> xyyx;
		float4swizzle<2, float4, 0, 1, 1, 1> xyyy;
		float4swizzle<2, float4, 1, 0, 0, 0> yxxx;
		float4swizzle<2, float4, 1, 0, 0, 1> yxxy;
		float4swizzle<2, float4, 1, 0, 1, 0> yxyx;
		float4swizzle<2, float4, 1, 0, 1, 1> yxyy;
		float4swizzle<2, float4, 1, 1, 0, 0> yyxx;
		float4swizzle<2, float4, 1, 1, 0, 1> yyxy;
		float4swizzle<2, float4, 1, 1, 1, 0> yyyx;
		float4swizzle<2, float4, 1, 1, 1, 1> yyyy;

	};

	float2():x(0.0f),y(0.0f){}

	float2(float f):x(f),y(f){}

	float2(float x, float y):x(x),y(y){}

	float2(float x, float y, float z, float w):x(x),y(y){z; w;}

	float2(int2 i):x((float)i.x),y((float)i.y){}

	float2& operator+=(const float2& o)
	{
		x += o.x;
		y += o.y;
	}

	float2& operator-=(const float2& o)
	{
		x -= o.x;
		y -= o.y;
	}

	float2& operator*=(const float2& o)
	{
		x *= o.x;
		y *= o.y;
	}

	float2& operator/=(const float2& o)
	{
		x /= o.x;
		y /= o.y;
	}

	float2& operator%=(const float2& o)
	{
		x = fmodf(x, o.x);
		y = fmodf(y, o.y);
	}

	bool2 operator==(const float2& o) const
	{
		return bool2(x == o.x, y == o.y);
	}

	bool2 operator!=(const float2& o) const
	{
		return bool2(x != o.x, y != o.y);
	}

	bool2 operator<(const float2& o) const
	{
		return bool2(x < o.x, y < o.y);
	}

	bool2 operator<=(const float2& o) const
	{
		return bool2(x <= o.x, y <= o.y);
	}

	bool2 operator>(const float2& o) const
	{
		return bool2(x > o.x, y > o.y);
	}

	bool2 operator>=(const float2& o) const
	{
		return bool2(x >= o.x, y >= o.y);
	}

	float2 operator+(const float2& o) const
	{
		return float2(x + o.x, y + o.y);
	}

	float2 operator-(const float2& o) const
	{
		return float2(x - o.x, y - o.y);
	}

	float2 operator*(const float2& o) const
	{
		return float2(x * o.x, y * o.y);
	}

	float2 operator/(const float2& o) const
	{
		return float2(x / o.x, y / o.y);
	}

	float2 operator%(const float2& o) const
	{
		return float2(fmodf(x, o.x), fmodf(y, o.y));
	}


	float2 operator+() const
	{
		return float2(+x, +y);
	}

	float2 operator-() const
	{
		return float2(-x, -y);
	}

	float2 operator!() const
	{
		return float2(+x, -y);
	}

	float operator[](unsigned int i) const
	{
		if(i < 0 || i > 1)
			throw std::range_error("float2 index out of range.");
		return v[i];
	}

	float& operator[](unsigned int i)
	{
		if(i < 0 || i > 1)
			throw std::range_error("float2 index out of range.");
		return v[i];
	}

	DEFINE_FLOAT2_METHOD(abs, float2)
	DEFINE_FLOAT2_METHOD(acos, float2)
	DEFINE_FLOAT2_METHOD(asin, float2)
	DEFINE_FLOAT2_METHOD(atan, float2)
	DEFINE_FLOAT2_METHOD(ceil, int2)
	DEFINE_FLOAT2_METHOD(cos, float2)
	DEFINE_FLOAT2_METHOD(cosh, float2)
	DEFINE_FLOAT2_METHOD(exp, float2)
	DEFINE_FLOAT2_METHOD(floor, int2)

	float2 exp2() const
	{
		return float2( ::pow(2.0f, x), ::pow(2.0f, y));
	}

	DEFINE_FLOAT2_METHOD_DIADIC(atan2, float2)
	DEFINE_FLOAT2_METHOD_DIADIC(fmod, float2)

	float distance(const float2& o) const
	{
		return (*this - o).length();
	}

	float dot(const float2& o) const
	{
		return x * o.x + y * o.y;
	}

	float2 frac() const
	{
		return *this - trunc();
	}
/*
	bool2 isfinite() const
	{
		return bool2( ::_finite(x), ::_finite(y));
	}

	bool2 isinf() const
	{
		return bool2( !::_finite(x), !::_finite(y));
	}

	bool2 isnan() const
	{
		return bool2( ::_isnan(x), ::_isnan(y));
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

	float2 lerp(const float2& a, const float2& b) const
	{
		return a * (float2(1, 1) - *this) + b * *this;
	}

	DEFINE_FLOAT2_METHOD(log, float2)
	DEFINE_FLOAT2_METHOD(log10, float2)

	float2 log2() const
	{
		return float2( ::log(x) / ::log(2.0f), ::log(y) / ::log(2.0f));
	}

	float2 max(const float2& o) const
	{
		return float2( (x>o.x)?x:o.x, (y>o.y)?y:o.y);
	}

	float2 min(const float2& o) const
	{
		return float2( (x<o.x)?x:o.x, (y<o.y)?y:o.y);
	}

	float2 normalize() const
	{
		return *this / length();
	}

	DEFINE_FLOAT2_METHOD_DIADIC(pow, float2)

	int2 round() const
	{
		return int2( (int)(x+0.5f), (int)(y+0.5f));
	}

	float2 rsqrt() const
	{
		return float2( 1.0f / ::sqrtf(x), 1.0f / ::sqrtf(y));
	}

	float2 saturate() const
	{
		return clamp( float2(0, 0), float2(1, 1));
	}

	float2 sign() const
	{
		return float2( (x>0.0f)?1.0f:((x<0.0f)?-1.0f:0.0f), (y>0.0f)?1.0f:((y<0.0f)?-1.0f:0.0f));
	}

	DEFINE_FLOAT2_METHOD(sin, float2)
	DEFINE_FLOAT2_METHOD(sinh, float2)

	float2 smoothstep(const float2& a, const float2& b) const
	{
		float2 r = ((*this - a)/(b - a)).saturate(); 
		return r*r*(float2(3,3) - r*2);
	}

	float2 sqrt() const
	{
		return float2( ::sqrtf(x), ::sqrtf(y));
	}

	float2 step(const float2& o) const
	{
		return float2( (x>=o.x)?1.0f:0.0f, (y>=o.y)?1.0f:0.0f);
	}

	DEFINE_FLOAT2_METHOD(tan, float2)

	int2 trunc() const
	{
		return int2( (int)(x), (int)(y));
	}

	float arg() const
	{
		return ::atan2(y, x);
	}

	float2 clamp(const float2& low, const float2& high) const
	{
		return float2( (x<low.x)?low.x:((x>high.x)?high.x:x), (y<low.y)?low.y:((y>high.y)?high.y:y));
	}

	float2 complexmul(const float2& o) const
	{
		return float2( x * o.x - y * o.y, x * o.y + y * o.x );
	}

	float2 polar() const
	{
		return float2( length(), arg() );
	}

	float2 cartesian() const
	{
		return float2( cosf(y), sinf(y) ) * x;
	}

	static float2 random(float lower=0.0f, float upper=1.0f)
	{
		float range = upper - lower;
		return float2(
			rand() * range / RAND_MAX + lower,
			rand() * range / RAND_MAX + lower);
	}

	static const float2 zero;
	static const float2 xUnit;
	static const float2 yUnit;
	static const float2 one;

};

}}
