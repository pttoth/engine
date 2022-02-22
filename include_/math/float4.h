/*
 * -------------------------------------------------------------------------
 * FILE:    float4.h
 * AUTHOR:  Dr. Szécsi László - szecsi@iit.bme.hu
 * -------------------------------------------------------------------------
 */
#pragma once

#include <math.h>

#define DEFINE_FLOAT4_METHOD(FLOAT4_METHOD_NAME, FLOAT4_METHOD_RETURN_TYPE)	\
	FLOAT4_METHOD_RETURN_TYPE FLOAT4_METHOD_NAME () const	\
	{	\
		return FLOAT4_METHOD_RETURN_TYPE ( :: FLOAT4_METHOD_NAME (x), :: FLOAT4_METHOD_NAME (y), :: FLOAT4_METHOD_NAME (z), :: FLOAT4_METHOD_NAME (w));	\
	}

#define DEFINE_FLOAT4_METHOD_DIADIC(FLOAT4_METHOD_NAME, FLOAT4_METHOD_RETURN_TYPE)	\
	FLOAT4_METHOD_RETURN_TYPE FLOAT4_METHOD_NAME (const float4& o) const	\
	{	\
		return FLOAT4_METHOD_RETURN_TYPE ( :: FLOAT4_METHOD_NAME (x, o.x), :: FLOAT4_METHOD_NAME (y, o.y), :: FLOAT4_METHOD_NAME (z, o.z), :: FLOAT4_METHOD_NAME (w, o.w));	\
	}

#define DEFINE_FLOAT4_METHOD_TRIADIC(FLOAT4_METHOD_NAME, FLOAT4_METHOD_RETURN_TYPE)	\
	FLOAT4_METHOD_RETURN_TYPE FLOAT4_METHOD_NAME (const float4& a, const float4& b) const	\
	{	\
		return FLOAT4_METHOD_RETURN_TYPE ( :: FLOAT4_METHOD_NAME (x, a.x, b.x), :: FLOAT4_METHOD_NAME (y, a.y, b.y), :: FLOAT4_METHOD_NAME (z, a.z, b.z), :: FLOAT4_METHOD_NAME (w, a.w, b.w));	\
	}

namespace pttoth { namespace math {

class float2;
class float3;
class float4;

class float4
{
public:
	union{
		struct {
			float x;
			float y;
			float z;
			float w;
		};
		
		float v[4];

		float2swizzle<4, float2, 0, 0> xx;
		float2swizzle<4, float2, 0, 1> xy;
		float2swizzle<4, float2, 0, 2> xz;
		float2swizzle<4, float2, 0, 3> xw;
		float2swizzle<4, float2, 1, 0> yx;
		float2swizzle<4, float2, 1, 1> yy;
		float2swizzle<4, float2, 1, 2> yz;
		float2swizzle<4, float2, 1, 3> yw;
		float2swizzle<4, float2, 2, 0> zx;
		float2swizzle<4, float2, 2, 1> zy;
		float2swizzle<4, float2, 2, 2> zz;
		float2swizzle<4, float2, 2, 3> zw;
		float2swizzle<4, float2, 3, 0> wx;
		float2swizzle<4, float2, 3, 1> wy;
		float2swizzle<4, float2, 3, 2> wz;
		float2swizzle<4, float2, 3, 3> ww;

		float3swizzle<4, float3, 0, 0, 0> xxx;
		float3swizzle<4, float3, 0, 0, 1> xxy;
		float3swizzle<4, float3, 0, 0, 2> xxz;
		float3swizzle<4, float3, 0, 0, 3> xxw;
		float3swizzle<4, float3, 0, 1, 0> xyx;
		float3swizzle<4, float3, 0, 1, 1> xyy;
		float3swizzle<4, float3, 0, 1, 2> xyz;
		float3swizzle<4, float3, 0, 1, 3> xyw;
		float3swizzle<4, float3, 0, 2, 0> xzx;
		float3swizzle<4, float3, 0, 2, 1> xzy;
		float3swizzle<4, float3, 0, 2, 2> xzz;
		float3swizzle<4, float3, 0, 2, 3> xzw;
		float3swizzle<4, float3, 0, 3, 0> xwx;
		float3swizzle<4, float3, 0, 3, 1> xwy;
		float3swizzle<4, float3, 0, 3, 2> xwz;
		float3swizzle<4, float3, 0, 3, 3> xww;

