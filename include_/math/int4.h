/*
 * -------------------------------------------------------------------------
 * FILE:    int4.h
 * AUTHOR:  Dr. Szécsi László - szecsi@iit.bme.hu
 * -------------------------------------------------------------------------
 */
 #pragma once

#include <math.h>

namespace pttoth { namespace math {

class int2;
class int3;

class int4
{
public:
	union{
		struct {
			int x;
			int y;
			int z;
			int w;
		};

		int v[4];

		intswizzle<4, int2, bool2, 0, 0> xx;
		intswizzle<4, int2, bool2, 0, 1> xy;
		intswizzle<4, int2, bool2, 0, 2> xz;
		intswizzle<4, int2, bool2, 0, 3> xw;
		intswizzle<4, int2, bool2, 1, 0> yx;
		intswizzle<4, int2, bool2, 1, 1> yy;
		intswizzle<4, int2, bool2, 1, 2> yz;
		intswizzle<4, int2, bool2, 1, 3> yw;
		intswizzle<4, int2, bool2, 2, 0> zx;
		intswizzle<4, int2, bool2, 2, 1> zy;
		intswizzle<4, int2, bool2, 2, 2> zz;
		intswizzle<4, int2, bool2, 2, 3> zw;
		intswizzle<4, int2, bool2, 3, 0> wx;
		intswizzle<4, int2, bool2, 3, 1> wy;
		intswizzle<4, int2, bool2, 3, 2> wz;
		intswizzle<4, int2, bool2, 3, 3> ww;

		intswizzle<4, int3, bool3, 0, 0, 0> xxx;
		intswizzle<4, int3, bool3, 0, 0, 1> xxy;
		intswizzle<4, int3, bool3, 0, 0, 2> xxz;
		intswizzle<4, int3, bool3, 0, 0, 3> xxw;
		intswizzle<4, int3, bool3, 0, 1, 0> xyx;
		intswizzle<4, int3, bool3, 0, 1, 1> xyy;
		intswizzle<4, int3, bool3, 0, 1, 2> xyz;
		intswizzle<4, int3, bool3, 0, 1, 3> xyw;
		intswizzle<4, int3, bool3, 0, 2, 0> xzx;
		intswizzle<4, int3, bool3, 0, 2, 1> xzy;
		intswizzle<4, int3, bool3, 0, 2, 2> xzz;
		intswizzle<4, int3, bool3, 0, 2, 3> xzw;
		intswizzle<4, int3, bool3, 0, 3, 0> xwx;
		intswizzle<4, int3, bool3, 0, 3, 1> xwy;
		intswizzle<4, int3, bool3, 0, 3, 2> xwz;
		intswizzle<4, int3, bool3, 0, 3, 3> xww;

		intswizzle<4, int3, bool3, 1, 0, 0> yxx;
		intswizzle<4, int3, bool3, 1, 0, 1> yxy;
		intswizzle<4, int3, bool3, 1, 0, 2> yxz;
		intswizzle<4, int3, bool3, 1, 0, 3> yxw;
		intswizzle<4, int3, bool3, 1, 1, 0> yyx;
		intswizzle<4, int3, bool3, 1, 1, 1> yyy;
		intswizzle<4, int3, bool3, 1, 1, 2> yyz;
		intswizzle<4, int3, bool3, 1, 1, 3> yyw;
		intswizzle<4, int3, bool3, 1, 2, 0> yzx;
		intswizzle<4, int3, bool3, 1, 2, 1> yzy;
		intswizzle<4, int3, bool3, 1, 2, 2> yzz;
		intswizzle<4, int3, bool3, 1, 2, 3> yzw;
		intswizzle<4, int3, bool3, 1, 3, 0> ywx;
		intswizzle<4, int3, bool3, 1, 3, 1> ywy;
		intswizzle<4, int3, bool3, 1, 3, 2> ywz;
		intswizzle<4, int3, bool3, 1, 3, 3> yww;

