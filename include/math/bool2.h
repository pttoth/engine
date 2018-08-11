/*
 * -------------------------------------------------------------------------
 * FILE:    bool2.h
 * AUTHOR:  Dr. Szécsi László - szecsi@iit.bme.hu
 * -------------------------------------------------------------------------
 */
#pragma once

#include <math.h>

namespace pttoth { namespace math {

class bool3;
class bool4;

class bool2
{
public:
	union{
		struct {
			bool x;
			bool y;
		};

		bool v[2];

		boolswizzle<2, bool2, 0, 0> xx;
		boolswizzle<2, bool2, 0, 1> xy;
		boolswizzle<2, bool2, 1, 0> yx;
		boolswizzle<2, bool2, 1, 1> yy;
						   
		boolswizzle<2, bool3, 0, 0, 0> xxx;
		boolswizzle<2, bool3, 0, 0, 1> xxy;
		boolswizzle<2, bool3, 0, 1, 0> xyx;
		boolswizzle<2, bool3, 0, 1, 1> xyy;
		boolswizzle<2, bool3, 1, 0, 0> yxx;
		boolswizzle<2, bool3, 1, 0, 1> yxy;
		boolswizzle<2, bool3, 1, 1, 0> yyx;
		boolswizzle<2, bool3, 1, 1, 1> yyy;
						   
		boolswizzle<2, bool4, 0, 0, 0, 0> xxxx;
		boolswizzle<2, bool4, 0, 0, 0, 1> xxxy;
		boolswizzle<2, bool4, 0, 0, 1, 0> xxyx;
		boolswizzle<2, bool4, 0, 0, 1, 1> xxyy;
		boolswizzle<2, bool4, 0, 1, 0, 0> xyxx;
		boolswizzle<2, bool4, 0, 1, 0, 1> xyxy;
		boolswizzle<2, bool4, 0, 1, 1, 0> xyyx;
		boolswizzle<2, bool4, 0, 1, 1, 1> xyyy;
		boolswizzle<2, bool4, 1, 0, 0, 0> yxxx;
		boolswizzle<2, bool4, 1, 0, 0, 1> yxxy;
		boolswizzle<2, bool4, 1, 0, 1, 0> yxyx;
		boolswizzle<2, bool4, 1, 0, 1, 1> yxyy;
		boolswizzle<2, bool4, 1, 1, 0, 0> yyxx;
		boolswizzle<2, bool4, 1, 1, 0, 1> yyxy;
		boolswizzle<2, bool4, 1, 1, 1, 0> yyyx;
		boolswizzle<2, bool4, 1, 1, 1, 1> yyyy;
	};

	bool2():x(false),y(false){}

	bool2(bool b):x(b),y(b){}

	bool2(bool x, bool y):x(x),y(y){}

	bool2(bool x, bool y, bool z, bool w):x(x),y(y){z; w;}

	bool2& operator&=(const bool2& o)
	{
		x = x && o.x;
		y = y && o.y;
		return *this;
	}

	bool2& operator|=(const bool2& o)
	{
		x = x || o.x;
		y = y || o.y;
		return *this;
	}

	bool2 operator&	(const bool2& o) const
	{
		return bool2(x && o.x, y && o.y);
	}

	bool2 operator&&(const bool2& o) const
	{
		return bool2(x && o.x, y && o.y);
	}

	bool2 operator|	(const bool2& o) const
	{
		return bool2(x || o.x, y || o.y);
	}

	bool2 operator||(const bool2& o) const
	{
		return bool2(x || o.x, y || o.y);
	}

	bool2 operator==(const bool2& o) const
	{
		return bool2(x == o.x, y == o.y);
	}

	bool2 operator!=(const bool2& o) const
	{
		return bool2(x != o.x, y != o.y);
	}

	bool2 operator<=(const bool2& o) const
	{
		return bool2(x <= o.x, y <= o.y);
	}

	bool2 operator>=(const bool2& o) const
	{
		return bool2(x >= o.x, y >= o.y);
	}

	bool2 operator!() const
	{
		return bool2(!x, !y);
	}

	bool operator[](unsigned int i) const
	{
		if(i < 0 || i > 1)
			throw std::range_error("bool2 index out of range.");
		return v[i];
	}

	bool& operator[](unsigned int i)
	{
		if(i < 0 || i > 1)
			throw std::range_error("bool2 index out of range.");
		return v[i];
	}

	bool any() const
	{
		return x || y;
	}

	bool all() const
	{
		return x && y;
	}

	static bool2 random()
	{
		return bool2(rand()%2==0, rand()%2==0);
	}

	static const bool2 zero;
	static const bool2 xUnit;
	static const bool2 yUnit;
	static const bool2 one;
};

}}