		float3swizzle<4, float3, 1, 0, 0> yxx;
		float3swizzle<4, float3, 1, 0, 1> yxy;
		float3swizzle<4, float3, 1, 0, 2> yxz;
		float3swizzle<4, float3, 1, 0, 3> yxw;
		float3swizzle<4, float3, 1, 1, 0> yyx;
		float3swizzle<4, float3, 1, 1, 1> yyy;
		float3swizzle<4, float3, 1, 1, 2> yyz;
		float3swizzle<4, float3, 1, 1, 3> yyw;
		float3swizzle<4, float3, 1, 2, 0> yzx;
		float3swizzle<4, float3, 1, 2, 1> yzy;
		float3swizzle<4, float3, 1, 2, 2> yzz;
		float3swizzle<4, float3, 1, 2, 3> yzw;
		float3swizzle<4, float3, 1, 3, 0> ywx;
		float3swizzle<4, float3, 1, 3, 1> ywy;
		float3swizzle<4, float3, 1, 3, 2> ywz;
		float3swizzle<4, float3, 1, 3, 3> yww;

		float3swizzle<4, float3, 2, 0, 0> zxx;
		float3swizzle<4, float3, 2, 0, 1> zxy;
		float3swizzle<4, float3, 2, 0, 2> zxz;
		float3swizzle<4, float3, 2, 0, 3> zxw;
		float3swizzle<4, float3, 2, 1, 0> zyx;
		float3swizzle<4, float3, 2, 1, 1> zyy;
		float3swizzle<4, float3, 2, 1, 2> zyz;
		float3swizzle<4, float3, 2, 1, 3> zyw;
		float3swizzle<4, float3, 2, 2, 0> zzx;
		float3swizzle<4, float3, 2, 2, 1> zzy;
		float3swizzle<4, float3, 2, 2, 2> zzz;
		float3swizzle<4, float3, 2, 2, 3> zzw;
		float3swizzle<4, float3, 2, 3, 0> zwx;
		float3swizzle<4, float3, 2, 3, 1> zwy;
		float3swizzle<4, float3, 2, 3, 2> zwz;
		float3swizzle<4, float3, 2, 3, 3> zww;

		float3swizzle<4, float3, 3, 0, 0> wxx;
		float3swizzle<4, float3, 3, 0, 1> wxy;
		float3swizzle<4, float3, 3, 0, 2> wxz;
		float3swizzle<4, float3, 3, 0, 3> wxw;
		float3swizzle<4, float3, 3, 1, 0> wyx;
		float3swizzle<4, float3, 3, 1, 1> wyy;
		float3swizzle<4, float3, 3, 1, 2> wyz;
		float3swizzle<4, float3, 3, 1, 3> wyw;
		float3swizzle<4, float3, 3, 2, 0> wzx;
		float3swizzle<4, float3, 3, 2, 1> wzy;
		float3swizzle<4, float3, 3, 2, 2> wzz;
		float3swizzle<4, float3, 3, 2, 3> wzw;
		float3swizzle<4, float3, 3, 3, 0> wwx;
		float3swizzle<4, float3, 3, 3, 1> wwy;
		float3swizzle<4, float3, 3, 3, 2> wwz;
		float3swizzle<4, float3, 3, 3, 3> www;

