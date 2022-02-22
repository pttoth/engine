/*
 * -------------------------------------------------------------------------
 * FILE:    float3.h
 * AUTHOR:  Dr. Szécsi László - szecsi@iit.bme.hu
 * -------------------------------------------------------------------------
 */

#pragma once

#include <math.h>

#define DEFINE_FLOAT3_METHOD(FLOAT3_METHOD_NAME, FLOAT3_METHOD_RETURN_TYPE)	\
	FLOAT3_METHOD_RETURN_TYPE FLOAT3_METHOD_NAME () const	\
	{	\
		return FLOAT3_METHOD_RETURN_TYPE ( :: FLOAT3_METHOD_NAME (x), :: FLOAT3_METHOD_NAME (y), :: FLOAT3_METHOD_NAME (z));	\
	}

#define DEFINE_FLOAT3_METHOD_DIADIC(FLOAT3_METHOD_NAME, FLOAT3_METHOD_RETURN_TYPE)	\
	FLOAT3_METHOD_RETURN_TYPE FLOAT3_METHOD_NAME (const float3& o) const	\
	{	\
		return FLOAT3_METHOD_RETURN_TYPE ( :: FLOAT3_METHOD_NAME (x, o.x), :: FLOAT3_METHOD_NAME (y, o.y), :: FLOAT3_METHOD_NAME (z, o.z));	\
	}

#define DEFINE_FLOAT3_METHOD_TRIADIC(FLOAT3_METHOD_NAME, FLOAT3_METHOD_RETURN_TYPE)	\
	FLOAT3_METHOD_RETURN_TYPE FLOAT3_METHOD_NAME (const float3& a, const float3& b) const	\
	{	\
		return FLOAT3_METHOD_RETURN_TYPE ( :: FLOAT3_METHOD_NAME (x, a.x, b.x), :: FLOAT3_METHOD_NAME (y, a.y, b.y), :: FLOAT3_METHOD_NAME (z, a.z, b.z));	\
	}

namespace pttoth { namespace math {

class float2;
class float3;
class float4;

class float3
{
public:
	union
	{
		struct 
		{
			float x;
			float y;
			float z;
		};

		float v[3];

		float2swizzle<3, float2, 0, 0> xx;
		float2swizzle<3, float2, 0, 1> xy;
		float2swizzle<3, float2, 0, 2> xz;
		float2swizzle<3, float2, 1, 0> yx;
		float2swizzle<3, float2, 1, 1> yy;
		float2swizzle<3, float2, 1, 2> yz;
		float2swizzle<3, float2, 2, 0> zx;
		float2swizzle<3, float2, 2, 1> zy;
		float2swizzle<3, float2, 2, 2> zz;

		float3swizzle<3, float3, 0, 0, 0> xxx;
		float3swizzle<3, float3, 0, 0, 1> xxy;
		float3swizzle<3, float3, 0, 0, 2> xxz;
		float3swizzle<3, float3, 0, 1, 0> xyx;
		float3swizzle<3, float3, 0, 1, 1> xyy;
		float3swizzle<3, float3, 0, 1, 2> xyz;
		float3swizzle<3, float3, 0, 2, 0> xzx;
		float3swizzle<3, float3, 0, 2, 1> xzy;
		float3swizzle<3, float3, 0, 2, 2> xzz;
		float3swizzle<3, float3, 1, 0, 0> yxx;
		float3swizzle<3, float3, 1, 0, 1> yxy;
		float3swizzle<3, float3, 1, 0, 2> yxz;
		float3swizzle<3, float3, 1, 1, 0> yyx;
		float3swizzle<3, float3, 1, 1, 1> yyy;
		float3swizzle<3, float3, 1, 1, 2> yyz;
		float3swizzle<3, float3, 1, 2, 0> yzx;
		float3swizzle<3, float3, 1, 2, 1> yzy;
		float3swizzle<3, float3, 1, 2, 2> yzz;
		float3swizzle<3, float3, 2, 0, 0> zxx;
		float3swizzle<3, float3, 2, 0, 1> zxy;
		float3swizzle<3, float3, 2, 0, 2> zxz;
		float3swizzle<3, float3, 2, 1, 0> zyx;
		float3swizzle<3, float3, 2, 1, 1> zyy;
		float3swizzle<3, float3, 2, 1, 2> zyz;
		float3swizzle<3, float3, 2, 2, 0> zzx;
		float3swizzle<3, float3, 2, 2, 1> zzy;
		float3swizzle<3, float3, 2, 2, 2> zzz;