		intswizzle<4, int3, bool3, 2, 0, 0> zxx;
		intswizzle<4, int3, bool3, 2, 0, 1> zxy;
		intswizzle<4, int3, bool3, 2, 0, 2> zxz;
		intswizzle<4, int3, bool3, 2, 0, 3> zxw;
		intswizzle<4, int3, bool3, 2, 1, 0> zyx;
		intswizzle<4, int3, bool3, 2, 1, 1> zyy;
		intswizzle<4, int3, bool3, 2, 1, 2> zyz;
		intswizzle<4, int3, bool3, 2, 1, 3> zyw;
		intswizzle<4, int3, bool3, 2, 2, 0> zzx;
		intswizzle<4, int3, bool3, 2, 2, 1> zzy;
		intswizzle<4, int3, bool3, 2, 2, 2> zzz;
		intswizzle<4, int3, bool3, 2, 2, 3> zzw;
		intswizzle<4, int3, bool3, 2, 3, 0> zwx;
		intswizzle<4, int3, bool3, 2, 3, 1> zwy;
		intswizzle<4, int3, bool3, 2, 3, 2> zwz;
		intswizzle<4, int3, bool3, 2, 3, 3> zww;

		intswizzle<4, int3, bool3, 3, 0, 0> wxx;
		intswizzle<4, int3, bool3, 3, 0, 1> wxy;
		intswizzle<4, int3, bool3, 3, 0, 2> wxz;
		intswizzle<4, int3, bool3, 3, 0, 3> wxw;
		intswizzle<4, int3, bool3, 3, 1, 0> wyx;
		intswizzle<4, int3, bool3, 3, 1, 1> wyy;
		intswizzle<4, int3, bool3, 3, 1, 2> wyz;
		intswizzle<4, int3, bool3, 3, 1, 3> wyw;
		intswizzle<4, int3, bool3, 3, 2, 0> wzx;
		intswizzle<4, int3, bool3, 3, 2, 1> wzy;
		intswizzle<4, int3, bool3, 3, 2, 2> wzz;
		intswizzle<4, int3, bool3, 3, 2, 3> wzw;
		intswizzle<4, int3, bool3, 3, 3, 0> wwx;
		intswizzle<4, int3, bool3, 3, 3, 1> wwy;
		intswizzle<4, int3, bool3, 3, 3, 2> wwz;
		intswizzle<4, int3, bool3, 3, 3, 3> www;