		float4swizzle<4, float4, 0, 0, 0, 0> xxxx;
		float4swizzle<4, float4, 0, 0, 0, 1> xxxy;
		float4swizzle<4, float4, 0, 0, 0, 2> xxxz;
		float4swizzle<4, float4, 0, 0, 0, 3> xxxw;
		float4swizzle<4, float4, 0, 0, 1, 0> xxyx;
		float4swizzle<4, float4, 0, 0, 1, 1> xxyy;
		float4swizzle<4, float4, 0, 0, 1, 2> xxyz;
		float4swizzle<4, float4, 0, 0, 1, 3> xxyw;
		float4swizzle<4, float4, 0, 0, 2, 0> xxzx;
		float4swizzle<4, float4, 0, 0, 2, 1> xxzy;
		float4swizzle<4, float4, 0, 0, 2, 2> xxzz;
		float4swizzle<4, float4, 0, 0, 2, 3> xxzw;
		float4swizzle<4, float4, 0, 0, 3, 0> xxwx;
		float4swizzle<4, float4, 0, 0, 3, 1> xxwy;
		float4swizzle<4, float4, 0, 0, 3, 2> xxwz;
		float4swizzle<4, float4, 0, 0, 3, 3> xxww;
		float4swizzle<4, float4, 0, 1, 0, 0> xyxx;
		float4swizzle<4, float4, 0, 1, 0, 1> xyxy;
		float4swizzle<4, float4, 0, 1, 0, 2> xyxz;
		float4swizzle<4, float4, 0, 1, 0, 3> xyxw;
		float4swizzle<4, float4, 0, 1, 1, 0> xyyx;
		float4swizzle<4, float4, 0, 1, 1, 1> xyyy;
		float4swizzle<4, float4, 0, 1, 1, 2> xyyz;
		float4swizzle<4, float4, 0, 1, 1, 3> xyyw;
		float4swizzle<4, float4, 0, 1, 2, 0> xyzx;
		float4swizzle<4, float4, 0, 1, 2, 1> xyzy;
		float4swizzle<4, float4, 0, 1, 2, 2> xyzz;
		float4swizzle<4, float4, 0, 1, 2, 3> xyzw;
		float4swizzle<4, float4, 0, 1, 3, 0> xywx;
		float4swizzle<4, float4, 0, 1, 3, 1> xywy;
		float4swizzle<4, float4, 0, 1, 3, 2> xywz;
		float4swizzle<4, float4, 0, 1, 3, 3> xyww;
		float4swizzle<4, float4, 0, 2, 0, 0> xzxx;
		float4swizzle<4, float4, 0, 2, 0, 1> xzxy;
		float4swizzle<4, float4, 0, 2, 0, 2> xzxz;
		float4swizzle<4, float4, 0, 2, 0, 3> xzxw;
		float4swizzle<4, float4, 0, 2, 1, 0> xzyx;
		float4swizzle<4, float4, 0, 2, 1, 1> xzyy;
		float4swizzle<4, float4, 0, 2, 1, 2> xzyz;
		float4swizzle<4, float4, 0, 2, 1, 3> xzyw;
		float4swizzle<4, float4, 0, 2, 2, 0> xzzx;
		float4swizzle<4, float4, 0, 2, 2, 1> xzzy;
		float4swizzle<4, float4, 0, 2, 2, 2> xzzz;
		float4swizzle<4, float4, 0, 2, 2, 3> xzzw;
		float4swizzle<4, float4, 0, 2, 3, 0> xzwx;
		float4swizzle<4, float4, 0, 2, 3, 1> xzwy;
		float4swizzle<4, float4, 0, 2, 3, 2> xzwz;
		float4swizzle<4, float4, 0, 2, 3, 3> xzww;
		float4swizzle<4, float4, 0, 3, 0, 0> xwxx;
		float4swizzle<4, float4, 0, 3, 0, 1> xwxy;
		float4swizzle<4, float4, 0, 3, 0, 2> xwxz;
		float4swizzle<4, float4, 0, 3, 0, 3> xwxw;
		float4swizzle<4, float4, 0, 3, 1, 0> xwyx;
		float4swizzle<4, float4, 0, 3, 1, 1> xwyy;
		float4swizzle<4, float4, 0, 3, 1, 2> xwyz;
		float4swizzle<4, float4, 0, 3, 1, 3> xwyw;
		float4swizzle<4, float4, 0, 3, 2, 0> xwzx;
		float4swizzle<4, float4, 0, 3, 2, 1> xwzy;
		float4swizzle<4, float4, 0, 3, 2, 2> xwzz;
		float4swizzle<4, float4, 0, 3, 2, 3> xwzw;
		float4swizzle<4, float4, 0, 3, 3, 0> xwwx;
		float4swizzle<4, float4, 0, 3, 3, 1> xwwy;
		float4swizzle<4, float4, 0, 3, 3, 2> xwwz;
		float4swizzle<4, float4, 0, 3, 3, 3> xwww;

