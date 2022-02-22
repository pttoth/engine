/*
 * -------------------------------------------------------------------------
 * FILE:    int3.h
 * AUTHOR:  Dr. Szécsi László - szecsi@iit.bme.hu
 * -------------------------------------------------------------------------
 */
#pragma once

#include <math.h>

namespace pttoth { namespace math {

class int2;
class int4;

class int3
{
public:
	union{
		struct {
			int x;
			int y;
			int z;
		};

		int v[3];

		intswizzle<3, int2, bool2, 0, 0> xx;
		intswizzle<3, int2, bool2, 0, 1> xy;
		intswizzle<3, int2, bool2, 0, 2> xz;
		intswizzle<3, int2, bool2, 1, 0> yx;
		intswizzle<3, int2, bool2, 1, 1> yy;
		intswizzle<3, int2, bool2, 1, 2> yz;
		intswizzle<3, int2, bool2, 2, 0> zx;
		intswizzle<3, int2, bool2, 2, 1> zy;
		intswizzle<3, int2, bool2, 2, 2> zz;

		intswizzle<3, int3, bool3, 0, 0, 0> xxx;
		intswizzle<3, int3, bool3, 0, 0, 1> xxy;
		intswizzle<3, int3, bool3, 0, 0, 2> xxz;
		intswizzle<3, int3, bool3, 0, 1, 0> xyx;
		intswizzle<3, int3, bool3, 0, 1, 1> xyy;
		intswizzle<3, int3, bool3, 0, 1, 2> xyz;
		intswizzle<3, int3, bool3, 0, 2, 0> xzx;
		intswizzle<3, int3, bool3, 0, 2, 1> xzy;
		intswizzle<3, int3, bool3, 0, 2, 2> xzz;
		intswizzle<3, int3, bool3, 1, 0, 0> yxx;
		intswizzle<3, int3, bool3, 1, 0, 1> yxy;
		intswizzle<3, int3, bool3, 1, 0, 2> yxz;
		intswizzle<3, int3, bool3, 1, 1, 0> yyx;
		intswizzle<3, int3, bool3, 1, 1, 1> yyy;
		intswizzle<3, int3, bool3, 1, 1, 2> yyz;
		intswizzle<3, int3, bool3, 1, 2, 0> yzx;
		intswizzle<3, int3, bool3, 1, 2, 1> yzy;
		intswizzle<3, int3, bool3, 1, 2, 2> yzz;
		intswizzle<3, int3, bool3, 2, 0, 0> zxx;
		intswizzle<3, int3, bool3, 2, 0, 1> zxy;
		intswizzle<3, int3, bool3, 2, 0, 2> zxz;
		intswizzle<3, int3, bool3, 2, 1, 0> zyx;
		intswizzle<3, int3, bool3, 2, 1, 1> zyy;
		intswizzle<3, int3, bool3, 2, 1, 2> zyz;
		intswizzle<3, int3, bool3, 2, 2, 0> zzx;
		intswizzle<3, int3, bool3, 2, 2, 1> zzy;
		intswizzle<3, int3, bool3, 2, 2, 2> zzz;

