/*
 * -------------------------------------------------------------------------
 * FILE:    bool3.h
 * AUTHOR:  Dr. Szécsi László - szecsi@iit.bme.hu
 * -------------------------------------------------------------------------
 */
#pragma once

#include <math.h>

namespace pttoth { namespace math {

class bool2;
class bool4;

class bool3
{
public:
	union{
		struct {
			bool x;
			bool y;
			bool z;
		};

		bool v[3];

		boolswizzle<3, bool2, 0, 0> xx;
		boolswizzle<3, bool2, 0, 1> xy;
		boolswizzle<3, bool2, 0, 2> xz;
		boolswizzle<3, bool2, 1, 0> yx;
		boolswizzle<3, bool2, 1, 1> yy;
		boolswizzle<3, bool2, 1, 2> yz;
		boolswizzle<3, bool2, 2, 0> zx;
		boolswizzle<3, bool2, 2, 1> zy;
		boolswizzle<3, bool2, 2, 2> zz;

		boolswizzle<3, bool3, 0, 0, 0> xxx;
		boolswizzle<3, bool3, 0, 0, 1> xxy;
		boolswizzle<3, bool3, 0, 0, 2> xxz;
		boolswizzle<3, bool3, 0, 1, 0> xyx;
		boolswizzle<3, bool3, 0, 1, 1> xyy;
		boolswizzle<3, bool3, 0, 1, 2> xyz;
		boolswizzle<3, bool3, 0, 2, 0> xzx;
		boolswizzle<3, bool3, 0, 2, 1> xzy;
		boolswizzle<3, bool3, 0, 2, 2> xzz;
		boolswizzle<3, bool3, 1, 0, 0> yxx;
		boolswizzle<3, bool3, 1, 0, 1> yxy;
		boolswizzle<3, bool3, 1, 0, 2> yxz;
		boolswizzle<3, bool3, 1, 1, 0> yyx;
		boolswizzle<3, bool3, 1, 1, 1> yyy;
		boolswizzle<3, bool3, 1, 1, 2> yyz;
		boolswizzle<3, bool3, 1, 2, 0> yzx;
		boolswizzle<3, bool3, 1, 2, 1> yzy;
		boolswizzle<3, bool3, 1, 2, 2> yzz;
		boolswizzle<3, bool3, 2, 0, 0> zxx;
		boolswizzle<3, bool3, 2, 0, 1> zxy;
		boolswizzle<3, bool3, 2, 0, 2> zxz;
		boolswizzle<3, bool3, 2, 1, 0> zyx;
		boolswizzle<3, bool3, 2, 1, 1> zyy;
		boolswizzle<3, bool3, 2, 1, 2> zyz;
		boolswizzle<3, bool3, 2, 2, 0> zzx;
		boolswizzle<3, bool3, 2, 2, 1> zzy;
		boolswizzle<3, bool3, 2, 2, 2> zzz;

		boolswizzle<3, bool4, 0, 0, 0, 0> xxxx;
		boolswizzle<3, bool4, 0, 0, 0, 1> xxxy;
		boolswizzle<3, bool4, 0, 0, 0, 2> xxxz;
		boolswizzle<3, bool4, 0, 0, 1, 0> xxyx;
		boolswizzle<3, bool4, 0, 0, 1, 1> xxyy;
		boolswizzle<3, bool4, 0, 0, 1, 2> xxyz;
		boolswizzle<3, bool4, 0, 0, 2, 0> xxzx;
		boolswizzle<3, bool4, 0, 0, 2, 1> xxzy;
		boolswizzle<3, bool4, 0, 0, 2, 2> xxzz;
		boolswizzle<3, bool4, 0, 1, 0, 0> xyxx;
		boolswizzle<3, bool4, 0, 1, 0, 1> xyxy;
		boolswizzle<3, bool4, 0, 1, 0, 2> xyxz;
		boolswizzle<3, bool4, 0, 1, 1, 0> xyyx;
		boolswizzle<3, bool4, 0, 1, 1, 1> xyyy;
		boolswizzle<3, bool4, 0, 1, 1, 2> xyyz;
		boolswizzle<3, bool4, 0, 1, 2, 0> xyzx;
		boolswizzle<3, bool4, 0, 1, 2, 1> xyzy;
		boolswizzle<3, bool4, 0, 1, 2, 2> xyzz;
		boolswizzle<3, bool4, 0, 2, 0, 0> xzxx;
		boolswizzle<3, bool4, 0, 2, 0, 1> xzxy;
		boolswizzle<3, bool4, 0, 2, 0, 2> xzxz;
		boolswizzle<3, bool4, 0, 2, 1, 0> xzyx;
		boolswizzle<3, bool4, 0, 2, 1, 1> xzyy;
		boolswizzle<3, bool4, 0, 2, 1, 2> xzyz;
		boolswizzle<3, bool4, 0, 2, 2, 0> xzzx;
		boolswizzle<3, bool4, 0, 2, 2, 1> xzzy;
		boolswizzle<3, bool4, 0, 2, 2, 2> xzzz;