		intswizzle<4, int4, bool4, 0, 0, 0, 0> xxxx;
		intswizzle<4, int4, bool4, 0, 0, 0, 1> xxxy;
		intswizzle<4, int4, bool4, 0, 0, 0, 2> xxxz;
		intswizzle<4, int4, bool4, 0, 0, 0, 3> xxxw;
		intswizzle<4, int4, bool4, 0, 0, 1, 0> xxyx;
		intswizzle<4, int4, bool4, 0, 0, 1, 1> xxyy;
		intswizzle<4, int4, bool4, 0, 0, 1, 2> xxyz;
		intswizzle<4, int4, bool4, 0, 0, 1, 3> xxyw;
		intswizzle<4, int4, bool4, 0, 0, 2, 0> xxzx;
		intswizzle<4, int4, bool4, 0, 0, 2, 1> xxzy;
		intswizzle<4, int4, bool4, 0, 0, 2, 2> xxzz;
		intswizzle<4, int4, bool4, 0, 0, 2, 3> xxzw;
		intswizzle<4, int4, bool4, 0, 0, 3, 0> xxwx;
		intswizzle<4, int4, bool4, 0, 0, 3, 1> xxwy;
		intswizzle<4, int4, bool4, 0, 0, 3, 2> xxwz;
		intswizzle<4, int4, bool4, 0, 0, 3, 3> xxww;
		intswizzle<4, int4, bool4, 0, 1, 0, 0> xyxx;
		intswizzle<4, int4, bool4, 0, 1, 0, 1> xyxy;
		intswizzle<4, int4, bool4, 0, 1, 0, 2> xyxz;
		intswizzle<4, int4, bool4, 0, 1, 0, 3> xyxw;
		intswizzle<4, int4, bool4, 0, 1, 1, 0> xyyx;
		intswizzle<4, int4, bool4, 0, 1, 1, 1> xyyy;
		intswizzle<4, int4, bool4, 0, 1, 1, 2> xyyz;
		intswizzle<4, int4, bool4, 0, 1, 1, 3> xyyw;
		intswizzle<4, int4, bool4, 0, 1, 2, 0> xyzx;
		intswizzle<4, int4, bool4, 0, 1, 2, 1> xyzy;
		intswizzle<4, int4, bool4, 0, 1, 2, 2> xyzz;
		intswizzle<4, int4, bool4, 0, 1, 2, 3> xyzw;
		intswizzle<4, int4, bool4, 0, 1, 3, 0> xywx;
		intswizzle<4, int4, bool4, 0, 1, 3, 1> xywy;
		intswizzle<4, int4, bool4, 0, 1, 3, 2> xywz;
		intswizzle<4, int4, bool4, 0, 1, 3, 3> xyww;
		intswizzle<4, int4, bool4, 0, 2, 0, 0> xzxx;
		intswizzle<4, int4, bool4, 0, 2, 0, 1> xzxy;
		intswizzle<4, int4, bool4, 0, 2, 0, 2> xzxz;
		intswizzle<4, int4, bool4, 0, 2, 0, 3> xzxw;
		intswizzle<4, int4, bool4, 0, 2, 1, 0> xzyx;
		intswizzle<4, int4, bool4, 0, 2, 1, 1> xzyy;
		intswizzle<4, int4, bool4, 0, 2, 1, 2> xzyz;
		intswizzle<4, int4, bool4, 0, 2, 1, 3> xzyw;
		intswizzle<4, int4, bool4, 0, 2, 2, 0> xzzx;
		intswizzle<4, int4, bool4, 0, 2, 2, 1> xzzy;
		intswizzle<4, int4, bool4, 0, 2, 2, 2> xzzz;
		intswizzle<4, int4, bool4, 0, 2, 2, 3> xzzw;
		intswizzle<4, int4, bool4, 0, 2, 3, 0> xzwx;
		intswizzle<4, int4, bool4, 0, 2, 3, 1> xzwy;
		intswizzle<4, int4, bool4, 0, 2, 3, 2> xzwz;
		intswizzle<4, int4, bool4, 0, 2, 3, 3> xzww;
		intswizzle<4, int4, bool4, 0, 3, 0, 0> xwxx;
		intswizzle<4, int4, bool4, 0, 3, 0, 1> xwxy;
		intswizzle<4, int4, bool4, 0, 3, 0, 2> xwxz;
		intswizzle<4, int4, bool4, 0, 3, 0, 3> xwxw;
		intswizzle<4, int4, bool4, 0, 3, 1, 0> xwyx;
		intswizzle<4, int4, bool4, 0, 3, 1, 1> xwyy;
		intswizzle<4, int4, bool4, 0, 3, 1, 2> xwyz;
		intswizzle<4, int4, bool4, 0, 3, 1, 3> xwyw;
		intswizzle<4, int4, bool4, 0, 3, 2, 0> xwzx;
		intswizzle<4, int4, bool4, 0, 3, 2, 1> xwzy;
		intswizzle<4, int4, bool4, 0, 3, 2, 2> xwzz;
		intswizzle<4, int4, bool4, 0, 3, 2, 3> xwzw;
		intswizzle<4, int4, bool4, 0, 3, 3, 0> xwwx;
		intswizzle<4, int4, bool4, 0, 3, 3, 1> xwwy;
		intswizzle<4, int4, bool4, 0, 3, 3, 2> xwwz;
		intswizzle<4, int4, bool4, 0, 3, 3, 3> xwww;

