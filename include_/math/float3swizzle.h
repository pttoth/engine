/*
 * -------------------------------------------------------------------------
 * FILE:    float3swizzle.h
 * AUTHOR:  Dr. Szécsi László - szecsi@iit.bme.hu
 * -------------------------------------------------------------------------
 */
 #pragma once

#define DEFINE_FLOAT3SWIZZLE_METHOD( FLOAT3SWIZZLE_METHOD_NAME, FLOAT3SWIZZLE_METHOD_RETURN_TYPE )	\
	FLOAT3SWIZZLE_METHOD_RETURN_TYPE FLOAT3SWIZZLE_METHOD_NAME () const	\
	{	\
		T t = *this;	\
		return t. FLOAT3SWIZZLE_METHOD_NAME ();	\
	}

#define DEFINE_FLOAT3SWIZZLE_METHOD_DIADIC( FLOAT3SWIZZLE_METHOD_NAME, FLOAT3SWIZZLE_METHOD_RETURN_TYPE )	\
	FLOAT3SWIZZLE_METHOD_RETURN_TYPE FLOAT3SWIZZLE_METHOD_NAME (const T& o) const	\
	{	\
		T t = *this;	\
		return t. FLOAT3SWIZZLE_METHOD_NAME (o);	\
	}

namespace pttoth { namespace math {

template<int nBase, class T, int s0=0, int s1=0, int s2=0, int s3=0>
class float3swizzle : public floatswizzle<nBase, T, int3, bool3, s0, s1, s2, s3> 
{
	DEFINE_FLOAT3SWIZZLE_METHOD_DIADIC(cross, T)

};

	
}}