		boolswizzle<3, bool4, 1, 0, 0, 0> yxxx;
		boolswizzle<3, bool4, 1, 0, 0, 1> yxxy;
		boolswizzle<3, bool4, 1, 0, 0, 2> yxxz;
		boolswizzle<3, bool4, 1, 0, 1, 0> yxyx;
		boolswizzle<3, bool4, 1, 0, 1, 1> yxyy;
		boolswizzle<3, bool4, 1, 0, 1, 2> yxyz;
		boolswizzle<3, bool4, 1, 0, 2, 0> yxzx;
		boolswizzle<3, bool4, 1, 0, 2, 1> yxzy;
		boolswizzle<3, bool4, 1, 0, 2, 2> yxzz;
		boolswizzle<3, bool4, 1, 1, 0, 0> yyxx;
		boolswizzle<3, bool4, 1, 1, 0, 1> yyxy;
		boolswizzle<3, bool4, 1, 1, 0, 2> yyxz;
		boolswizzle<3, bool4, 1, 1, 1, 0> yyyx;
		boolswizzle<3, bool4, 1, 1, 1, 1> yyyy;
		boolswizzle<3, bool4, 1, 1, 1, 2> yyyz;
		boolswizzle<3, bool4, 1, 1, 2, 0> yyzx;
		boolswizzle<3, bool4, 1, 1, 2, 1> yyzy;
		boolswizzle<3, bool4, 1, 1, 2, 2> yyzz;
		boolswizzle<3, bool4, 1, 2, 0, 0> yzxx;
		boolswizzle<3, bool4, 1, 2, 0, 1> yzxy;
		boolswizzle<3, bool4, 1, 2, 0, 2> yzxz;
		boolswizzle<3, bool4, 1, 2, 1, 0> yzyx;
		boolswizzle<3, bool4, 1, 2, 1, 1> yzyy;
		boolswizzle<3, bool4, 1, 2, 1, 2> yzyz;
		boolswizzle<3, bool4, 1, 2, 2, 0> yzzx;
		boolswizzle<3, bool4, 1, 2, 2, 1> yzzy;
		boolswizzle<3, bool4, 1, 2, 2, 2> yzzz;