		intswizzle<4, int4, bool4, 1, 0, 0, 0> yxxx;
		intswizzle<4, int4, bool4, 1, 0, 0, 1> yxxy;
		intswizzle<4, int4, bool4, 1, 0, 0, 2> yxxz;
		intswizzle<4, int4, bool4, 1, 0, 0, 3> yxxw;
		intswizzle<4, int4, bool4, 1, 0, 1, 0> yxyx;
		intswizzle<4, int4, bool4, 1, 0, 1, 1> yxyy;
		intswizzle<4, int4, bool4, 1, 0, 1, 2> yxyz;
		intswizzle<4, int4, bool4, 1, 0, 1, 3> yxyw;
		intswizzle<4, int4, bool4, 1, 0, 2, 0> yxzx;
		intswizzle<4, int4, bool4, 1, 0, 2, 1> yxzy;
		intswizzle<4, int4, bool4, 1, 0, 2, 2> yxzz;
		intswizzle<4, int4, bool4, 1, 0, 2, 3> yxzw;
		intswizzle<4, int4, bool4, 1, 0, 3, 0> yxwx;
		intswizzle<4, int4, bool4, 1, 0, 3, 1> yxwy;
		intswizzle<4, int4, bool4, 1, 0, 3, 2> yxwz;
		intswizzle<4, int4, bool4, 1, 0, 3, 3> yxww;
		intswizzle<4, int4, bool4, 1, 1, 0, 0> yyxx;
		intswizzle<4, int4, bool4, 1, 1, 0, 1> yyxy;
		intswizzle<4, int4, bool4, 1, 1, 0, 2> yyxz;
		intswizzle<4, int4, bool4, 1, 1, 0, 3> yyxw;
		intswizzle<4, int4, bool4, 1, 1, 1, 0> yyyx;
		intswizzle<4, int4, bool4, 1, 1, 1, 1> yyyy;
		intswizzle<4, int4, bool4, 1, 1, 1, 2> yyyz;
		intswizzle<4, int4, bool4, 1, 1, 1, 3> yyyw;
		intswizzle<4, int4, bool4, 1, 1, 2, 0> yyzx;
		intswizzle<4, int4, bool4, 1, 1, 2, 1> yyzy;
		intswizzle<4, int4, bool4, 1, 1, 2, 2> yyzz;
		intswizzle<4, int4, bool4, 1, 1, 2, 3> yyzw;
		intswizzle<4, int4, bool4, 1, 1, 3, 0> yywx;
		intswizzle<4, int4, bool4, 1, 1, 3, 1> yywy;
		intswizzle<4, int4, bool4, 1, 1, 3, 2> yywz;
		intswizzle<4, int4, bool4, 1, 1, 3, 3> yyww;
		intswizzle<4, int4, bool4, 1, 2, 0, 0> yzxx;
		intswizzle<4, int4, bool4, 1, 2, 0, 1> yzxy;
		intswizzle<4, int4, bool4, 1, 2, 0, 2> yzxz;
		intswizzle<4, int4, bool4, 1, 2, 0, 3> yzxw;
		intswizzle<4, int4, bool4, 1, 2, 1, 0> yzyx;
		intswizzle<4, int4, bool4, 1, 2, 1, 1> yzyy;
		intswizzle<4, int4, bool4, 1, 2, 1, 2> yzyz;
		intswizzle<4, int4, bool4, 1, 2, 1, 3> yzyw;
		intswizzle<4, int4, bool4, 1, 2, 2, 0> yzzx;
		intswizzle<4, int4, bool4, 1, 2, 2, 1> yzzy;
		intswizzle<4, int4, bool4, 1, 2, 2, 2> yzzz;
		intswizzle<4, int4, bool4, 1, 2, 2, 3> yzzw;
		intswizzle<4, int4, bool4, 1, 2, 3, 0> yzwx;
		intswizzle<4, int4, bool4, 1, 2, 3, 1> yzwy;
		intswizzle<4, int4, bool4, 1, 2, 3, 2> yzwz;
		intswizzle<4, int4, bool4, 1, 2, 3, 3> yzww;
		intswizzle<4, int4, bool4, 1, 3, 0, 0> ywxx;
		intswizzle<4, int4, bool4, 1, 3, 0, 1> ywxy;
		intswizzle<4, int4, bool4, 1, 3, 0, 2> ywxz;
		intswizzle<4, int4, bool4, 1, 3, 0, 3> ywxw;
		intswizzle<4, int4, bool4, 1, 3, 1, 0> ywyx;
		intswizzle<4, int4, bool4, 1, 3, 1, 1> ywyy;
		intswizzle<4, int4, bool4, 1, 3, 1, 2> ywyz;
		intswizzle<4, int4, bool4, 1, 3, 1, 3> ywyw;
		intswizzle<4, int4, bool4, 1, 3, 2, 0> ywzx;
		intswizzle<4, int4, bool4, 1, 3, 2, 1> ywzy;
		intswizzle<4, int4, bool4, 1, 3, 2, 2> ywzz;
		intswizzle<4, int4, bool4, 1, 3, 2, 3> ywzw;
		intswizzle<4, int4, bool4, 1, 3, 3, 0> ywwx;
		intswizzle<4, int4, bool4, 1, 3, 3, 1> ywwy;
		intswizzle<4, int4, bool4, 1, 3, 3, 2> ywwz;
		intswizzle<4, int4, bool4, 1, 3, 3, 3> ywww;