		float4swizzle<3, float4, 0, 0, 0, 0> xxxx;
		float4swizzle<3, float4, 0, 0, 0, 1> xxxy;
		float4swizzle<3, float4, 0, 0, 0, 2> xxxz;
		float4swizzle<3, float4, 0, 0, 1, 0> xxyx;
		float4swizzle<3, float4, 0, 0, 1, 1> xxyy;
		float4swizzle<3, float4, 0, 0, 1, 2> xxyz;
		float4swizzle<3, float4, 0, 0, 2, 0> xxzx;
		float4swizzle<3, float4, 0, 0, 2, 1> xxzy;
		float4swizzle<3, float4, 0, 0, 2, 2> xxzz;
		float4swizzle<3, float4, 0, 1, 0, 0> xyxx;
		float4swizzle<3, float4, 0, 1, 0, 1> xyxy;
		float4swizzle<3, float4, 0, 1, 0, 2> xyxz;
		float4swizzle<3, float4, 0, 1, 1, 0> xyyx;
		float4swizzle<3, float4, 0, 1, 1, 1> xyyy;
		float4swizzle<3, float4, 0, 1, 1, 2> xyyz;
		float4swizzle<3, float4, 0, 1, 2, 0> xyzx;
		float4swizzle<3, float4, 0, 1, 2, 1> xyzy;
		float4swizzle<3, float4, 0, 1, 2, 2> xyzz;
		float4swizzle<3, float4, 0, 2, 0, 0> xzxx;
		float4swizzle<3, float4, 0, 2, 0, 1> xzxy;
		float4swizzle<3, float4, 0, 2, 0, 2> xzxz;
		float4swizzle<3, float4, 0, 2, 1, 0> xzyx;
		float4swizzle<3, float4, 0, 2, 1, 1> xzyy;
		float4swizzle<3, float4, 0, 2, 1, 2> xzyz;
		float4swizzle<3, float4, 0, 2, 2, 0> xzzx;
		float4swizzle<3, float4, 0, 2, 2, 1> xzzy;
		float4swizzle<3, float4, 0, 2, 2, 2> xzzz;

		float4swizzle<3, float4, 1, 0, 0, 0> yxxx;
		float4swizzle<3, float4, 1, 0, 0, 1> yxxy;
		float4swizzle<3, float4, 1, 0, 0, 2> yxxz;
		float4swizzle<3, float4, 1, 0, 1, 0> yxyx;
		float4swizzle<3, float4, 1, 0, 1, 1> yxyy;
		float4swizzle<3, float4, 1, 0, 1, 2> yxyz;
		float4swizzle<3, float4, 1, 0, 2, 0> yxzx;
		float4swizzle<3, float4, 1, 0, 2, 1> yxzy;
		float4swizzle<3, float4, 1, 0, 2, 2> yxzz;
		float4swizzle<3, float4, 1, 1, 0, 0> yyxx;
		float4swizzle<3, float4, 1, 1, 0, 1> yyxy;
		float4swizzle<3, float4, 1, 1, 0, 2> yyxz;
		float4swizzle<3, float4, 1, 1, 1, 0> yyyx;
		float4swizzle<3, float4, 1, 1, 1, 1> yyyy;
		float4swizzle<3, float4, 1, 1, 1, 2> yyyz;
		float4swizzle<3, float4, 1, 1, 2, 0> yyzx;
		float4swizzle<3, float4, 1, 1, 2, 1> yyzy;
		float4swizzle<3, float4, 1, 1, 2, 2> yyzz;
		float4swizzle<3, float4, 1, 2, 0, 0> yzxx;
		float4swizzle<3, float4, 1, 2, 0, 1> yzxy;
		float4swizzle<3, float4, 1, 2, 0, 2> yzxz;
		float4swizzle<3, float4, 1, 2, 1, 0> yzyx;
		float4swizzle<3, float4, 1, 2, 1, 1> yzyy;
		float4swizzle<3, float4, 1, 2, 1, 2> yzyz;
		float4swizzle<3, float4, 1, 2, 2, 0> yzzx;
		float4swizzle<3, float4, 1, 2, 2, 1> yzzy;
		float4swizzle<3, float4, 1, 2, 2, 2> yzzz;