		float4swizzle<4, float4, 1, 0, 0, 0> yxxx;
		float4swizzle<4, float4, 1, 0, 0, 1> yxxy;
		float4swizzle<4, float4, 1, 0, 0, 2> yxxz;
		float4swizzle<4, float4, 1, 0, 0, 3> yxxw;
		float4swizzle<4, float4, 1, 0, 1, 0> yxyx;
		float4swizzle<4, float4, 1, 0, 1, 1> yxyy;
		float4swizzle<4, float4, 1, 0, 1, 2> yxyz;
		float4swizzle<4, float4, 1, 0, 1, 3> yxyw;
		float4swizzle<4, float4, 1, 0, 2, 0> yxzx;
		float4swizzle<4, float4, 1, 0, 2, 1> yxzy;
		float4swizzle<4, float4, 1, 0, 2, 2> yxzz;
		float4swizzle<4, float4, 1, 0, 2, 3> yxzw;
		float4swizzle<4, float4, 1, 0, 3, 0> yxwx;
		float4swizzle<4, float4, 1, 0, 3, 1> yxwy;
		float4swizzle<4, float4, 1, 0, 3, 2> yxwz;
		float4swizzle<4, float4, 1, 0, 3, 3> yxww;
		float4swizzle<4, float4, 1, 1, 0, 0> yyxx;
		float4swizzle<4, float4, 1, 1, 0, 1> yyxy;
		float4swizzle<4, float4, 1, 1, 0, 2> yyxz;
		float4swizzle<4, float4, 1, 1, 0, 3> yyxw;
		float4swizzle<4, float4, 1, 1, 1, 0> yyyx;
		float4swizzle<4, float4, 1, 1, 1, 1> yyyy;
		float4swizzle<4, float4, 1, 1, 1, 2> yyyz;
		float4swizzle<4, float4, 1, 1, 1, 3> yyyw;
		float4swizzle<4, float4, 1, 1, 2, 0> yyzx;
		float4swizzle<4, float4, 1, 1, 2, 1> yyzy;
		float4swizzle<4, float4, 1, 1, 2, 2> yyzz;
		float4swizzle<4, float4, 1, 1, 2, 3> yyzw;
		float4swizzle<4, float4, 1, 1, 3, 0> yywx;
		float4swizzle<4, float4, 1, 1, 3, 1> yywy;
		float4swizzle<4, float4, 1, 1, 3, 2> yywz;
		float4swizzle<4, float4, 1, 1, 3, 3> yyww;
		float4swizzle<4, float4, 1, 2, 0, 0> yzxx;
		float4swizzle<4, float4, 1, 2, 0, 1> yzxy;
		float4swizzle<4, float4, 1, 2, 0, 2> yzxz;
		float4swizzle<4, float4, 1, 2, 0, 3> yzxw;
		float4swizzle<4, float4, 1, 2, 1, 0> yzyx;
		float4swizzle<4, float4, 1, 2, 1, 1> yzyy;
		float4swizzle<4, float4, 1, 2, 1, 2> yzyz;
		float4swizzle<4, float4, 1, 2, 1, 3> yzyw;
		float4swizzle<4, float4, 1, 2, 2, 0> yzzx;
		float4swizzle<4, float4, 1, 2, 2, 1> yzzy;
		float4swizzle<4, float4, 1, 2, 2, 2> yzzz;
		float4swizzle<4, float4, 1, 2, 2, 3> yzzw;
		float4swizzle<4, float4, 1, 2, 3, 0> yzwx;
		float4swizzle<4, float4, 1, 2, 3, 1> yzwy;
		float4swizzle<4, float4, 1, 2, 3, 2> yzwz;
		float4swizzle<4, float4, 1, 2, 3, 3> yzww;
		float4swizzle<4, float4, 1, 3, 0, 0> ywxx;
		float4swizzle<4, float4, 1, 3, 0, 1> ywxy;
		float4swizzle<4, float4, 1, 3, 0, 2> ywxz;
		float4swizzle<4, float4, 1, 3, 0, 3> ywxw;
		float4swizzle<4, float4, 1, 3, 1, 0> ywyx;
		float4swizzle<4, float4, 1, 3, 1, 1> ywyy;
		float4swizzle<4, float4, 1, 3, 1, 2> ywyz;
		float4swizzle<4, float4, 1, 3, 1, 3> ywyw;
		float4swizzle<4, float4, 1, 3, 2, 0> ywzx;
		float4swizzle<4, float4, 1, 3, 2, 1> ywzy;
		float4swizzle<4, float4, 1, 3, 2, 2> ywzz;
		float4swizzle<4, float4, 1, 3, 2, 3> ywzw;
		float4swizzle<4, float4, 1, 3, 3, 0> ywwx;
		float4swizzle<4, float4, 1, 3, 3, 1> ywwy;
		float4swizzle<4, float4, 1, 3, 3, 2> ywwz;
		float4swizzle<4, float4, 1, 3, 3, 3> ywww;