		intswizzle<4, int4, bool4, 2, 0, 0, 0> zxxx;
		intswizzle<4, int4, bool4, 2, 0, 0, 1> zxxy;
		intswizzle<4, int4, bool4, 2, 0, 0, 2> zxxz;
		intswizzle<4, int4, bool4, 2, 0, 0, 3> zxxw;
		intswizzle<4, int4, bool4, 2, 0, 1, 0> zxyx;
		intswizzle<4, int4, bool4, 2, 0, 1, 1> zxyy;
		intswizzle<4, int4, bool4, 2, 0, 1, 2> zxyz;
		intswizzle<4, int4, bool4, 2, 0, 1, 3> zxyw;
		intswizzle<4, int4, bool4, 2, 0, 2, 0> zxzx;
		intswizzle<4, int4, bool4, 2, 0, 2, 1> zxzy;
		intswizzle<4, int4, bool4, 2, 0, 2, 2> zxzz;
		intswizzle<4, int4, bool4, 2, 0, 2, 3> zxzw;
		intswizzle<4, int4, bool4, 2, 0, 3, 0> zxwx;
		intswizzle<4, int4, bool4, 2, 0, 3, 1> zxwy;
		intswizzle<4, int4, bool4, 2, 0, 3, 2> zxwz;
		intswizzle<4, int4, bool4, 2, 0, 3, 3> zxww;
		intswizzle<4, int4, bool4, 2, 1, 0, 0> zyxx;
		intswizzle<4, int4, bool4, 2, 1, 0, 1> zyxy;
		intswizzle<4, int4, bool4, 2, 1, 0, 2> zyxz;
		intswizzle<4, int4, bool4, 2, 1, 0, 3> zyxw;
		intswizzle<4, int4, bool4, 2, 1, 1, 0> zyyx;
		intswizzle<4, int4, bool4, 2, 1, 1, 1> zyyy;
		intswizzle<4, int4, bool4, 2, 1, 1, 2> zyyz;
		intswizzle<4, int4, bool4, 2, 1, 1, 3> zyyw;
		intswizzle<4, int4, bool4, 2, 1, 2, 0> zyzx;
		intswizzle<4, int4, bool4, 2, 1, 2, 1> zyzy;
		intswizzle<4, int4, bool4, 2, 1, 2, 2> zyzz;
		intswizzle<4, int4, bool4, 2, 1, 2, 3> zyzw;
		intswizzle<4, int4, bool4, 2, 1, 3, 0> zywx;
		intswizzle<4, int4, bool4, 2, 1, 3, 1> zywy;
		intswizzle<4, int4, bool4, 2, 1, 3, 2> zywz;
		intswizzle<4, int4, bool4, 2, 1, 3, 3> zyww;
		intswizzle<4, int4, bool4, 2, 2, 0, 0> zzxx;
		intswizzle<4, int4, bool4, 2, 2, 0, 1> zzxy;
		intswizzle<4, int4, bool4, 2, 2, 0, 2> zzxz;
		intswizzle<4, int4, bool4, 2, 2, 0, 3> zzxw;
		intswizzle<4, int4, bool4, 2, 2, 1, 0> zzyx;
		intswizzle<4, int4, bool4, 2, 2, 1, 1> zzyy;
		intswizzle<4, int4, bool4, 2, 2, 1, 2> zzyz;
		intswizzle<4, int4, bool4, 2, 2, 1, 3> zzyw;
		intswizzle<4, int4, bool4, 2, 2, 2, 0> zzzx;
		intswizzle<4, int4, bool4, 2, 2, 2, 1> zzzy;
		intswizzle<4, int4, bool4, 2, 2, 2, 2> zzzz;
		intswizzle<4, int4, bool4, 2, 2, 2, 3> zzzw;
		intswizzle<4, int4, bool4, 2, 2, 3, 0> zzwx;
		intswizzle<4, int4, bool4, 2, 2, 3, 1> zzwy;
		intswizzle<4, int4, bool4, 2, 2, 3, 2> zzwz;
		intswizzle<4, int4, bool4, 2, 2, 3, 3> zzww;
		intswizzle<4, int4, bool4, 2, 3, 0, 0> zwxx;
		intswizzle<4, int4, bool4, 2, 3, 0, 1> zwxy;
		intswizzle<4, int4, bool4, 2, 3, 0, 2> zwxz;
		intswizzle<4, int4, bool4, 2, 3, 0, 3> zwxw;
		intswizzle<4, int4, bool4, 2, 3, 1, 0> zwyx;
		intswizzle<4, int4, bool4, 2, 3, 1, 1> zwyy;
		intswizzle<4, int4, bool4, 2, 3, 1, 2> zwyz;
		intswizzle<4, int4, bool4, 2, 3, 1, 3> zwyw;
		intswizzle<4, int4, bool4, 2, 3, 2, 0> zwzx;
		intswizzle<4, int4, bool4, 2, 3, 2, 1> zwzy;
		intswizzle<4, int4, bool4, 2, 3, 2, 2> zwzz;
		intswizzle<4, int4, bool4, 2, 3, 2, 3> zwzw;
		intswizzle<4, int4, bool4, 2, 3, 3, 0> zwwx;
		intswizzle<4, int4, bool4, 2, 3, 3, 1> zwwy;
		intswizzle<4, int4, bool4, 2, 3, 3, 2> zwwz;
		intswizzle<4, int4, bool4, 2, 3, 3, 3> zwww;