		float4swizzle<3, float4, 2, 0, 0, 0> zxxx;
		float4swizzle<3, float4, 2, 0, 0, 1> zxxy;
		float4swizzle<3, float4, 2, 0, 0, 2> zxxz;
		float4swizzle<3, float4, 2, 0, 1, 0> zxyx;
		float4swizzle<3, float4, 2, 0, 1, 1> zxyy;
		float4swizzle<3, float4, 2, 0, 1, 2> zxyz;
		float4swizzle<3, float4, 2, 0, 2, 0> zxzx;
		float4swizzle<3, float4, 2, 0, 2, 1> zxzy;
		float4swizzle<3, float4, 2, 0, 2, 2> zxzz;
		float4swizzle<3, float4, 2, 1, 0, 0> zyxx;
		float4swizzle<3, float4, 2, 1, 0, 1> zyxy;
		float4swizzle<3, float4, 2, 1, 0, 2> zyxz;
		float4swizzle<3, float4, 2, 1, 1, 0> zyyx;
		float4swizzle<3, float4, 2, 1, 1, 1> zyyy;
		float4swizzle<3, float4, 2, 1, 1, 2> zyyz;
		float4swizzle<3, float4, 2, 1, 2, 0> zyzx;
		float4swizzle<3, float4, 2, 1, 2, 1> zyzy;
		float4swizzle<3, float4, 2, 1, 2, 2> zyzz;
		float4swizzle<3, float4, 2, 2, 0, 0> zzxx;
		float4swizzle<3, float4, 2, 2, 0, 1> zzxy;
		float4swizzle<3, float4, 2, 2, 0, 2> zzxz;
		float4swizzle<3, float4, 2, 2, 1, 0> zzyx;
		float4swizzle<3, float4, 2, 2, 1, 1> zzyy;
		float4swizzle<3, float4, 2, 2, 1, 2> zzyz;
		float4swizzle<3, float4, 2, 2, 2, 0> zzzx;
		float4swizzle<3, float4, 2, 2, 2, 1> zzzy;
		float4swizzle<3, float4, 2, 2, 2, 2> zzzz;

		float4swizzle<3, float4, 0, 1, 2, -1> xyz0;
		float4swizzle<3, float4, 0, 1, 2, -2> xyz1;
	};

	float3():x(0.0f),y(0.0f),z(0.0f){}

	float3(float f):x(f),y(f),z(f){}

	float3(float x, float y, float z):x(x),y(y),z(z){}

	float3(float x, float y, float z, float w):x(x),y(y),z(z){w;}

	float3(float x, float2 yz):x(x),y(yz.x),z(yz.y){}

	float3(float2 xy, float z):x(xy.x),y(xy.y),z(z){}

	float3(int3 i):x((float)i.x),y((float)i.y),z((float)i.z){}

	float3& operator+=(const float3& o)
	{
		x += o.x;
		y += o.y;
		z += o.z;
		return *this;
	}

	float3& operator-=(const float3& o)
	{
		x -= o.x;
		y -= o.y;
		z -= o.z;
		return *this;
	}