		intswizzle<3, int4, bool4, 0, 0, 0, 0> xxxx;
		intswizzle<3, int4, bool4, 0, 0, 0, 1> xxxy;
		intswizzle<3, int4, bool4, 0, 0, 0, 2> xxxz;
		intswizzle<3, int4, bool4, 0, 0, 1, 0> xxyx;
		intswizzle<3, int4, bool4, 0, 0, 1, 1> xxyy;
		intswizzle<3, int4, bool4, 0, 0, 1, 2> xxyz;
		intswizzle<3, int4, bool4, 0, 0, 2, 0> xxzx;
		intswizzle<3, int4, bool4, 0, 0, 2, 1> xxzy;
		intswizzle<3, int4, bool4, 0, 0, 2, 2> xxzz;
		intswizzle<3, int4, bool4, 0, 1, 0, 0> xyxx;
		intswizzle<3, int4, bool4, 0, 1, 0, 1> xyxy;
		intswizzle<3, int4, bool4, 0, 1, 0, 2> xyxz;
		intswizzle<3, int4, bool4, 0, 1, 1, 0> xyyx;
		intswizzle<3, int4, bool4, 0, 1, 1, 1> xyyy;
		intswizzle<3, int4, bool4, 0, 1, 1, 2> xyyz;
		intswizzle<3, int4, bool4, 0, 1, 2, 0> xyzx;
		intswizzle<3, int4, bool4, 0, 1, 2, 1> xyzy;
		intswizzle<3, int4, bool4, 0, 1, 2, 2> xyzz;
		intswizzle<3, int4, bool4, 0, 2, 0, 0> xzxx;
		intswizzle<3, int4, bool4, 0, 2, 0, 1> xzxy;
		intswizzle<3, int4, bool4, 0, 2, 0, 2> xzxz;
		intswizzle<3, int4, bool4, 0, 2, 1, 0> xzyx;
		intswizzle<3, int4, bool4, 0, 2, 1, 1> xzyy;
		intswizzle<3, int4, bool4, 0, 2, 1, 2> xzyz;
		intswizzle<3, int4, bool4, 0, 2, 2, 0> xzzx;
		intswizzle<3, int4, bool4, 0, 2, 2, 1> xzzy;
		intswizzle<3, int4, bool4, 0, 2, 2, 2> xzzz;

		intswizzle<3, int4, bool4, 1, 0, 0, 0> yxxx;
		intswizzle<3, int4, bool4, 1, 0, 0, 1> yxxy;
		intswizzle<3, int4, bool4, 1, 0, 0, 2> yxxz;
		intswizzle<3, int4, bool4, 1, 0, 1, 0> yxyx;
		intswizzle<3, int4, bool4, 1, 0, 1, 1> yxyy;
		intswizzle<3, int4, bool4, 1, 0, 1, 2> yxyz;
		intswizzle<3, int4, bool4, 1, 0, 2, 0> yxzx;
		intswizzle<3, int4, bool4, 1, 0, 2, 1> yxzy;
		intswizzle<3, int4, bool4, 1, 0, 2, 2> yxzz;
		intswizzle<3, int4, bool4, 1, 1, 0, 0> yyxx;
		intswizzle<3, int4, bool4, 1, 1, 0, 1> yyxy;
		intswizzle<3, int4, bool4, 1, 1, 0, 2> yyxz;
		intswizzle<3, int4, bool4, 1, 1, 1, 0> yyyx;
		intswizzle<3, int4, bool4, 1, 1, 1, 1> yyyy;
		intswizzle<3, int4, bool4, 1, 1, 1, 2> yyyz;
		intswizzle<3, int4, bool4, 1, 1, 2, 0> yyzx;
		intswizzle<3, int4, bool4, 1, 1, 2, 1> yyzy;
		intswizzle<3, int4, bool4, 1, 1, 2, 2> yyzz;
		intswizzle<3, int4, bool4, 1, 2, 0, 0> yzxx;
		intswizzle<3, int4, bool4, 1, 2, 0, 1> yzxy;
		intswizzle<3, int4, bool4, 1, 2, 0, 2> yzxz;
		intswizzle<3, int4, bool4, 1, 2, 1, 0> yzyx;
		intswizzle<3, int4, bool4, 1, 2, 1, 1> yzyy;
		intswizzle<3, int4, bool4, 1, 2, 1, 2> yzyz;
		intswizzle<3, int4, bool4, 1, 2, 2, 0> yzzx;
		intswizzle<3, int4, bool4, 1, 2, 2, 1> yzzy;
		intswizzle<3, int4, bool4, 1, 2, 2, 2> yzzz;

