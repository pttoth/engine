/*
 * -------------------------------------------------------------------------
 * FILE:    int2.h
 * AUTHOR:  Dr. Szécsi László - szecsi@iit.bme.hu
 * -------------------------------------------------------------------------
 */
#pragma once

#include <math.h>
#include <stdexcept>

namespace pttoth { namespace math {

class int3;
class int4;

class int2
{
public:
	union{
		struct {
			int x;
			int y;
		};

		int v[2];

		intswizzle<2, int2, bool2, 0, 0> xx;
		intswizzle<2, int2, bool2, 0, 1> xy;
		intswizzle<2, int2, bool2, 1, 0> yx;
		intswizzle<2, int2, bool2, 1, 1> yy;

		intswizzle<2, int3, bool3, 0, 0, 0> xxx;
		intswizzle<2, int3, bool3, 0, 0, 1> xxy;
		intswizzle<2, int3, bool3, 0, 1, 0> xyx;
		intswizzle<2, int3, bool3, 0, 1, 1> xyy;
		intswizzle<2, int3, bool3, 1, 0, 0> yxx;
		intswizzle<2, int3, bool3, 1, 0, 1> yxy;
		intswizzle<2, int3, bool3, 1, 1, 0> yyx;
		intswizzle<2, int3, bool3, 1, 1, 1> yyy;

		intswizzle<2, int4, bool4, 0, 0, 0, 0> xxxx;
		intswizzle<2, int4, bool4, 0, 0, 0, 1> xxxy;
		intswizzle<2, int4, bool4, 0, 0, 1, 0> xxyx;
		intswizzle<2, int4, bool4, 0, 0, 1, 1> xxyy;
		intswizzle<2, int4, bool4, 0, 1, 0, 0> xyxx;
		intswizzle<2, int4, bool4, 0, 1, 0, 1> xyxy;
		intswizzle<2, int4, bool4, 0, 1, 1, 0> xyyx;
		intswizzle<2, int4, bool4, 0, 1, 1, 1> xyyy;
		intswizzle<2, int4, bool4, 1, 0, 0, 0> yxxx;
		intswizzle<2, int4, bool4, 1, 0, 0, 1> yxxy;
		intswizzle<2, int4, bool4, 1, 0, 1, 0> yxyx;
		intswizzle<2, int4, bool4, 1, 0, 1, 1> yxyy;
		intswizzle<2, int4, bool4, 1, 1, 0, 0> yyxx;
		intswizzle<2, int4, bool4, 1, 1, 0, 1> yyxy;
		intswizzle<2, int4, bool4, 1, 1, 1, 0> yyyx;
		intswizzle<2, int4, bool4, 1, 1, 1, 1> yyyy;

	};

	int2():x(0),y(0){}

	int2(int i):x(i),y(i){}

	int2(int x, int y):x(x),y(y){}

	int2(int x, int y, int z, int w):x(x),y(y){z; w;}

	int2(bool2 b):x(b.x),y(b.y){}

	int2& operator+=(const int2& o)
	{
		x += o.x;
		y += o.y;
		return *this;
	}

	int2& operator-=(const int2& o)
	{
		x -= o.x;
		y -= o.y;
		return *this;
	}

	int2& operator*=(const int2& o)
	{
		x *= o.x;
		y *= o.y;
		return *this;
	}

	int2& operator/=(const int2& o)
	{
		x /= o.x;
		y /= o.y;
		return *this;
	}

	int2& operator%=(const int2& o)
	{
		x %= o.x;
		y %= o.y;
		return *this;
	}

	int2& operator>>=(const int2& o)
	{
		x >>= o.x;
		y >>= o.y;
		return *this;
	}

	int2& operator<<=(const int2& o)
	{
		x <<= o.x;
		y <<= o.y;
		return *this;
	}

	int2& operator&=(const int2& o)
	{
		x &= o.x;
		y &= o.y;
		return *this;
	}

	int2& operator|=(const int2& o)
	{
		x |= o.x;
		y |= o.y;
		return *this;
	}

	int2 operator&(const int2& o) const
	{
		return int2(x & o.x, y & o.y);
	}

	int2 operator&&(const int2& o) const
	{
		return int2(x && o.x, y && o.y);
	}

	int2 operator|	(const int2& o) const
	{
		return int2(x | o.x, y | o.y);
	}

	int2 operator||(const int2& o) const
	{
		return int2(x || o.x, y || o.y);
	}

	bool2 operator==(const int2& o) const
	{
		return bool2(x == o.x, y == o.y);
	}

	bool2 operator!=(const int2& o) const
	{
		return bool2(x != o.x, y != o.y);
	}

	bool2 operator<(const int2& o) const
	{
		return bool2(x < o.x, y < o.y);
	}

	bool2 operator>(const int2& o) const
	{
		return bool2(x > o.x, y > o.y);
	}

	bool2 operator<=(const int2& o) const
	{
		return bool2(x <= o.x, y <= o.y);
	}

	bool2 operator>=(const int2& o) const
	{
		return bool2(x >= o.x, y >= o.y);
	}

	int2 operator<<(const int2& o) const
	{
		return int2(x << o.x, y << o.y);
	}

	int2 operator>>(const int2& o) const
	{
		return int2(x >> o.x, y >> o.y);
	}

	int2 operator+(const int2& o) const
	{
		return int2(x + o.x, y + o.y);
	}

	int2 operator-(const int2& o) const
	{
		return int2(x - o.x, y - o.y);
	}

	int2 operator*(const int2& o) const
	{
		return int2(x * o.x, y * o.y);
	}

	int2 operator/(const int2& o) const
	{
		return int2(x / o.x, y / o.y);
	}

	int2 operator%(const int2& o) const
	{
		return int2(x % o.x, y % o.y);
	}

	int2 operator+() const
	{
		return int2(+x, +y);
	}

	int2 operator-() const
	{
		return int2(-x, -y);
	}

	int2 operator!() const
	{
		return int2(!x, !y);
	}

	int2 operator~() const
	{
		return int2(~x, ~y);
	}

	int2 operator++()
	{
		return int2(++x, ++y);
	}

	int2 operator--()
	{
		return int2(--x, --y);
	}


	int2 operator++(int)
	{
		return int2(x++, y++);
	}
	
	int2 operator--(int)
	{
		return int2(x++, y++);
	}


	int operator[](unsigned int i) const
	{
		if(i < 0 || i > 1)
			throw std::range_error("int2 index out of range.");
		return v[i];
	}

	int& operator[](unsigned int i)
	{
		if(i < 0 || i > 1)
			throw std::range_error("int2 index out of range.");
		return v[i];
	}

	int2 max(const int2& o) const
	{
		return int2( (x>o.x)?x:o.x, (y>o.y)?y:o.y );
	}

	int2 min(const int2& o) const
	{
		return int2( (x<o.x)?x:o.x, (y<o.y)?y:o.y );
	}

	static int2 random(int lower=0, int upper=6)
	{
		int range = upper - lower + 1;
		return int2(
			rand() % range + lower,
			rand() % range + lower);
	}

	static const int2 zero;
	static const int2 xUnit;
	static const int2 yUnit;
	static const int2 one;

};

}}
