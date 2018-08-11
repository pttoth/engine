/*
 * -------------------------------------------------------------------------
 * FILE:    float2swizzle.h
 * AUTHOR:  Dr. Szécsi László - szecsi@iit.bme.hu
 * -------------------------------------------------------------------------
 */
 #pragma once

#define DEFINE_FLOAT2SWIZZLE_METHOD( FLOAT2SWIZZLE_METHOD_NAME, FLOAT2SWIZZLE_METHOD_RETURN_TYPE )	\
	FLOAT2SWIZZLE_METHOD_RETURN_TYPE FLOAT2SWIZZLE_METHOD_NAME () const	\
	{	\
		T t = *this;	\
		return t. FLOAT2SWIZZLE_METHOD_NAME ();	\
	}

#define DEFINE_FLOAT2SWIZZLE_METHOD_DIADIC( FLOAT2SWIZZLE_METHOD_NAME, FLOAT2SWIZZLE_METHOD_RETURN_TYPE )	\
	FLOAT2SWIZZLE_METHOD_RETURN_TYPE FLOAT2SWIZZLE_METHOD_NAME (const T& o) const	\
	{	\
		T t = *this;	\
		return t. FLOAT2SWIZZLE_METHOD_NAME (o);	\
	}

namespace pttoth { namespace math {

template<int nBase, class T, int s0=0, int s1=1, int s2=2, int s3=3>
class float2swizzle : public floatswizzle<nBase, T, int2, bool2, s0, s1, s2, s3> 
{
	DEFINE_FLOAT2SWIZZLE_METHOD(arg, float)
	DEFINE_FLOAT2SWIZZLE_METHOD_DIADIC(complexmul, T)
	DEFINE_FLOAT2SWIZZLE_METHOD(polar, T)
	DEFINE_FLOAT2SWIZZLE_METHOD(cartesian, T)

	T rotate(float angle) const
	{
		T t = *this;
		return t.rotate(angle);
	}
};

}}