		float4swizzle<4, float4, 2, 0, 0, 0> zxxx;
		float4swizzle<4, float4, 2, 0, 0, 1> zxxy;
		float4swizzle<4, float4, 2, 0, 0, 2> zxxz;
		float4swizzle<4, float4, 2, 0, 0, 3> zxxw;
		float4swizzle<4, float4, 2, 0, 1, 0> zxyx;
		float4swizzle<4, float4, 2, 0, 1, 1> zxyy;
		float4swizzle<4, float4, 2, 0, 1, 2> zxyz;
		float4swizzle<4, float4, 2, 0, 1, 3> zxyw;
		float4swizzle<4, float4, 2, 0, 2, 0> zxzx;
		float4swizzle<4, float4, 2, 0, 2, 1> zxzy;
		float4swizzle<4, float4, 2, 0, 2, 2> zxzz;
		float4swizzle<4, float4, 2, 0, 2, 3> zxzw;
		float4swizzle<4, float4, 2, 0, 3, 0> zxwx;
		float4swizzle<4, float4, 2, 0, 3, 1> zxwy;
		float4swizzle<4, float4, 2, 0, 3, 2> zxwz;
		float4swizzle<4, float4, 2, 0, 3, 3> zxww;
		float4swizzle<4, float4, 2, 1, 0, 0> zyxx;
		float4swizzle<4, float4, 2, 1, 0, 1> zyxy;
		float4swizzle<4, float4, 2, 1, 0, 2> zyxz;
		float4swizzle<4, float4, 2, 1, 0, 3> zyxw;
		float4swizzle<4, float4, 2, 1, 1, 0> zyyx;
		float4swizzle<4, float4, 2, 1, 1, 1> zyyy;
		float4swizzle<4, float4, 2, 1, 1, 2> zyyz;
		float4swizzle<4, float4, 2, 1, 1, 3> zyyw;
		float4swizzle<4, float4, 2, 1, 2, 0> zyzx;
		float4swizzle<4, float4, 2, 1, 2, 1> zyzy;
		float4swizzle<4, float4, 2, 1, 2, 2> zyzz;
		float4swizzle<4, float4, 2, 1, 2, 3> zyzw;
		float4swizzle<4, float4, 2, 1, 3, 0> zywx;
		float4swizzle<4, float4, 2, 1, 3, 1> zywy;
		float4swizzle<4, float4, 2, 1, 3, 2> zywz;
		float4swizzle<4, float4, 2, 1, 3, 3> zyww;
		float4swizzle<4, float4, 2, 2, 0, 0> zzxx;
		float4swizzle<4, float4, 2, 2, 0, 1> zzxy;
		float4swizzle<4, float4, 2, 2, 0, 2> zzxz;
		float4swizzle<4, float4, 2, 2, 0, 3> zzxw;
		float4swizzle<4, float4, 2, 2, 1, 0> zzyx;
		float4swizzle<4, float4, 2, 2, 1, 1> zzyy;
		float4swizzle<4, float4, 2, 2, 1, 2> zzyz;
		float4swizzle<4, float4, 2, 2, 1, 3> zzyw;
		float4swizzle<4, float4, 2, 2, 2, 0> zzzx;
		float4swizzle<4, float4, 2, 2, 2, 1> zzzy;
		float4swizzle<4, float4, 2, 2, 2, 2> zzzz;
		float4swizzle<4, float4, 2, 2, 2, 3> zzzw;
		float4swizzle<4, float4, 2, 2, 3, 0> zzwx;
		float4swizzle<4, float4, 2, 2, 3, 1> zzwy;
		float4swizzle<4, float4, 2, 2, 3, 2> zzwz;
		float4swizzle<4, float4, 2, 2, 3, 3> zzww;
		float4swizzle<4, float4, 2, 3, 0, 0> zwxx;
		float4swizzle<4, float4, 2, 3, 0, 1> zwxy;
		float4swizzle<4, float4, 2, 3, 0, 2> zwxz;
		float4swizzle<4, float4, 2, 3, 0, 3> zwxw;
		float4swizzle<4, float4, 2, 3, 1, 0> zwyx;
		float4swizzle<4, float4, 2, 3, 1, 1> zwyy;
		float4swizzle<4, float4, 2, 3, 1, 2> zwyz;
		float4swizzle<4, float4, 2, 3, 1, 3> zwyw;
		float4swizzle<4, float4, 2, 3, 2, 0> zwzx;
		float4swizzle<4, float4, 2, 3, 2, 1> zwzy;
		float4swizzle<4, float4, 2, 3, 2, 2> zwzz;
		float4swizzle<4, float4, 2, 3, 2, 3> zwzw;
		float4swizzle<4, float4, 2, 3, 3, 0> zwwx;
		float4swizzle<4, float4, 2, 3, 3, 1> zwwy;
		float4swizzle<4, float4, 2, 3, 3, 2> zwwz;
		float4swizzle<4, float4, 2, 3, 3, 3> zwww;

