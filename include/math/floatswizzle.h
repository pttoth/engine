/*
 * -------------------------------------------------------------------------
 * FILE:    floatswizzle.h
 * AUTHOR:  Dr. Szécsi László - szecsi@iit.bme.hu
 * -------------------------------------------------------------------------
 */

#pragma once

#define DEFINE_FLOATSWIZZLE_ASSIGNMENT_OPERATOR(FLOATSWIZZLE_OPERATOR)		\
	inline floatswizzle& operator FLOATSWIZZLE_OPERATOR (const T& o)		\
	{	\
		if(0 < nBase) v[ s0 ] FLOATSWIZZLE_OPERATOR o[0];	\
		if(1 < nBase) v[ s1 ] FLOATSWIZZLE_OPERATOR o[1];	\
		if(2 < nBase) v[ s2 ] FLOATSWIZZLE_OPERATOR o[2];	\
		if(3 < nBase) v[ s3 ] FLOATSWIZZLE_OPERATOR o[3];	\
		return *this;	\
	}

#define DEFINE_FLOATSWIZZLE_MONADIC_OPERATOR(FLOATSWIZZLE_OPERATOR, T)		\
	T operator FLOATSWIZZLE_OPERATOR () const	\
	{	\
		T t = *this;	\
		return FLOATSWIZZLE_OPERATOR t;	\
	}

#define DEFINE_FLOATSWIZZLE_DIADIC_OPERATOR(FLOATSWIZZLE_OPERATOR, T)		\
	inline T operator FLOATSWIZZLE_OPERATOR (const T & o) const	\
	{	\
		T t = *this;	\
		return t FLOATSWIZZLE_OPERATOR o;	\
	}

#define DEFINE_FLOATSWIZZLE_METHOD(FLOATSWIZZLE_METHOD_NAME, FLOATSWIZZLE_METHOD_RETURN_TYPE)		\
	inline FLOATSWIZZLE_METHOD_RETURN_TYPE FLOATSWIZZLE_METHOD_NAME () const	\
	{	\
		T t = *this;	\
		return t. FLOATSWIZZLE_METHOD_NAME ();	\
	}

#define DEFINE_FLOATSWIZZLE_METHOD_DIADIC(FLOATSWIZZLE_METHOD_NAME, FLOATSWIZZLE_METHOD_RETURN_TYPE)		\
	inline FLOATSWIZZLE_METHOD_RETURN_TYPE FLOATSWIZZLE_METHOD_NAME (const T& o) const	\
	{	\
		T t = *this;	\
		return t. FLOATSWIZZLE_METHOD_NAME ( o );	\
	}

#define DEFINE_FLOATSWIZZLE_METHOD_TRIADIC(FLOATSWIZZLE_METHOD_NAME, FLOATSWIZZLE_METHOD_RETURN_TYPE)		\
	inline FLOATSWIZZLE_METHOD_RETURN_TYPE FLOATSWIZZLE_METHOD_NAME (const T& a, const T& b) const	\
	{	\
		T t = *this;	\
		return t. FLOATSWIZZLE_METHOD_NAME ( a, b );	\
	}

namespace pttoth { namespace math {

template<int nBase, class T, class I, class B, int s0=0, int s1=0, int s2=0, int s3=0>
class floatswizzle
{
	float v[nBase];
public:
	operator T() const
	{
		return T( 
			(s0>=0)?v[s0]:((s0==-1)?0.0f:1.0f),
			(s1>=0)?v[s1]:((s1==-1)?0.0f:1.0f),
			(s2>=0)?v[s2]:((s2==-1)?0.0f:1.0f),
			(s3>=0)?v[s3]:((s3==-1)?0.0f:1.0f));
	}

	DEFINE_FLOATSWIZZLE_ASSIGNMENT_OPERATOR(=)
	DEFINE_FLOATSWIZZLE_ASSIGNMENT_OPERATOR(+=)
	DEFINE_FLOATSWIZZLE_ASSIGNMENT_OPERATOR(-=)
	DEFINE_FLOATSWIZZLE_ASSIGNMENT_OPERATOR(*=)
	DEFINE_FLOATSWIZZLE_ASSIGNMENT_OPERATOR(/=)

	floatswizzle& operator%=(const T& o)
	{
    //original code:
        /*
            for(int u=0; u<nBase; u++)
                v[s[u]] = fmodf(v[s[u]], o[u]);
            return *this;
        */
        if(0 < nBase){
            v[s0] = fmodf(v[s0], o[0]);
        }
        if(1 < nBase){
            v[s1] = fmodf(v[s1], o[1]);
        }
        if(2 < nBase){
            v[s2] = fmodf(v[s2], o[2]);
        }
        if(3 < nBase){
            v[s3] = fmodf(v[s3], o[3]);
        }
	}