		intswizzle<4, int4, bool4, 3, 0, 0, 0> wxxx;
		intswizzle<4, int4, bool4, 3, 0, 0, 1> wxxy;
		intswizzle<4, int4, bool4, 3, 0, 0, 2> wxxz;
		intswizzle<4, int4, bool4, 3, 0, 0, 3> wxxw;
		intswizzle<4, int4, bool4, 3, 0, 1, 0> wxyx;
		intswizzle<4, int4, bool4, 3, 0, 1, 1> wxyy;
		intswizzle<4, int4, bool4, 3, 0, 1, 2> wxyz;
		intswizzle<4, int4, bool4, 3, 0, 1, 3> wxyw;
		intswizzle<4, int4, bool4, 3, 0, 2, 0> wxzx;
		intswizzle<4, int4, bool4, 3, 0, 2, 1> wxzy;
		intswizzle<4, int4, bool4, 3, 0, 2, 2> wxzz;
		intswizzle<4, int4, bool4, 3, 0, 2, 3> wxzw;
		intswizzle<4, int4, bool4, 3, 0, 3, 0> wxwx;
		intswizzle<4, int4, bool4, 3, 0, 3, 1> wxwy;
		intswizzle<4, int4, bool4, 3, 0, 3, 2> wxwz;
		intswizzle<4, int4, bool4, 3, 0, 3, 3> wxww;
		intswizzle<4, int4, bool4, 3, 1, 0, 0> wyxx;
		intswizzle<4, int4, bool4, 3, 1, 0, 1> wyxy;
		intswizzle<4, int4, bool4, 3, 1, 0, 2> wyxz;
		intswizzle<4, int4, bool4, 3, 1, 0, 3> wyxw;
		intswizzle<4, int4, bool4, 3, 1, 1, 0> wyyx;
		intswizzle<4, int4, bool4, 3, 1, 1, 1> wyyy;
		intswizzle<4, int4, bool4, 3, 1, 1, 2> wyyz;
		intswizzle<4, int4, bool4, 3, 1, 1, 3> wyyw;
		intswizzle<4, int4, bool4, 3, 1, 2, 0> wyzx;
		intswizzle<4, int4, bool4, 3, 1, 2, 1> wyzy;
		intswizzle<4, int4, bool4, 3, 1, 2, 2> wyzz;
		intswizzle<4, int4, bool4, 3, 1, 2, 3> wyzw;
		intswizzle<4, int4, bool4, 3, 1, 3, 0> wywx;
		intswizzle<4, int4, bool4, 3, 1, 3, 1> wywy;
		intswizzle<4, int4, bool4, 3, 1, 3, 2> wywz;
		intswizzle<4, int4, bool4, 3, 1, 3, 3> wyww;
		intswizzle<4, int4, bool4, 3, 2, 0, 0> wzxx;
		intswizzle<4, int4, bool4, 3, 2, 0, 1> wzxy;
		intswizzle<4, int4, bool4, 3, 2, 0, 2> wzxz;
		intswizzle<4, int4, bool4, 3, 2, 0, 3> wzxw;
		intswizzle<4, int4, bool4, 3, 2, 1, 0> wzyx;
		intswizzle<4, int4, bool4, 3, 2, 1, 1> wzyy;
		intswizzle<4, int4, bool4, 3, 2, 1, 2> wzyz;
		intswizzle<4, int4, bool4, 3, 2, 1, 3> wzyw;
		intswizzle<4, int4, bool4, 3, 2, 2, 0> wzzx;
		intswizzle<4, int4, bool4, 3, 2, 2, 1> wzzy;
		intswizzle<4, int4, bool4, 3, 2, 2, 2> wzzz;
		intswizzle<4, int4, bool4, 3, 2, 2, 3> wzzw;
		intswizzle<4, int4, bool4, 3, 2, 3, 0> wzwx;
		intswizzle<4, int4, bool4, 3, 2, 3, 1> wzwy;
		intswizzle<4, int4, bool4, 3, 2, 3, 2> wzwz;
		intswizzle<4, int4, bool4, 3, 2, 3, 3> wzww;
		intswizzle<4, int4, bool4, 3, 3, 0, 0> wwxx;
		intswizzle<4, int4, bool4, 3, 3, 0, 1> wwxy;
		intswizzle<4, int4, bool4, 3, 3, 0, 2> wwxz;
		intswizzle<4, int4, bool4, 3, 3, 0, 3> wwxw;
		intswizzle<4, int4, bool4, 3, 3, 1, 0> wwyx;
		intswizzle<4, int4, bool4, 3, 3, 1, 1> wwyy;
		intswizzle<4, int4, bool4, 3, 3, 1, 2> wwyz;
		intswizzle<4, int4, bool4, 3, 3, 1, 3> wwyw;
		intswizzle<4, int4, bool4, 3, 3, 2, 0> wwzx;
		intswizzle<4, int4, bool4, 3, 3, 2, 1> wwzy;
		intswizzle<4, int4, bool4, 3, 3, 2, 2> wwzz;
		intswizzle<4, int4, bool4, 3, 3, 2, 3> wwzw;
		intswizzle<4, int4, bool4, 3, 3, 3, 0> wwwx;
		intswizzle<4, int4, bool4, 3, 3, 3, 1> wwwy;
		intswizzle<4, int4, bool4, 3, 3, 3, 2> wwwz;
		intswizzle<4, int4, bool4, 3, 3, 3, 3> wwww;


	};