		float4swizzle<4, float4, 3, 0, 0, 0> wxxx;
		float4swizzle<4, float4, 3, 0, 0, 1> wxxy;
		float4swizzle<4, float4, 3, 0, 0, 2> wxxz;
		float4swizzle<4, float4, 3, 0, 0, 3> wxxw;
		float4swizzle<4, float4, 3, 0, 1, 0> wxyx;
		float4swizzle<4, float4, 3, 0, 1, 1> wxyy;
		float4swizzle<4, float4, 3, 0, 1, 2> wxyz;
		float4swizzle<4, float4, 3, 0, 1, 3> wxyw;
		float4swizzle<4, float4, 3, 0, 2, 0> wxzx;
		float4swizzle<4, float4, 3, 0, 2, 1> wxzy;
		float4swizzle<4, float4, 3, 0, 2, 2> wxzz;
		float4swizzle<4, float4, 3, 0, 2, 3> wxzw;
		float4swizzle<4, float4, 3, 0, 3, 0> wxwx;
		float4swizzle<4, float4, 3, 0, 3, 1> wxwy;
		float4swizzle<4, float4, 3, 0, 3, 2> wxwz;
		float4swizzle<4, float4, 3, 0, 3, 3> wxww;
		float4swizzle<4, float4, 3, 1, 0, 0> wyxx;
		float4swizzle<4, float4, 3, 1, 0, 1> wyxy;
		float4swizzle<4, float4, 3, 1, 0, 2> wyxz;
		float4swizzle<4, float4, 3, 1, 0, 3> wyxw;
		float4swizzle<4, float4, 3, 1, 1, 0> wyyx;
		float4swizzle<4, float4, 3, 1, 1, 1> wyyy;
		float4swizzle<4, float4, 3, 1, 1, 2> wyyz;
		float4swizzle<4, float4, 3, 1, 1, 3> wyyw;
		float4swizzle<4, float4, 3, 1, 2, 0> wyzx;
		float4swizzle<4, float4, 3, 1, 2, 1> wyzy;
		float4swizzle<4, float4, 3, 1, 2, 2> wyzz;
		float4swizzle<4, float4, 3, 1, 2, 3> wyzw;
		float4swizzle<4, float4, 3, 1, 3, 0> wywx;
		float4swizzle<4, float4, 3, 1, 3, 1> wywy;
		float4swizzle<4, float4, 3, 1, 3, 2> wywz;
		float4swizzle<4, float4, 3, 1, 3, 3> wyww;
		float4swizzle<4, float4, 3, 2, 0, 0> wzxx;
		float4swizzle<4, float4, 3, 2, 0, 1> wzxy;
		float4swizzle<4, float4, 3, 2, 0, 2> wzxz;
		float4swizzle<4, float4, 3, 2, 0, 3> wzxw;
		float4swizzle<4, float4, 3, 2, 1, 0> wzyx;
		float4swizzle<4, float4, 3, 2, 1, 1> wzyy;
		float4swizzle<4, float4, 3, 2, 1, 2> wzyz;
		float4swizzle<4, float4, 3, 2, 1, 3> wzyw;
		float4swizzle<4, float4, 3, 2, 2, 0> wzzx;
		float4swizzle<4, float4, 3, 2, 2, 1> wzzy;
		float4swizzle<4, float4, 3, 2, 2, 2> wzzz;
		float4swizzle<4, float4, 3, 2, 2, 3> wzzw;
		float4swizzle<4, float4, 3, 2, 3, 0> wzwx;
		float4swizzle<4, float4, 3, 2, 3, 1> wzwy;
		float4swizzle<4, float4, 3, 2, 3, 2> wzwz;
		float4swizzle<4, float4, 3, 2, 3, 3> wzww;
		float4swizzle<4, float4, 3, 3, 0, 0> wwxx;
		float4swizzle<4, float4, 3, 3, 0, 1> wwxy;
		float4swizzle<4, float4, 3, 3, 0, 2> wwxz;
		float4swizzle<4, float4, 3, 3, 0, 3> wwxw;
		float4swizzle<4, float4, 3, 3, 1, 0> wwyx;
		float4swizzle<4, float4, 3, 3, 1, 1> wwyy;
		float4swizzle<4, float4, 3, 3, 1, 2> wwyz;
		float4swizzle<4, float4, 3, 3, 1, 3> wwyw;
		float4swizzle<4, float4, 3, 3, 2, 0> wwzx;
		float4swizzle<4, float4, 3, 3, 2, 1> wwzy;
		float4swizzle<4, float4, 3, 3, 2, 2> wwzz;
		float4swizzle<4, float4, 3, 3, 2, 3> wwzw;
		float4swizzle<4, float4, 3, 3, 3, 0> wwwx;
		float4swizzle<4, float4, 3, 3, 3, 1> wwwy;
		float4swizzle<4, float4, 3, 3, 3, 2> wwwz;
		float4swizzle<4, float4, 3, 3, 3, 3> wwww;


	};

	float4():x(0.0f),y(0.0f),z(0.0f),w(0.0f){}

	float4(float f):x(f),y(f),z(f),w(f){}

	float4(float x, float y, float z, float w):x(x),y(y),z(z),w(w){}

	float4(float2 xy, float z, float w):x(xy.x),y(xy.y),z(z),w(w){}

	float4(float x, float2 yz, float w):x(x),y(yz.x),z(yz.y),w(w){}

	float4(float x, float y, float2 zw):x(x),y(y),z(zw.x),w(zw.y){}

	float4(float2 xy, float2 zw):x(xy.x),y(xy.y),z(zw.x),w(zw.y){}

	float4(float3 xyz, float w):x(xyz.x),y(xyz.y),z(xyz.z),w(w){}

	float4(float x, float3 yzw):x(x),y(yzw.x),z(yzw.y),w(yzw.z){}