		intswizzle<3, int4, bool4, 2, 0, 0, 0> zxxx;
		intswizzle<3, int4, bool4, 2, 0, 0, 1> zxxy;
		intswizzle<3, int4, bool4, 2, 0, 0, 2> zxxz;
		intswizzle<3, int4, bool4, 2, 0, 1, 0> zxyx;
		intswizzle<3, int4, bool4, 2, 0, 1, 1> zxyy;
		intswizzle<3, int4, bool4, 2, 0, 1, 2> zxyz;
		intswizzle<3, int4, bool4, 2, 0, 2, 0> zxzx;
		intswizzle<3, int4, bool4, 2, 0, 2, 1> zxzy;
		intswizzle<3, int4, bool4, 2, 0, 2, 2> zxzz;
		intswizzle<3, int4, bool4, 2, 1, 0, 0> zyxx;
		intswizzle<3, int4, bool4, 2, 1, 0, 1> zyxy;
		intswizzle<3, int4, bool4, 2, 1, 0, 2> zyxz;
		intswizzle<3, int4, bool4, 2, 1, 1, 0> zyyx;
		intswizzle<3, int4, bool4, 2, 1, 1, 1> zyyy;
		intswizzle<3, int4, bool4, 2, 1, 1, 2> zyyz;
		intswizzle<3, int4, bool4, 2, 1, 2, 0> zyzx;
		intswizzle<3, int4, bool4, 2, 1, 2, 1> zyzy;
		intswizzle<3, int4, bool4, 2, 1, 2, 2> zyzz;
		intswizzle<3, int4, bool4, 2, 2, 0, 0> zzxx;
		intswizzle<3, int4, bool4, 2, 2, 0, 1> zzxy;
		intswizzle<3, int4, bool4, 2, 2, 0, 2> zzxz;
		intswizzle<3, int4, bool4, 2, 2, 1, 0> zzyx;
		intswizzle<3, int4, bool4, 2, 2, 1, 1> zzyy;
		intswizzle<3, int4, bool4, 2, 2, 1, 2> zzyz;
		intswizzle<3, int4, bool4, 2, 2, 2, 0> zzzx;
		intswizzle<3, int4, bool4, 2, 2, 2, 1> zzzy;
		intswizzle<3, int4, bool4, 2, 2, 2, 2> zzzz;

	};

	int3():x(0),y(0),z(0){}

	int3(int i):x(i),y(i),z(i){}

	int3(int x, int y, int z):x(x),y(y),z(z){}

	int3(int x, int y, int z, int w):x(x),y(y),z(z){w;}

	int3(int x, int2 yz):x(x),y(yz.x),z(yz.y){}

	int3(int2 xy, int z):x(xy.x),y(xy.y),z(z){}

	int3(bool3 b):x(b.x),y(b.y),z(b.z){}

	int3& operator+=(const int3& o)
	{
		x += o.x;
		y += o.y;
		z += o.z;
		return *this;
	}

	int3& operator-=(const int3& o)
	{
		x -= o.x;
		y -= o.y;
		z -= o.z;
		return *this;
	}

	int3& operator*=(const int3& o)
	{
		x *= o.x;
		y *= o.y;
		z *= o.z;
		return *this;
	}

	int3& operator/=(const int3& o)
	{
		x /= o.x;
		y /= o.y;
		z /= o.z;
		return *this;
	}

	int3& operator%=(const int3& o)
	{
		x %= o.x;
		y %= o.y;
		z %= o.z;
		return *this;
	}

	int3& operator>>=(const int3& o)
	{
		x >>= o.x;
		y >>= o.y;
		z >>= o.z;
		return *this;
	}

	int3& operator<<=(const int3& o)
	{
		x <<= o.x;
		y <<= o.y;
		z <<= o.z;
		return *this;
	}

	int3& operator&=(const int3& o)
	{
		x &= o.x;
		y &= o.y;
		z &= o.z;
		return *this;
	}