	int4():x(0),y(0),z(0),w(0){}

	int4(int i):x(i),y(i),z(i),w(i){}

	int4(int x, int y, int z, int w):x(x),y(y),z(z),w(w){}

	int4(int2 xy, int z, int w):x(xy.y),y(xy.y),z(z),w(w){}

	int4(int x, int2 yz, int w):x(x),y(yz.x),z(yz.y),w(w){}

	int4(int x, int y, int2 zw):x(x),y(y),z(zw.x),w(zw.y){}

	int4(int2 xy, int2 zw):x(xy.y),y(xy.y),z(zw.x),w(zw.y){}

	int4(int3 xyz, int w):x(xyz.x),y(xyz.y),z(xyz.z),w(w){}

	int4(int x, int3 yzw):x(x),y(yzw.x),z(yzw.y),w(yzw.z){}

	int4(bool4 b):x(b.x),y(b.y),z(b.z),w(b.w){}

	int4& operator+=(const int4& o)
	{
		x += o.x;
		y += o.y;
		z += o.z;
		w += o.w;
		return *this;
	}

	int4& operator-=(const int4& o)
	{
		x -= o.x;
		y -= o.y;
		z -= o.z;
		w -= o.w;
		return *this;
	}

	int4& operator*=(const int4& o)
	{
		x *= o.x;
		y *= o.y;
		z *= o.z;
		w *= o.w;
		return *this;
	}

	int4& operator/=(const int4& o)
	{
		x /= o.x;
		y /= o.y;
		z /= o.z;
		w /= o.w;
		return *this;
	}

	int4& operator%=(const int4& o)
	{
		x %= o.x;
		y %= o.y;
		z %= o.z;
		w %= o.w;
		return *this;
	}

	int4& operator>>=(const int4& o)
	{
		x >>= o.x;
		y >>= o.y;
		z >>= o.z;
		w >>= o.w;
		return *this;
	}

	int4& operator<<=(const int4& o)
	{
		x <<= o.x;
		y <<= o.y;
		z <<= o.z;
		w <<= o.w;
		return *this;
	}

	int4& operator&=(const int4& o)
	{
		x &= o.x;
		y &= o.y;
		z &= o.z;
		w &= o.w;
		return *this;
	}