	float3& operator*=(const float3& o)
	{
		x *= o.x;
		y *= o.y;
		z *= o.z;
		return *this;
	}

	float3& operator/=(const float3& o)
	{
		x /= o.x;
		y /= o.y;
		z /= o.z;
		return *this;
	}

	float3& operator%=(const float3& o)
	{
		x = fmodf(x, o.x);
		y = fmodf(y, o.y);
		z = fmodf(z, o.z);
		return *this;
	}

	bool3 operator==(const float3& o) const
	{
		return bool3(x == o.x, y == o.y, z == o.z);
	}

	bool3 operator!=(const float3& o) const
	{
		return bool3(x != o.x, y != o.y, z != o.z);
	}

	bool3 operator<(const float3& o) const
	{
		return bool3(x < o.x, y < o.y, z < o.z);
	}

	bool3 operator<=(const float3& o) const
	{
		return bool3(x <= o.x, y <= o.y, z <= o.z);
	}

	bool3 operator>(const float3& o) const
	{
		return bool3(x > o.x, y > o.y, z > o.z);
	}

	bool3 operator>=(const float3& o) const
	{
		return bool3(x >= o.x, y >= o.y, z >= o.z);
	}

	float3 operator+(const float3& o) const
	{
		return float3(x + o.x, y + o.y, z + o.z);
	}

	float3 operator-(const float3& o) const
	{
		return float3(x - o.x, y - o.y, z - o.z);
	}

	float3 operator*(const float3& o) const
	{
		return float3(x * o.x, y * o.y, z * o.z);
	}

	float3 operator/(const float3& o) const
	{
		return float3(x / o.x, y / o.y, z / o.z);
	}

	float3 operator%(const float3& o) const
	{
		return float3(fmodf(x, o.x), fmodf(y, o.y), fmodf(z, o.z));
	}


	float3 operator+() const
	{
		return float3(+x, +y, +z);
	}

	float3 operator-() const
	{
		return float3(-x, -y, -z);
	}

	float3 operator!() const
	{
		return float3(+x, -y, -z);
	}

	float operator[](unsigned int i) const
	{
		if(i < 0 || i > 2)
			throw std::range_error("float3 index out of range.");
		return v[i];
	}

	float& operator[](unsigned int i)
	{
		if(i < 0 || i > 2)
			throw std::range_error("float3 index out of range.");
		return v[i];
	}

	DEFINE_FLOAT3_METHOD(abs, float3)
	DEFINE_FLOAT3_METHOD(acos, float3)
	DEFINE_FLOAT3_METHOD(asin, float3)
	DEFINE_FLOAT3_METHOD(atan, float3)
	DEFINE_FLOAT3_METHOD(ceil, int3)
	DEFINE_FLOAT3_METHOD(cos, float3)
	DEFINE_FLOAT3_METHOD(cosh, float3)
	DEFINE_FLOAT3_METHOD(exp, float3)
	DEFINE_FLOAT3_METHOD(floor, int3)

	float3 exp2() const
	{
		return float3( ::pow(2.0f, x), ::pow(2.0f, y), ::pow(2.0f, z));
	}

	DEFINE_FLOAT3_METHOD_DIADIC(atan2, float3)
	DEFINE_FLOAT3_METHOD_DIADIC(fmod, float3)

	float distance(const float3& o) const
	{
		return (*this - o).length();
	}

	float dot(const float3& o) const
	{
		return x * o.x + y * o.y + z * o.z;
	}