	float4(int4 i):x(i.x),y(i.y),z(i.z),w(i.w){}

	float4& operator+=(const float4& o)
	{
		x += o.x;
		y += o.y;
		z += o.z;
		w += o.w;
		return *this;
	}

	float4& operator-=(const float4& o)
	{
		x -= o.x;
		y -= o.y;
		z -= o.z;
		w -= o.w;
		return *this;
	}

	float4& operator*=(const float4& o)
	{
		x *= o.x;
		y *= o.y;
		z *= o.z;
		w *= o.w;
		return *this;
	}

	float4& operator/=(const float4& o)
	{
		x /= o.x;
		y /= o.y;
		z /= o.z;
		w /= o.w;
		return *this;
	}

	float4& operator%=(const float4& o)
	{
		x = fmodf(x, o.x);
		y = fmodf(y, o.y);
		z = fmodf(z, o.z);
		w = fmodf(w, o.w);
		return *this;
	}

	bool4 operator==(const float4& o) const
	{
		return bool4(x == o.x, y == o.y, z == o.z, w == o.w);
	}

	bool4 operator!=(const float4& o) const
	{
		return bool4(x != o.x, y != o.y, z != o.z, w != o.w);
	}

	bool4 operator<(const float4& o) const
	{
		return bool4(x < o.x, y < o.y, z < o.z, w < o.w);
	}

	bool4 operator<=(const float4& o) const
	{
		return bool4(x <= o.x, y <= o.y, z <= o.z, w <= o.w);
	}

	bool4 operator>(const float4& o) const
	{
		return bool4(x > o.x, y > o.y, z > o.z, w > o.w);
	}

	bool4 operator>=(const float4& o) const
	{
		return bool4(x >= o.x, y >= o.y, z >= o.z, w >= o.w);
	}

	float4 operator+(const float4& o) const
	{
		return float4(x + o.x, y + o.y, z + o.z, w + o.w);
	}

	float4 operator-(const float4& o) const
	{
		return float4(x - o.x, y - o.y, z - o.z, w - o.w);
	}

	float4 operator*(const float4& o) const
	{
		return float4(x * o.x, y * o.y, z * o.z, w * o.w);
	}

	float4 operator/(const float4& o) const
	{
		return float4(x / o.x, y / o.y, z / o.z, w / o.w);
	}

	float4 operator%(const float4& o) const
	{
		return float4(fmodf(x, o.x), fmodf(y, o.y), fmodf(z, o.z), fmodf(w, o.w));
	}


	float4 operator+() const
	{
		return float4(+x, +y, +z, +w);
	}

	float4 operator-() const
	{
		return float4(-x, -y, -z, -w);
	}

	float4 operator!() const
	{
		return float4(-x, -y, -z, +w);
	}

	float operator[](unsigned int i) const
	{
		if(i < 0 || i > 3)
			throw std::range_error("float4 index out of range.");
		return v[i];
	}

	float& operator[](unsigned int i)
	{
		if(i < 0 || i > 3)
			throw std::range_error("float4 index out of range.");
		return v[i];
	}

	DEFINE_FLOAT4_METHOD(abs, float4)
	DEFINE_FLOAT4_METHOD(acos, float4)
	DEFINE_FLOAT4_METHOD(asin, float4)
	DEFINE_FLOAT4_METHOD(atan, float4)
	DEFINE_FLOAT4_METHOD(ceil, int4)
	DEFINE_FLOAT4_METHOD(cos, float4)
	DEFINE_FLOAT4_METHOD(cosh, float4)
	DEFINE_FLOAT4_METHOD(exp, float4)
	DEFINE_FLOAT4_METHOD(floor, int4)

	float4 exp2() const
	{
		return float4( ::pow(2.0f, x), ::pow(2.0f, y), ::pow(2.0f, z), ::pow(2.0f, w));
	}

	DEFINE_FLOAT4_METHOD_DIADIC(atan2, float4)
	DEFINE_FLOAT4_METHOD_DIADIC(fmod, float4)

	float distance(const float4& o) const
	{
		return (*this - o).length();
	}

	float dot(const float4& o) const
	{
		return x * o.x + y * o.y + z * o.z + w * o.w;
	}

	float4 frac() const
	{
		return *this - trunc();
	}
/*
	bool4 isfinite() const
	{
		return bool4( ::_finite(x), ::_finite(y), ::_finite(z), ::_finite(w));
	}

	bool4 isinf() const
	{
		return bool4( !::_finite(x), !::_finite(y), !::_finite(z), !::_finite(w));
	}

	bool4 isnan() const
	{
		return bool4( ::_isnan(x), ::_isnan(y), ::_isnan(z), ::_isnan(w));
	}
*/
	float length() const
	{
		return sqrtf( this->dot(*this));
	}

