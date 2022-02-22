/*
 * -------------------------------------------------------------------------
 * FILE:    bool1.h
 * AUTHOR:  Dr. Szécsi László - szecsi@iit.bme.hu
 * -------------------------------------------------------------------------
 */

#pragma once

#include <math.h>

namespace pttoth { namespace math {

class bool2;
class bool3;
class bool4;

class bool1
{
public:
	union{
		struct {
			bool x;
		};

		bool v[1];

		boolswizzle<1, bool2, 0, 0> xx;
		boolswizzle<1, bool3, 0, 0, 0> xxx;
		boolswizzle<1, bool4, 0, 0, 0, 0> xxxx;
	};

	operator bool() const
	{
		return x;
	}

	bool1():x(false){}

	bool1(bool b):x(b){}

	bool1(bool x, bool y, bool z, bool w):x(x){y; z; w;}

	bool1& operator&=(const bool1& o)
	{
		x = x && o.x;
		return *this;
	}

	bool1& operator|=(const bool1& o)
	{
		x = x || o.x;
		return *this;
	}

	bool1 operator&	(const bool1& o) const
	{
		return bool1(x && o.x);
	}

	bool1 operator&&(const bool1& o) const
	{
		return bool1(x && o.x);
	}

	bool1 operator|	(const bool1& o) const
	{
		return bool1(x || o.x);
	}

	bool1 operator||(const bool1& o) const
	{
		return bool1(x || o.x);
	}

	bool1 operator==(const bool1& o) const
	{
		return bool1(x == o.x);
	}

	bool1 operator!=(const bool1& o) const
	{
		return bool1(x != o.x);
	}

	bool1 operator<=(const bool1& o) const
	{
		return bool1(x <= o.x);
	}

	bool1 operator>=(const bool1& o) const
	{
		return bool1(x >= o.x);
	}

	bool1 operator!() const
	{
		return bool1(!x);
	}

	bool operator[](unsigned int i) const
	{
		if(i < 0 || i > 0)
			throw std::range_error("bool1 index out of range.");
		return v[i];
	}

	bool& operator[](unsigned int i)
	{
		if(i < 0 || i > 0)
			throw std::range_error("bool1 index out of range.");
		return v[i];
	}

	bool any() const
	{
		return x;
	}

	bool all() const
	{
		return x;
	}

	static bool1 random()
	{
		return bool1(rand()%2==0);
	}

	static const bool1 zero;
	static const bool1 xUnit;
	static const bool1 one;
};

}}