	int4& operator|=(const int4& o)
	{
		x |= o.x;
		y |= o.y;
		z |= o.z;
		w |= o.w;
		return *this;
	}

	int4 operator&(const int4& o) const
	{
		return int4(x & o.x, y & o.y, z & o.z, w & o.w);
	}

	int4 operator&&(const int4& o) const
	{
		return int4(x && o.x, y && o.y, z && o.z, w && o.w);
	}

	int4 operator|	(const int4& o) const
	{
		return int4(x | o.x, y | o.y, z | o.z, w || o.w);
	}

	int4 operator||(const int4& o) const
	{
		return int4(x || o.x, y || o.y, z || o.z, w || o.w);
	}

	bool4 operator==(const int4& o) const
	{
		return bool4(x == o.x, y == o.y, z == o.z, w == o.w);
	}

	bool4 operator!=(const int4& o) const
	{
		return bool4(x != o.x, y != o.y, z != o.z, w != o.w);
	}

	bool4 operator<(const int4& o) const
	{
		return bool4(x < o.x, y < o.y, z < o.z, w < o.w);
	}

	bool4 operator>(const int4& o) const
	{
		return bool4(x > o.x, y > o.y, z > o.z, w > o.w);
	}

	bool4 operator<=(const int4& o) const
	{
		return bool4(x <= o.x, y <= o.y, z <= o.z, w <= o.w);
	}

	bool4 operator>=(const int4& o) const
	{
		return bool4(x >= o.x, y >= o.y, z >= o.z, w >= o.w);
	}

	int4 operator<<(const int4& o) const
	{
		return int4(x << o.x, y << o.y, z << o.z, w << o.w);
	}

	int4 operator>>(const int4& o) const
	{
		return int4(x >> o.x, y >> o.y, z >> o.z, w >> o.w);
	}

	int4 operator+(const int4& o) const
	{
		return int4(x + o.x, y + o.y, z + o.z, w + o.w);
	}

	int4 operator-(const int4& o) const
	{
		return int4(x - o.x, y - o.y, z - o.z, w - o.w);
	}

	int4 operator*(const int4& o) const
	{
		return int4(x * o.x, y * o.y, z * o.z, w * o.w);
	}

	int4 operator/(const int4& o) const
	{
		return int4(x / o.x, y / o.y, z / o.z, w / o.w);
	}

	int4 operator%(const int4& o) const
	{
		return int4(x % o.x, y % o.y, z % o.z, w % o.w);
	}

	int4 operator+() const
	{
		return int4(+x, +y, +z, +w);
	}

	int4 operator-() const
	{
		return int4(-x, -y, -z, -w);
	}

	int4 operator!() const
	{
		return int4(!x, !y, !z, !w);
	}

	int4 operator~() const
	{
		return int4(~x, ~y, ~z, ~w);
	}

	int4 operator++()
	{
		return int4(++x, ++y, ++z, ++w);
	}

	int4 operator--()
	{
		return int4(--x, --y, --z, --w);
	}


	int4 operator++(int)
	{
		return int4(x++, y++, z++, w++);
	}
	
	int4 operator--(int)
	{
		return int4(x++, y++, z++, w++);
	}


	int operator[](unsigned int i) const
	{
		if(i < 0 || i > 3)
			throw std::range_error("int4 index out of range.");
		return v[i];
	}

	int& operator[](unsigned int i)
	{
		if(i < 0 || i > 3)
			throw std::range_error("int4 index out of range.");
		return v[i];
	}

	int4 max(const int4& o) const
	{
		return int4( (x>o.x)?x:o.x, (y>o.y)?y:o.y, (z>o.z)?z:o.z, (w>o.w)?w:o.w );
	}

	int4 min(const int4& o) const
	{
		return int4( (x<o.x)?x:o.x, (y<o.y)?y:o.y, (z<o.z)?z:o.z, (w<o.w)?w:o.w);
	}

	static int4 random(int lower=0, int upper=6)
	{
		int range = upper - lower + 1;
		return int4(
			rand() % range + lower,
			rand() % range + lower,
			rand() % range + lower,
			rand() % range + lower);
	}

	static const int4 zero;
	static const int4 xUnit;
	static const int4 yUnit;
	static const int4 zUnit;
	static const int4 wUnit;
	static const int4 one;
};

}}