	int3& operator|=(const int3& o)
	{
		x |= o.x;
		y |= o.y;
		z |= o.z;
		return *this;
	}

	int3 operator&(const int3& o) const
	{
		return int3(x & o.x, y & o.y, z & o.z);
	}

	int3 operator&&(const int3& o) const
	{
		return int3(x && o.x, y && o.y, z && o.z);
	}

	int3 operator|	(const int3& o) const
	{
		return int3(x | o.x, y | o.y, z | o.z);
	}

	int3 operator||(const int3& o) const
	{
		return int3(x || o.x, y || o.y, z || o.z);
	}

	bool3 operator==(const int3& o) const
	{
		return bool3(x == o.x, y == o.y, z == o.z);
	}

	bool3 operator!=(const int3& o) const
	{
		return bool3(x != o.x, y != o.y, z != o.z);
	}

	bool3 operator<(const int3& o) const
	{
		return bool3(x < o.x, y < o.y, z < o.z);
	}

	bool3 operator>(const int3& o) const
	{
		return bool3(x > o.x, y > o.y, z > o.z);
	}

	bool3 operator<=(const int3& o) const
	{
		return bool3(x <= o.x, y <= o.y, z <= o.z);
	}

	bool3 operator>=(const int3& o) const
	{
		return bool3(x >= o.x, y >= o.y, z >= o.z);
	}

	int3 operator<<(const int3& o) const
	{
		return int3(x << o.x, y << o.y, z << o.z);
	}

	int3 operator>>(const int3& o) const
	{
		return int3(x >> o.x, y >> o.y, z >> o.z);
	}

	int3 operator+(const int3& o) const
	{
		return int3(x + o.x, y + o.y, z + o.z);
	}

	int3 operator-(const int3& o) const
	{
		return int3(x - o.x, y - o.y, z - o.z);
	}

	int3 operator*(const int3& o) const
	{
		return int3(x * o.x, y * o.y, z * o.z);
	}

	int3 operator/(const int3& o) const
	{
		return int3(x / o.x, y / o.y, z / o.z);
	}

	int3 operator%(const int3& o) const
	{
		return int3(x % o.x, y % o.y, z % o.z);
	}

	int3 operator+() const
	{
		return int3(+x, +y, +z);
	}

	int3 operator-() const
	{
		return int3(-x, -y, -z);
	}

	int3 operator!() const
	{
		return int3(!x, !y, !z);
	}

	int3 operator~() const
	{
		return int3(~x, ~y, ~z);
	}

	int3 operator++()
	{
		return int3(++x, ++y, ++z);
	}

	int3 operator--()
	{
		return int3(--x, --y, --z);
	}


	int3 operator++(int)
	{
		return int3(x++, y++, z++);
	}
	
	int3 operator--(int)
	{
		return int3(x++, y++, z++);
	}


	int operator[](unsigned int i) const
	{
		if(i < 0 || i > 2)
			throw std::range_error("int3 index out of range.");
		return v[i];
	}

	int& operator[](unsigned int i)
	{
		if(i < 0 || i > 2)
			throw std::range_error("int3 index out of range.");
		return v[i];
	}

	int3 max(const int3& o) const
	{
		return int3( (x>o.x)?x:o.x, (y>o.y)?y:o.y, (z>o.z)?z:o.z );
	}

	int3 min(const int3& o) const
	{
		return int3( (x<o.x)?x:o.x, (y<o.y)?y:o.y, (z<o.z)?z:o.z );
	}

	static int3 random(int lower=0, int upper=6)
	{
		int range = upper - lower + 1;
		return int3(
			rand() % range + lower,
			rand() % range + lower,
			rand() % range + lower);
	}

	static const int3 zero;
	static const int3 xUnit;
	static const int3 yUnit;
	static const int3 zUnit;
	static const int3 one;
};

}}