	float lengthSquared() const
	{
		return this->dot(*this);
	}

	float4 lerp(const float4& a, const float4& b) const
	{
		return a * (float4(1, 1, 1, 1) - *this) + b * *this;
	}

	DEFINE_FLOAT4_METHOD(log, float4)
	DEFINE_FLOAT4_METHOD(log10, float4)

	float4 log2() const
	{
		return float4( ::log(x) / ::log(2.0f), ::log(y) / ::log(2.0f), ::log(z) / ::log(2.0f), ::log(w) / ::log(2.0f));
	}

	float4 max(const float4& o) const
	{
		return float4( (x>o.x)?x:o.x, (y>o.y)?y:o.y, (z>o.z)?z:o.z, (w>o.w)?w:o.w);
	}

	float4 min(const float4& o) const
	{
		return float4( (x<o.x)?x:o.x, (y<o.y)?y:o.y, (z<o.z)?z:o.z, (w<o.w)?w:o.w);
	}

	float4 normalize() const
	{
		return *this / length();
	}

	DEFINE_FLOAT4_METHOD_DIADIC(pow, float4)

	int4 round() const
	{
		return int4( (int)(x+0.5f), (int)(y+0.5f), (int)(z+0.5f), (int)(w+0.5f));
	}

	float4 rsqrt() const
	{
		return float4( 1.0f / ::sqrtf(x), 1.0f / ::sqrtf(y), 1.0f / ::sqrtf(z), 1.0f / ::sqrtf(w));
	}

	float4 saturate() const
	{
		return clamp( float4(0, 0, 0, 0), float4(1, 1, 1, 1));
	}

	float4 sign() const
	{
		return float4( (x>0.0f)?1.0f:((x<0.0f)?-1.0f:0.0f), (y>0.0f)?1.0f:((y<0.0f)?-1.0f:0.0f), (z>0.0f)?1.0f:((z<0.0f)?-1.0f:0.0f), (w>0.0f)?1.0f:((w<0.0f)?-1.0f:0.0f));
	}

	DEFINE_FLOAT4_METHOD(sin, float4)
	DEFINE_FLOAT4_METHOD(sinh, float4)

	float4 smoothstep(const float4& a, const float4& b) const
	{
		float4 r = ((*this - a)/(b - a)).saturate(); 
		return r*r*(float4(3,3,3,3) - r*2);
	}

	float4 sqrt() const
	{
		return float4( ::sqrtf(x), ::sqrtf(y), ::sqrtf(z), ::sqrtf(w));
	}

	float4 step(const float4& o) const
	{
		return float4( (x>=o.x)?1:0, (y>=o.y)?1:0, (z>=o.z)?1:0, (w>=o.w)?1:0);
	}

	DEFINE_FLOAT4_METHOD(tan, float4)

	int4 trunc() const
	{
		return int4( (int)(x), (int)(y), (int)(z), (int)(w));
	}

	float4 clamp(const float4& low, const float4& high) const
	{
		return float4( (x<low.x)?low.x:((x>high.x)?high.x:x), (y<low.y)?low.y:((y>high.y)?high.y:y), (z<low.z)?low.z:((z>high.z)?high.z:z), (w<low.w)?low.w:((w>high.w)?high.w:w));
	}

	float4 quatMul(const float4& o) const
	{
		return float4(
			y*o.z-z*o.y + w*o.x + x*o.w,
			z*o.x-x*o.z + w*o.y + y*o.w,
			x*o.y-y*o.x + w*o.z + z*o.w,
			w*o.w  - (x*o.x + y*o.y + z*o.z) );
	}

	float4 quatExp()
	{
		float3 vv = xyz;
		float theta = vv.length();
		return float4(vv * (sinf(theta) / theta), cosf(theta)) * ::exp(w);
	}

	float4 quatLn()
	{
		float lq = length();
		float3 vv = xyz;
		float3 lv = vv.length();
		return float4(vv / (lv * ::cosf(w / lq) ), ::logf(lq));
	}

	static float4 quatAxisAngle(float3 axis, float angle)
	{
		return float4(axis.normalize() * sinf(angle/2), cosf(angle/2));
	}

	static float4 random(float lower=0.0f, float upper=1.0f)
	{
		float range = upper - lower;
		return float4(
			rand() * range / RAND_MAX + lower,
			rand() * range / RAND_MAX + lower,
			rand() * range / RAND_MAX + lower,
			rand() * range / RAND_MAX + lower);
	}

	static const float4 zero;
	static const float4 xUnit;
	static const float4 yUnit;
	static const float4 zUnit;
	static const float4 wUnit;
	static const float4 one;

	static const float4 identity;

};

}}
