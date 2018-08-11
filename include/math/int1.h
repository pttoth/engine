/*
 * -------------------------------------------------------------------------
 * FILE:    int1.h
 * AUTHOR:  Dr. Szécsi László - szecsi@iit.bme.hu
 * -------------------------------------------------------------------------
 */

#pragma once

#include <math.h>
#include <stdexcept>

namespace pttoth { namespace math {

class int2;
class int3;
class int4;

class int1
{
public:
	union{
		struct {
			int x;
		};

		int v[1];

		intswizzle<1, int2, bool2, 0, 0> xx;
		intswizzle<1, int3, bool3, 0, 0, 0> xxx;
		intswizzle<1, int4, bool4, 0, 0, 0, 0> xxxx;

	};

	operator int() const
	{
		return x;
	}

	int1():x(0){}

	int1(int i):x(i){}

	int1(int x, int y, int z, int w):x(x){y; z; w;}

	int1(bool1 b):x(b.x){}

	int1& operator+=(const int1& o)
	{
		x += o.x;
		return *this;
	}

	int1& operator-=(const int1& o)
	{
		x -= o.x;
		return *this;
	}

	int1& operator*=(const int1& o)
	{
		x *= o.x;
		return *this;
	}

	int1& operator/=(const int1& o)
	{
		x /= o.x;
		return *this;
	}

	int1& operator%=(const int1& o)
	{
		x %= o.x;
		return *this;
	}

	int1& operator>>=(const int1& o)
	{
		x >>= o.x;
		return *this;
	}

	int1& operator<<=(const int1& o)
	{
		x <<= o.x;
		return *this;
	}

	int1& operator&=(const int1& o)
	{
		x &= o.x;
		return *this;
	}

	int1& operator|=(const int1& o)
	{
		x |= o.x;
		return *this;
	}

	int1 operator&(const int1& o) const
	{
		return int1(x & o.x);
	}

	int1 operator&&(const int1& o) const
	{
		return int1(x && o.x);
	}

	int1 operator|	(const int1& o) const
	{
		return int1(x | o.x);
	}

	int1 operator||(const int1& o) const
	{
		return int1(x || o.x);
	}

	bool2 operator==(const int1& o) const
	{
		return bool2(x == o.x);
	}

	bool2 operator!=(const int1& o) const
	{
		return bool2(x != o.x);
	}

	bool2 operator<(const int1& o) const
	{
		return bool2(x < o.x);
	}

	bool2 operator>(const int1& o) const
	{
		return bool2(x > o.x);
	}

	bool2 operator<=(const int1& o) const
	{
		return bool2(x <= o.x);
	}

	bool2 operator>=(const int1& o) const
	{
		return bool2(x >= o.x);
	}

	int1 operator<<(const int1& o) const
	{
		return int1(x << o.x);
	}

	int1 operator>>(const int1& o) const
	{
		return int1(x >> o.x);
	}

	int1 operator+(const int1& o) const
	{
		return int1(x + o.x);
	}

	int1 operator-(const int1& o) const
	{
		return int1(x - o.x);
	}

	int1 operator*(const int1& o) const
	{
		return int1(x * o.x);
	}

	int1 operator/(const int1& o) const
	{
		return int1(x / o.x);
	}

	int1 operator%(const int1& o) const
	{
		return int1(x % o.x);
	}

	int1 operator+() const
	{
		return int1(+x);
	}

	int1 operator-() const
	{
		return int1(-x);
	}

	int1 operator!() const
	{
		return int1(!x);
	}

	int1 operator~() const
	{
		return int1(~x);
	}

	int1 operator++()
	{
		return int1(++x);
	}

	int1 operator--()
	{
		return int1(--x);
	}


	int1 operator++(int)
	{
		return int1(x++);
	}
	
	int1 operator--(int)
	{
		return int1(x++);
	}


	int operator[](unsigned int i) const
	{
		if(i < 0 || i > 0)
			throw std::range_error("int1 index out of range.");
		return v[i];
	}

	int& operator[](unsigned int i)
	{
		if(i < 0 || i > 0)
			throw std::range_error("int1 index out of range.");
		return v[i];
	}

	int1 max(const int1& o) const
	{
		return int1( (x>o.x)?x:o.x );
	}

	int1 min(const int1& o) const
	{
		return int1( (x<o.x)?x:o.x );
	}

	static int1 random(int lower=0, int upper=6)
	{
		int range = upper - lower + 1;
		return int1(
			rand() % range + lower);
	}

	static const int1 zero;
	static const int1 xUnit;
	static const int1 one;

};

}}