		boolswizzle<3, bool4, 2, 0, 0, 0> zxxx;
		boolswizzle<3, bool4, 2, 0, 0, 1> zxxy;
		boolswizzle<3, bool4, 2, 0, 0, 2> zxxz;
		boolswizzle<3, bool4, 2, 0, 1, 0> zxyx;
		boolswizzle<3, bool4, 2, 0, 1, 1> zxyy;
		boolswizzle<3, bool4, 2, 0, 1, 2> zxyz;
		boolswizzle<3, bool4, 2, 0, 2, 0> zxzx;
		boolswizzle<3, bool4, 2, 0, 2, 1> zxzy;
		boolswizzle<3, bool4, 2, 0, 2, 2> zxzz;
		boolswizzle<3, bool4, 2, 1, 0, 0> zyxx;
		boolswizzle<3, bool4, 2, 1, 0, 1> zyxy;
		boolswizzle<3, bool4, 2, 1, 0, 2> zyxz;
		boolswizzle<3, bool4, 2, 1, 1, 0> zyyx;
		boolswizzle<3, bool4, 2, 1, 1, 1> zyyy;
		boolswizzle<3, bool4, 2, 1, 1, 2> zyyz;
		boolswizzle<3, bool4, 2, 1, 2, 0> zyzx;
		boolswizzle<3, bool4, 2, 1, 2, 1> zyzy;
		boolswizzle<3, bool4, 2, 1, 2, 2> zyzz;
		boolswizzle<3, bool4, 2, 2, 0, 0> zzxx;
		boolswizzle<3, bool4, 2, 2, 0, 1> zzxy;
		boolswizzle<3, bool4, 2, 2, 0, 2> zzxz;
		boolswizzle<3, bool4, 2, 2, 1, 0> zzyx;
		boolswizzle<3, bool4, 2, 2, 1, 1> zzyy;
		boolswizzle<3, bool4, 2, 2, 1, 2> zzyz;
		boolswizzle<3, bool4, 2, 2, 2, 0> zzzx;
		boolswizzle<3, bool4, 2, 2, 2, 1> zzzy;
		boolswizzle<3, bool4, 2, 2, 2, 2> zzzz;

	};

	bool3():x(false),y(false),z(false){}

	bool3(bool b):x(b),y(b),z(b){}

	bool3(bool x, bool y, bool z):x(x),y(y),z(z){}

	bool3(bool x, bool y, bool z, bool w):x(x),y(y),z(z){w;}

	bool3(bool x, bool2 yz):x(x),y(yz.x),z(yz.y){}

	bool3(bool2 xy, bool z):x(xy.x),y(xy.y),z(z){}

	bool3& operator&=(const bool3& o)
	{
		x = x && o.x;
		y = y && o.y;
		z = z && o.z;
		return *this;
	}

	bool3& operator|=(const bool3& o)
	{
		x = x || o.x;
		y = y || o.y;
		z = z || o.z;
		return *this;
	}

	bool3 operator&	(const bool3& o) const
	{
		return bool3(x && o.x, y && o.y, z && o.z);
	}

	bool3 operator&&(const bool3& o) const
	{
		return bool3(x && o.x, y && o.y, z && o.z);
	}

	bool3 operator|	(const bool3& o) const
	{
		return bool3(x || o.x, y || o.y, z || o.z);
	}

	bool3 operator||(const bool3& o) const
	{
		return bool3(x || o.x, y || o.y, z || o.z);
	}

	bool3 operator==(const bool3& o) const
	{
		return bool3(x == o.x, y == o.y, z == o.z);
	}

	bool3 operator!=(const bool3& o) const
	{
		return bool3(x != o.x, y != o.y, z != o.z);
	}

	bool3 operator<=(const bool3& o) const
	{
		return bool3(x <= o.x, y <= o.y, z <= o.z);
	}

	bool3 operator>=(const bool3& o) const
	{
		return bool3(x >= o.x, y >= o.y, z >= o.z);
	}

	bool3 operator!() const
	{
		return bool3(!x, !y, !z);
	}

	bool operator[](unsigned int i) const
	{
		if(i < 0 || i > 2)
			throw std::range_error("bool3 index out of range.");
		return v[i];
	}

	bool& operator[](unsigned int i)
	{
		if(i < 0 || i > 2)
			throw std::range_error("bool3 index out of range.");
		return v[i];
	}

	bool any() const
	{
		return x || y || z;
	}

	bool all() const
	{
		return x && y && z;
	}

	static bool3 random()
	{
		return bool3(rand()%2==0, rand()%2==0, rand()%2==0);
	}

	static const bool3 zero;
	static const bool3 xUnit;
	static const bool3 yUnit;
	static const bool3 zUnit;
	static const bool3 one;
};

}}
