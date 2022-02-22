/*
 * -------------------------------------------------------------------------
 * FILE:    intswizzle.h
 * AUTHOR:  Dr. Szécsi László - szecsi@iit.bme.hu
 * -------------------------------------------------------------------------
 */

#pragma once

#define DEFINE_INTSWIZZLE_ASSIGNMENT_OPERATOR(INTSWIZZLE_OPERATOR)		\
	inline intswizzle& operator INTSWIZZLE_OPERATOR (const T& o)		\
	{	\
		if(0 < nBase) v[ s0 ] INTSWIZZLE_OPERATOR o[0];	\
		if(1 < nBase) v[ s1 ] INTSWIZZLE_OPERATOR o[1];	\
		if(2 < nBase) v[ s2 ] INTSWIZZLE_OPERATOR o[2];	\
		if(3 < nBase) v[ s3 ] INTSWIZZLE_OPERATOR o[3];	\
		return *this;	\
	}

#define DEFINE_INTSWIZZLE_MONADIC_OPERATOR(INTSWIZZLE_OPERATOR, T)		\
	T operator INTSWIZZLE_OPERATOR () const	\
	{	\
		T t = *this;	\
		return INTSWIZZLE_OPERATOR t;	\
	}

#define DEFINE_INTSWIZZLE_DIADIC_OPERATOR(INTSWIZZLE_OPERATOR, T)		\
	inline T operator INTSWIZZLE_OPERATOR (const T & o) const	\
	{	\
		T t = *this;	\
		return t INTSWIZZLE_OPERATOR o;	\
	}

#define DEFINE_INTSWIZZLE_METHOD(INTSWIZZLE_METHOD_NAME, INTSWIZZLE_METHOD_RETURN_TYPE)		\
	inline INTSWIZZLE_METHOD_RETURN_TYPE INTSWIZZLE_METHOD_NAME () const	\
	{	\
		T t = *this;	\
		return t. INTSWIZZLE_METHOD_NAME ();	\
	}

#define DEFINE_INTSWIZZLE_METHOD_DIADIC(INTSWIZZLE_METHOD_NAME, INTSWIZZLE_METHOD_RETURN_TYPE)		\
	inline INTSWIZZLE_METHOD_RETURN_TYPE INTSWIZZLE_METHOD_NAME (const T& o) const	\
	{	\
		T t = *this;	\
		return t. INTSWIZZLE_METHOD_NAME ( o );	\
	}

#define DEFINE_INTSWIZZLE_METHOD_TRIADIC(INTSWIZZLE_METHOD_NAME, INTSWIZZLE_METHOD_RETURN_TYPE)		\
	inline INTSWIZZLE_METHOD_RETURN_TYPE INTSWIZZLE_METHOD_NAME (const T& a, const T& b) const	\
	{	\
		T t = *this;	\
		return t. INTSWIZZLE_METHOD_NAME ( a, b );	\
	}

namespace pttoth { namespace math {

template<int nBase, class T, class B, int s0=0, int s1=0, int s2=0, int s3=0>
class intswizzle
{
	int v[nBase];
public:
	inline operator T() const
	{
		return T(v[s0], v[s1], v[s2], v[s3]);
	}
	
	DEFINE_INTSWIZZLE_ASSIGNMENT_OPERATOR(=)
	DEFINE_INTSWIZZLE_ASSIGNMENT_OPERATOR(+=)
	DEFINE_INTSWIZZLE_ASSIGNMENT_OPERATOR(-=)
	DEFINE_INTSWIZZLE_ASSIGNMENT_OPERATOR(*=)
	DEFINE_INTSWIZZLE_ASSIGNMENT_OPERATOR(/=)
	DEFINE_INTSWIZZLE_ASSIGNMENT_OPERATOR(%=)
	DEFINE_INTSWIZZLE_ASSIGNMENT_OPERATOR(>>)
	DEFINE_INTSWIZZLE_ASSIGNMENT_OPERATOR(<<)
	DEFINE_INTSWIZZLE_ASSIGNMENT_OPERATOR(&=)
	DEFINE_INTSWIZZLE_ASSIGNMENT_OPERATOR(|=)

	DEFINE_INTSWIZZLE_DIADIC_OPERATOR(&	, T)
	DEFINE_INTSWIZZLE_DIADIC_OPERATOR(&&, T)
	DEFINE_INTSWIZZLE_DIADIC_OPERATOR(|	, T)
	DEFINE_INTSWIZZLE_DIADIC_OPERATOR(||, T)
	DEFINE_INTSWIZZLE_DIADIC_OPERATOR(==, B)
	DEFINE_INTSWIZZLE_DIADIC_OPERATOR(!=, B)
	DEFINE_INTSWIZZLE_DIADIC_OPERATOR(< , B)
	DEFINE_INTSWIZZLE_DIADIC_OPERATOR(> , B)
	DEFINE_INTSWIZZLE_DIADIC_OPERATOR(<=, B)
	DEFINE_INTSWIZZLE_DIADIC_OPERATOR(>=, B)
	DEFINE_INTSWIZZLE_DIADIC_OPERATOR(<<, T)
	DEFINE_INTSWIZZLE_DIADIC_OPERATOR(>>, T)

	DEFINE_INTSWIZZLE_DIADIC_OPERATOR(+, T)
	DEFINE_INTSWIZZLE_DIADIC_OPERATOR(-, T)
	DEFINE_INTSWIZZLE_DIADIC_OPERATOR(*, T)
	DEFINE_INTSWIZZLE_DIADIC_OPERATOR(/, T)
	DEFINE_INTSWIZZLE_DIADIC_OPERATOR(%, T)

	DEFINE_INTSWIZZLE_MONADIC_OPERATOR(+, T)
	DEFINE_INTSWIZZLE_MONADIC_OPERATOR(-, T)
	DEFINE_INTSWIZZLE_MONADIC_OPERATOR(!, T)
	DEFINE_INTSWIZZLE_MONADIC_OPERATOR(~, T)
	DEFINE_INTSWIZZLE_MONADIC_OPERATOR(++, T)
	DEFINE_INTSWIZZLE_MONADIC_OPERATOR(--, T)

	inline T operator++(int)
	{
		T t = *this;
		return t++;
	}

	inline T operator--(int)
	{
		T t = *this;
		return t--;
	}

	int operator[](unsigned int i) const
	{
		if(0 < nBase) if(i == 0) return v[s0];
		if(1 < nBase) if(i == 1) return v[s1];
		if(2 < nBase) if(i == 2) return v[s2];
		if(3 < nBase) if(i == 3) return v[s3];

		throw std::range_error("Vector index invalid.");
	}

	int& operator[](unsigned int i)
	{
		if(0 < nBase) if(i == 0) return v[s0];
		if(1 < nBase) if(i == 1) return v[s1];
		if(2 < nBase) if(i == 2) return v[s2];
		if(3 < nBase) if(i == 3) return v[s3];

		throw std::range_error("Vector index invalid.");

	}

	DEFINE_INTSWIZZLE_METHOD_DIADIC(max, T)
	DEFINE_INTSWIZZLE_METHOD_DIADIC(min, T)
};

}}