	float3 frac() const
	{
		return *this - trunc();
	}
/*
	bool3 isfinite() const
	{
		return bool3( ::_finite(x), ::_finite(y), ::_finite(z));
	}

	bool3 isinf() const
	{
		return bool3( !::_finite(x), !::_finite(y), !::_finite(z));
	}

	bool3 isnan() const
	{
		return bool3( ::_isnan(x), ::_isnan(y), ::_isnan(z));
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

	float3 lerp(const float3& a, const float3& b) const
	{
		return a * (float3(1, 1, 1) - *this) + b * *this;
	}

	DEFINE_FLOAT3_METHOD(log, float3)
	DEFINE_FLOAT3_METHOD(log10, float3)

	float3 log2() const
	{
		return float3( ::log(x) / ::log(2.0f), ::log(y) / ::log(2.0f), ::log(z) / ::log(2.0f));
	}

	float3 max(const float3& o) const
	{
		return float3( (x>o.x)?x:o.x, (y>o.y)?y:o.y, (z>o.z)?z:o.z);
	}

	float3 min(const float3& o) const
	{
		return float3( (x<o.x)?x:o.x, (y<o.y)?y:o.y, (z<o.z)?z:o.z);
	}

	float3 normalize() const
	{
		return *this / length();
	}

	DEFINE_FLOAT3_METHOD_DIADIC(pow, float3)

	int3 round() const
	{
		return int3( (int)(x+0.5f), (int)(y+0.5f), (int)(z+0.5f));
	}

	float3 rsqrt() const
	{
		return float3( 1.0f / ::sqrtf(x), 1.0f / ::sqrtf(y), 1.0f / ::sqrtf(z));
	}

	float3 saturate() const
	{
		return clamp( float3(0, 0, 0), float3(1, 1, 1));
	}

	float3 sign() const
	{
		return float3( (x>0.0f)?1.0f:((x<0.0f)?-1.0f:0.0f), (y>0.0f)?1.0f:((y<0.0f)?-1.0f:0.0f), (z>0.0f)?1.0f:((z<0.0f)?-1.0f:0.0f));
	}

	DEFINE_FLOAT3_METHOD(sin, float3)
	DEFINE_FLOAT3_METHOD(sinh, float3)

	float3 smoothstep(const float3& a, const float3& b) const
	{
		float3 r = ((*this - a)/(b - a)).saturate(); 
		return r*r*(float3(3,3,3) - r*2);
	}

	float3 sqrt() const
	{
		return float3( ::sqrtf(x), ::sqrtf(y), ::sqrtf(z));
	}

	float3 step(const float3& o) const
	{
		return float3( (x>=o.x)?1:0, (y>=o.y)?1:0, (z>=o.z)?1:0);
	}

	DEFINE_FLOAT3_METHOD(tan, float3)

	int3 trunc() const
	{
		return int3( (int)(x), (int)(y), (int)(z));
	}

	float3 clamp(const float3& low, const float3& high) const
	{
		return float3( (x<low.x)?low.x:((x>high.x)?high.x:x), (y<low.y)?low.y:((y>high.y)?high.y:y), (z<low.z)?low.z:((z>high.z)?high.z:z));
	}

	float3 cross(const float3& o) const
	{
		return float3(y * o.z - z * o.y, z * o.x - x * o.z, x * o.y - y * o.x);
	}

	static float3 random(float lower=0.0f, float upper=1.0f)
	{
		float range = upper - lower;
		return float3(
			rand() * range / RAND_MAX + lower,
			rand() * range / RAND_MAX + lower,
			rand() * range / RAND_MAX + lower);
	}


	static const float3 zero;
	static const float3 xUnit;
	static const float3 yUnit;
	static const float3 zUnit;
	static const float3 one;

    static const float3 black				;
    static const float3 navy				;
    static const float3 blue				;
    static const float3 darkGreen			;
    static const float3 teal				;
    static const float3 slateBlue			;
    static const float3 green				;
    static const float3 cyan				;
    static const float3 maroon				;
    static const float3 purple				;
    static const float3 azure				;
    static const float3 olive				;
    static const float3 gray				;
    static const float3 cornflower			;
    static const float3 aquamarine			;
    static const float3 red					;
    static const float3 deepPink			;
    static const float3 magenta				;
    static const float3 orange				;
    static const float3 coral				;
    static const float3 mallow				;
    static const float3 yellow				;
    static const float3 gold				;
    static const float3 white				;
    static const float3 silver				;
};

}}
