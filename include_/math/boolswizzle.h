/*
 * -------------------------------------------------------------------------
 * FILE:    boolswizzle.h
 * AUTHOR:  Dr. Szécsi László - szecsi@iit.bme.hu
 * -------------------------------------------------------------------------
 */
 #pragma once

#define DEFINE_BOOLSWIZZLE_ASSIGNMENT_OPERATOR(BOOLSWIZZLE_OPERATOR)		\
	inline boolswizzle& operator BOOLSWIZZLE_OPERATOR (const T& o)		\
	{	\
		if(0 < nBase) v[ s0 ] BOOLSWIZZLE_OPERATOR o[0];	\
		if(1 < nBase) v[ s1 ] BOOLSWIZZLE_OPERATOR o[1];	\
		if(2 < nBase) v[ s2 ] BOOLSWIZZLE_OPERATOR o[2];	\
		if(3 < nBase) v[ s3 ] BOOLSWIZZLE_OPERATOR o[3];	\
		return *this;	\
	}

#define DEFINE_BOOLSWIZZLE_MONADIC_OPERATOR(BOOLSWIZZLE_OPERATOR)		\
	T operator BOOLSWIZZLE_OPERATOR () const	\
	{	\
		T t = *this;	\
		return BOOLSWIZZLE_OPERATOR t;	\
	}

#define DEFINE_BOOLSWIZZLE_DIADIC_OPERATOR(BOOLSWIZZLE_OPERATOR)		\
	inline T operator BOOLSWIZZLE_OPERATOR(const T& o) const	\
	{	\
		T t = *this;	\
		return t BOOLSWIZZLE_OPERATOR o;	\
	}

#define DEFINE_BOOLSWIZZLE_METHOD(BOOLSWIZZLE_METHOD_NAME)		\
	inline bool BOOLSWIZZLE_METHOD_NAME () const	\
	{	\
		T t = *this;	\
		return t. BOOLSWIZZLE_METHOD_NAME ();	\
	}

namespace pttoth { namespace math {

template<int nBase, class T, int s0=0, int s1=0, int s2=0, int s3=0>
class boolswizzle
{
	bool v[nBase];
public:
	inline operator T() const
	{
		return T(v[s0], v[s1], v[s2], v[s3]);
	}

	DEFINE_BOOLSWIZZLE_ASSIGNMENT_OPERATOR(=)
	DEFINE_BOOLSWIZZLE_ASSIGNMENT_OPERATOR(&=)
	DEFINE_BOOLSWIZZLE_ASSIGNMENT_OPERATOR(|=)

	DEFINE_BOOLSWIZZLE_DIADIC_OPERATOR(&)
	DEFINE_BOOLSWIZZLE_DIADIC_OPERATOR(&&)
	DEFINE_BOOLSWIZZLE_DIADIC_OPERATOR(|)
	DEFINE_BOOLSWIZZLE_DIADIC_OPERATOR(||)
	DEFINE_BOOLSWIZZLE_DIADIC_OPERATOR(==)
	DEFINE_BOOLSWIZZLE_DIADIC_OPERATOR(!=)
	DEFINE_BOOLSWIZZLE_DIADIC_OPERATOR(<=)
	DEFINE_BOOLSWIZZLE_DIADIC_OPERATOR(>=)

	DEFINE_BOOLSWIZZLE_MONADIC_OPERATOR(!)

	DEFINE_BOOLSWIZZLE_METHOD(any)
	DEFINE_BOOLSWIZZLE_METHOD(all)

	inline bool operator[](unsigned int i) const
	{
		if(0 < nBase) if(i == 0) return v[s0];
		if(1 < nBase) if(i == 1) return v[s1];
		if(2 < nBase) if(i == 2) return v[s2];
		if(3 < nBase) if(i == 3) return v[s3];

		throw std::range_error("Vector index invalid.");
	}

	inline bool& operator[](unsigned int i)
	{
		if(0 < nBase) if(i == 0) return v[s0];
		if(1 < nBase) if(i == 1) return v[s1];
		if(2 < nBase) if(i == 2) return v[s2];
		if(3 < nBase) if(i == 3) return v[s3];

		throw std::range_error("Vector index invalid.");
	}
};


}}