	DEFINE_FLOATSWIZZLE_DIADIC_OPERATOR(==, B)
	DEFINE_FLOATSWIZZLE_DIADIC_OPERATOR(!=, B)
	DEFINE_FLOATSWIZZLE_DIADIC_OPERATOR(< , B)
	DEFINE_FLOATSWIZZLE_DIADIC_OPERATOR(<=, B)
	DEFINE_FLOATSWIZZLE_DIADIC_OPERATOR(> , B)
	DEFINE_FLOATSWIZZLE_DIADIC_OPERATOR(>=, B)
	DEFINE_FLOATSWIZZLE_DIADIC_OPERATOR(+ , T)
	DEFINE_FLOATSWIZZLE_DIADIC_OPERATOR(- , T)
	DEFINE_FLOATSWIZZLE_DIADIC_OPERATOR(* , T)
	DEFINE_FLOATSWIZZLE_DIADIC_OPERATOR(/ , T)
	DEFINE_FLOATSWIZZLE_DIADIC_OPERATOR(% , T)

	DEFINE_FLOATSWIZZLE_MONADIC_OPERATOR(+, T)
	DEFINE_FLOATSWIZZLE_MONADIC_OPERATOR(-, T)
	DEFINE_FLOATSWIZZLE_MONADIC_OPERATOR(!, T)

	float operator[](unsigned int i) const
	{
		if(0 < nBase) if(i == 0) return v[s0];
		if(1 < nBase) if(i == 1) return v[s1];
		if(2 < nBase) if(i == 2) return v[s2];
		if(3 < nBase) if(i == 3) return v[s3];

		throw std::range_error("Vector index invalid.");
	}

	float& operator[](unsigned int i)
	{
		if(0 < nBase) if(i == 0) return v[s0];
		if(1 < nBase) if(i == 1) return v[s1];
		if(2 < nBase) if(i == 2) return v[s2];
		if(3 < nBase) if(i == 3) return v[s3];

		throw std::range_error("Vector index invalid.");
	}

	DEFINE_FLOATSWIZZLE_METHOD(abs, T)
	DEFINE_FLOATSWIZZLE_METHOD(acos, T)
	DEFINE_FLOATSWIZZLE_METHOD(asin, T)
	DEFINE_FLOATSWIZZLE_METHOD(atan, T)
	DEFINE_FLOATSWIZZLE_METHOD(ceil, I)
	DEFINE_FLOATSWIZZLE_METHOD(cos, T)
	DEFINE_FLOATSWIZZLE_METHOD(cosh, T)
	DEFINE_FLOATSWIZZLE_METHOD(exp, T)
	DEFINE_FLOATSWIZZLE_METHOD(exp2, T)
	DEFINE_FLOATSWIZZLE_METHOD(floor, I)
	DEFINE_FLOATSWIZZLE_METHOD(frac, T)
/*
	DEFINE_FLOATSWIZZLE_METHOD(isfinite, B)
	DEFINE_FLOATSWIZZLE_METHOD(isinf, B)
	DEFINE_FLOATSWIZZLE_METHOD(isnan, B)
    */
	DEFINE_FLOATSWIZZLE_METHOD(log, T)
	DEFINE_FLOATSWIZZLE_METHOD(log10, T)
	DEFINE_FLOATSWIZZLE_METHOD(log2, T)
	DEFINE_FLOATSWIZZLE_METHOD(normalize, T)
	DEFINE_FLOATSWIZZLE_METHOD(length, float)
	DEFINE_FLOATSWIZZLE_METHOD(lengthSquared, float)
	DEFINE_FLOATSWIZZLE_METHOD(round, I)
	DEFINE_FLOATSWIZZLE_METHOD(rsqrt, T)
	DEFINE_FLOATSWIZZLE_METHOD(saturate, T)
	DEFINE_FLOATSWIZZLE_METHOD(sign, T)
	DEFINE_FLOATSWIZZLE_METHOD(sin, T)
	DEFINE_FLOATSWIZZLE_METHOD(sinh, T)
	DEFINE_FLOATSWIZZLE_METHOD(sqrt, T)
	DEFINE_FLOATSWIZZLE_METHOD(step, T)
	DEFINE_FLOATSWIZZLE_METHOD(tan, T)
	DEFINE_FLOATSWIZZLE_METHOD(trunc, I)

	DEFINE_FLOATSWIZZLE_METHOD_DIADIC(distance, T)
	DEFINE_FLOATSWIZZLE_METHOD_DIADIC(dot, T)
	DEFINE_FLOATSWIZZLE_METHOD_DIADIC(fmod, T)
	DEFINE_FLOATSWIZZLE_METHOD_DIADIC(max, T)
	DEFINE_FLOATSWIZZLE_METHOD_DIADIC(min, T)
	DEFINE_FLOATSWIZZLE_METHOD_DIADIC(pow, T)

	DEFINE_FLOATSWIZZLE_METHOD_TRIADIC(clamp, T)
	DEFINE_FLOATSWIZZLE_METHOD_TRIADIC(lerp, T)
	DEFINE_FLOATSWIZZLE_METHOD_TRIADIC(smoothstep, T)

};

}}
