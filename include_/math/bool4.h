/*
 * -------------------------------------------------------------------------
 * FILE:    bool4.h
 * AUTHOR:  Dr. Szécsi László - szecsi@iit.bme.hu
 * -------------------------------------------------------------------------
 */
#pragma once

#include <math.h>

namespace pttoth { namespace math {

class bool2;
class bool3;

class bool4
{
public:
	union{
		struct {
			bool x;
			bool y;
			bool z;
			bool w;
		};

		bool v[4];

		boolswizzle<4, bool2, 0, 0> xx;
		boolswizzle<4, bool2, 0, 1> xy;
		boolswizzle<4, bool2, 0, 2> xz;
		boolswizzle<4, bool2, 0, 3> xw;
		boolswizzle<4, bool2, 1, 0> yx;
		boolswizzle<4, bool2, 1, 1> yy;
		boolswizzle<4, bool2, 1, 2> yz;
		boolswizzle<4, bool2, 1, 3> yw;
		boolswizzle<4, bool2, 2, 0> zx;
		boolswizzle<4, bool2, 2, 1> zy;
		boolswizzle<4, bool2, 2, 2> zz;
		boolswizzle<4, bool2, 2, 3> zw;
		boolswizzle<4, bool2, 3, 0> wx;
		boolswizzle<4, bool2, 3, 1> wy;
		boolswizzle<4, bool2, 3, 2> wz;
		boolswizzle<4, bool2, 3, 3> ww;

		boolswizzle<4, bool3, 0, 0, 0> xxx;
		boolswizzle<4, bool3, 0, 0, 1> xxy;
		boolswizzle<4, bool3, 0, 0, 2> xxz;
		boolswizzle<4, bool3, 0, 0, 3> xxw;
		boolswizzle<4, bool3, 0, 1, 0> xyx;
		boolswizzle<4, bool3, 0, 1, 1> xyy;
		boolswizzle<4, bool3, 0, 1, 2> xyz;
		boolswizzle<4, bool3, 0, 1, 3> xyw;
		boolswizzle<4, bool3, 0, 2, 0> xzx;
		boolswizzle<4, bool3, 0, 2, 1> xzy;
		boolswizzle<4, bool3, 0, 2, 2> xzz;
		boolswizzle<4, bool3, 0, 2, 3> xzw;
		boolswizzle<4, bool3, 0, 3, 0> xwx;
		boolswizzle<4, bool3, 0, 3, 1> xwy;
		boolswizzle<4, bool3, 0, 3, 2> xwz;
		boolswizzle<4, bool3, 0, 3, 3> xww;

		boolswizzle<4, bool3, 1, 0, 0> yxx;
		boolswizzle<4, bool3, 1, 0, 1> yxy;
		boolswizzle<4, bool3, 1, 0, 2> yxz;
		boolswizzle<4, bool3, 1, 0, 3> yxw;
		boolswizzle<4, bool3, 1, 1, 0> yyx;
		boolswizzle<4, bool3, 1, 1, 1> yyy;
		boolswizzle<4, bool3, 1, 1, 2> yyz;
		boolswizzle<4, bool3, 1, 1, 3> yyw;
		boolswizzle<4, bool3, 1, 2, 0> yzx;
		boolswizzle<4, bool3, 1, 2, 1> yzy;
		boolswizzle<4, bool3, 1, 2, 2> yzz;
		boolswizzle<4, bool3, 1, 2, 3> yzw;
		boolswizzle<4, bool3, 1, 3, 0> ywx;
		boolswizzle<4, bool3, 1, 3, 1> ywy;
		boolswizzle<4, bool3, 1, 3, 2> ywz;
		boolswizzle<4, bool3, 1, 3, 3> yww;

		boolswizzle<4, bool3, 2, 0, 0> zxx;
		boolswizzle<4, bool3, 2, 0, 1> zxy;
		boolswizzle<4, bool3, 2, 0, 2> zxz;
		boolswizzle<4, bool3, 2, 0, 3> zxw;
		boolswizzle<4, bool3, 2, 1, 0> zyx;
		boolswizzle<4, bool3, 2, 1, 1> zyy;
		boolswizzle<4, bool3, 2, 1, 2> zyz;
		boolswizzle<4, bool3, 2, 1, 3> zyw;
		boolswizzle<4, bool3, 2, 2, 0> zzx;
		boolswizzle<4, bool3, 2, 2, 1> zzy;
		boolswizzle<4, bool3, 2, 2, 2> zzz;
		boolswizzle<4, bool3, 2, 2, 3> zzw;
		boolswizzle<4, bool3, 2, 3, 0> zwx;
		boolswizzle<4, bool3, 2, 3, 1> zwy;
		boolswizzle<4, bool3, 2, 3, 2> zwz;
		boolswizzle<4, bool3, 2, 3, 3> zww;

		boolswizzle<4, bool3, 3, 0, 0> wxx;
		boolswizzle<4, bool3, 3, 0, 1> wxy;
		boolswizzle<4, bool3, 3, 0, 2> wxz;
		boolswizzle<4, bool3, 3, 0, 3> wxw;
		boolswizzle<4, bool3, 3, 1, 0> wyx;
		boolswizzle<4, bool3, 3, 1, 1> wyy;
		boolswizzle<4, bool3, 3, 1, 2> wyz;
		boolswizzle<4, bool3, 3, 1, 3> wyw;
		boolswizzle<4, bool3, 3, 2, 0> wzx;
		boolswizzle<4, bool3, 3, 2, 1> wzy;
		boolswizzle<4, bool3, 3, 2, 2> wzz;
		boolswizzle<4, bool3, 3, 2, 3> wzw;
		boolswizzle<4, bool3, 3, 3, 0> wwx;
		boolswizzle<4, bool3, 3, 3, 1> wwy;
		boolswizzle<4, bool3, 3, 3, 2> wwz;
		boolswizzle<4, bool3, 3, 3, 3> www;

		boolswizzle<4, bool4, 0, 0, 0, 0> xxxx;
		boolswizzle<4, bool4, 0, 0, 0, 1> xxxy;
		boolswizzle<4, bool4, 0, 0, 0, 2> xxxz;
		boolswizzle<4, bool4, 0, 0, 0, 3> xxxw;
		boolswizzle<4, bool4, 0, 0, 1, 0> xxyx;
		boolswizzle<4, bool4, 0, 0, 1, 1> xxyy;
		boolswizzle<4, bool4, 0, 0, 1, 2> xxyz;
		boolswizzle<4, bool4, 0, 0, 1, 3> xxyw;
		boolswizzle<4, bool4, 0, 0, 2, 0> xxzx;
		boolswizzle<4, bool4, 0, 0, 2, 1> xxzy;
		boolswizzle<4, bool4, 0, 0, 2, 2> xxzz;
		boolswizzle<4, bool4, 0, 0, 2, 3> xxzw;
		boolswizzle<4, bool4, 0, 0, 3, 0> xxwx;
		boolswizzle<4, bool4, 0, 0, 3, 1> xxwy;
		boolswizzle<4, bool4, 0, 0, 3, 2> xxwz;
		boolswizzle<4, bool4, 0, 0, 3, 3> xxww;
		boolswizzle<4, bool4, 0, 1, 0, 0> xyxx;
		boolswizzle<4, bool4, 0, 1, 0, 1> xyxy;
		boolswizzle<4, bool4, 0, 1, 0, 2> xyxz;
		boolswizzle<4, bool4, 0, 1, 0, 3> xyxw;
		boolswizzle<4, bool4, 0, 1, 1, 0> xyyx;
		boolswizzle<4, bool4, 0, 1, 1, 1> xyyy;
		boolswizzle<4, bool4, 0, 1, 1, 2> xyyz;
		boolswizzle<4, bool4, 0, 1, 1, 3> xyyw;
		boolswizzle<4, bool4, 0, 1, 2, 0> xyzx;
		boolswizzle<4, bool4, 0, 1, 2, 1> xyzy;
		boolswizzle<4, bool4, 0, 1, 2, 2> xyzz;
		boolswizzle<4, bool4, 0, 1, 2, 3> xyzw;
		boolswizzle<4, bool4, 0, 1, 3, 0> xywx;
		boolswizzle<4, bool4, 0, 1, 3, 1> xywy;
		boolswizzle<4, bool4, 0, 1, 3, 2> xywz;
		boolswizzle<4, bool4, 0, 1, 3, 3> xyww;
		boolswizzle<4, bool4, 0, 2, 0, 0> xzxx;
		boolswizzle<4, bool4, 0, 2, 0, 1> xzxy;
		boolswizzle<4, bool4, 0, 2, 0, 2> xzxz;
		boolswizzle<4, bool4, 0, 2, 0, 3> xzxw;
		boolswizzle<4, bool4, 0, 2, 1, 0> xzyx;
		boolswizzle<4, bool4, 0, 2, 1, 1> xzyy;
		boolswizzle<4, bool4, 0, 2, 1, 2> xzyz;
		boolswizzle<4, bool4, 0, 2, 1, 3> xzyw;
		boolswizzle<4, bool4, 0, 2, 2, 0> xzzx;
		boolswizzle<4, bool4, 0, 2, 2, 1> xzzy;
		boolswizzle<4, bool4, 0, 2, 2, 2> xzzz;
		boolswizzle<4, bool4, 0, 2, 2, 3> xzzw;
		boolswizzle<4, bool4, 0, 2, 3, 0> xzwx;
		boolswizzle<4, bool4, 0, 2, 3, 1> xzwy;
		boolswizzle<4, bool4, 0, 2, 3, 2> xzwz;
		boolswizzle<4, bool4, 0, 2, 3, 3> xzww;
		boolswizzle<4, bool4, 0, 3, 0, 0> xwxx;
		boolswizzle<4, bool4, 0, 3, 0, 1> xwxy;
		boolswizzle<4, bool4, 0, 3, 0, 2> xwxz;
		boolswizzle<4, bool4, 0, 3, 0, 3> xwxw;
		boolswizzle<4, bool4, 0, 3, 1, 0> xwyx;
		boolswizzle<4, bool4, 0, 3, 1, 1> xwyy;
		boolswizzle<4, bool4, 0, 3, 1, 2> xwyz;
		boolswizzle<4, bool4, 0, 3, 1, 3> xwyw;
		boolswizzle<4, bool4, 0, 3, 2, 0> xwzx;
		boolswizzle<4, bool4, 0, 3, 2, 1> xwzy;
		boolswizzle<4, bool4, 0, 3, 2, 2> xwzz;
		boolswizzle<4, bool4, 0, 3, 2, 3> xwzw;
		boolswizzle<4, bool4, 0, 3, 3, 0> xwwx;
		boolswizzle<4, bool4, 0, 3, 3, 1> xwwy;
		boolswizzle<4, bool4, 0, 3, 3, 2> xwwz;
		boolswizzle<4, bool4, 0, 3, 3, 3> xwww;

		boolswizzle<4, bool4, 1, 0, 0, 0> yxxx;
		boolswizzle<4, bool4, 1, 0, 0, 1> yxxy;
		boolswizzle<4, bool4, 1, 0, 0, 2> yxxz;
		boolswizzle<4, bool4, 1, 0, 0, 3> yxxw;
		boolswizzle<4, bool4, 1, 0, 1, 0> yxyx;
		boolswizzle<4, bool4, 1, 0, 1, 1> yxyy;
		boolswizzle<4, bool4, 1, 0, 1, 2> yxyz;
		boolswizzle<4, bool4, 1, 0, 1, 3> yxyw;
		boolswizzle<4, bool4, 1, 0, 2, 0> yxzx;
		boolswizzle<4, bool4, 1, 0, 2, 1> yxzy;
		boolswizzle<4, bool4, 1, 0, 2, 2> yxzz;
		boolswizzle<4, bool4, 1, 0, 2, 3> yxzw;
		boolswizzle<4, bool4, 1, 0, 3, 0> yxwx;
		boolswizzle<4, bool4, 1, 0, 3, 1> yxwy;
		boolswizzle<4, bool4, 1, 0, 3, 2> yxwz;
		boolswizzle<4, bool4, 1, 0, 3, 3> yxww;
		boolswizzle<4, bool4, 1, 1, 0, 0> yyxx;
		boolswizzle<4, bool4, 1, 1, 0, 1> yyxy;
		boolswizzle<4, bool4, 1, 1, 0, 2> yyxz;
		boolswizzle<4, bool4, 1, 1, 0, 3> yyxw;
		boolswizzle<4, bool4, 1, 1, 1, 0> yyyx;
		boolswizzle<4, bool4, 1, 1, 1, 1> yyyy;
		boolswizzle<4, bool4, 1, 1, 1, 2> yyyz;
		boolswizzle<4, bool4, 1, 1, 1, 3> yyyw;
		boolswizzle<4, bool4, 1, 1, 2, 0> yyzx;
		boolswizzle<4, bool4, 1, 1, 2, 1> yyzy;
		boolswizzle<4, bool4, 1, 1, 2, 2> yyzz;
		boolswizzle<4, bool4, 1, 1, 2, 3> yyzw;
		boolswizzle<4, bool4, 1, 1, 3, 0> yywx;
		boolswizzle<4, bool4, 1, 1, 3, 1> yywy;
		boolswizzle<4, bool4, 1, 1, 3, 2> yywz;
		boolswizzle<4, bool4, 1, 1, 3, 3> yyww;
		boolswizzle<4, bool4, 1, 2, 0, 0> yzxx;
		boolswizzle<4, bool4, 1, 2, 0, 1> yzxy;
		boolswizzle<4, bool4, 1, 2, 0, 2> yzxz;
		boolswizzle<4, bool4, 1, 2, 0, 3> yzxw;
		boolswizzle<4, bool4, 1, 2, 1, 0> yzyx;
		boolswizzle<4, bool4, 1, 2, 1, 1> yzyy;
		boolswizzle<4, bool4, 1, 2, 1, 2> yzyz;
		boolswizzle<4, bool4, 1, 2, 1, 3> yzyw;
		boolswizzle<4, bool4, 1, 2, 2, 0> yzzx;
		boolswizzle<4, bool4, 1, 2, 2, 1> yzzy;
		boolswizzle<4, bool4, 1, 2, 2, 2> yzzz;
		boolswizzle<4, bool4, 1, 2, 2, 3> yzzw;
		boolswizzle<4, bool4, 1, 2, 3, 0> yzwx;
		boolswizzle<4, bool4, 1, 2, 3, 1> yzwy;
		boolswizzle<4, bool4, 1, 2, 3, 2> yzwz;
		boolswizzle<4, bool4, 1, 2, 3, 3> yzww;
		boolswizzle<4, bool4, 1, 3, 0, 0> ywxx;
		boolswizzle<4, bool4, 1, 3, 0, 1> ywxy;
		boolswizzle<4, bool4, 1, 3, 0, 2> ywxz;
		boolswizzle<4, bool4, 1, 3, 0, 3> ywxw;
		boolswizzle<4, bool4, 1, 3, 1, 0> ywyx;
		boolswizzle<4, bool4, 1, 3, 1, 1> ywyy;
		boolswizzle<4, bool4, 1, 3, 1, 2> ywyz;
		boolswizzle<4, bool4, 1, 3, 1, 3> ywyw;
		boolswizzle<4, bool4, 1, 3, 2, 0> ywzx;
		boolswizzle<4, bool4, 1, 3, 2, 1> ywzy;
		boolswizzle<4, bool4, 1, 3, 2, 2> ywzz;
		boolswizzle<4, bool4, 1, 3, 2, 3> ywzw;
		boolswizzle<4, bool4, 1, 3, 3, 0> ywwx;
		boolswizzle<4, bool4, 1, 3, 3, 1> ywwy;
		boolswizzle<4, bool4, 1, 3, 3, 2> ywwz;
		boolswizzle<4, bool4, 1, 3, 3, 3> ywww;

		boolswizzle<4, bool4, 2, 0, 0, 0> zxxx;
		boolswizzle<4, bool4, 2, 0, 0, 1> zxxy;
		boolswizzle<4, bool4, 2, 0, 0, 2> zxxz;
		boolswizzle<4, bool4, 2, 0, 0, 3> zxxw;
		boolswizzle<4, bool4, 2, 0, 1, 0> zxyx;
		boolswizzle<4, bool4, 2, 0, 1, 1> zxyy;
		boolswizzle<4, bool4, 2, 0, 1, 2> zxyz;
		boolswizzle<4, bool4, 2, 0, 1, 3> zxyw;
		boolswizzle<4, bool4, 2, 0, 2, 0> zxzx;
		boolswizzle<4, bool4, 2, 0, 2, 1> zxzy;
		boolswizzle<4, bool4, 2, 0, 2, 2> zxzz;
		boolswizzle<4, bool4, 2, 0, 2, 3> zxzw;
		boolswizzle<4, bool4, 2, 0, 3, 0> zxwx;
		boolswizzle<4, bool4, 2, 0, 3, 1> zxwy;
		boolswizzle<4, bool4, 2, 0, 3, 2> zxwz;
		boolswizzle<4, bool4, 2, 0, 3, 3> zxww;
		boolswizzle<4, bool4, 2, 1, 0, 0> zyxx;
		boolswizzle<4, bool4, 2, 1, 0, 1> zyxy;
		boolswizzle<4, bool4, 2, 1, 0, 2> zyxz;
		boolswizzle<4, bool4, 2, 1, 0, 3> zyxw;
		boolswizzle<4, bool4, 2, 1, 1, 0> zyyx;
		boolswizzle<4, bool4, 2, 1, 1, 1> zyyy;
		boolswizzle<4, bool4, 2, 1, 1, 2> zyyz;
		boolswizzle<4, bool4, 2, 1, 1, 3> zyyw;
		boolswizzle<4, bool4, 2, 1, 2, 0> zyzx;
		boolswizzle<4, bool4, 2, 1, 2, 1> zyzy;
		boolswizzle<4, bool4, 2, 1, 2, 2> zyzz;
		boolswizzle<4, bool4, 2, 1, 2, 3> zyzw;
		boolswizzle<4, bool4, 2, 1, 3, 0> zywx;
		boolswizzle<4, bool4, 2, 1, 3, 1> zywy;
		boolswizzle<4, bool4, 2, 1, 3, 2> zywz;
		boolswizzle<4, bool4, 2, 1, 3, 3> zyww;
		boolswizzle<4, bool4, 2, 2, 0, 0> zzxx;
		boolswizzle<4, bool4, 2, 2, 0, 1> zzxy;
		boolswizzle<4, bool4, 2, 2, 0, 2> zzxz;
		boolswizzle<4, bool4, 2, 2, 0, 3> zzxw;
		boolswizzle<4, bool4, 2, 2, 1, 0> zzyx;
		boolswizzle<4, bool4, 2, 2, 1, 1> zzyy;
		boolswizzle<4, bool4, 2, 2, 1, 2> zzyz;
		boolswizzle<4, bool4, 2, 2, 1, 3> zzyw;
		boolswizzle<4, bool4, 2, 2, 2, 0> zzzx;
		boolswizzle<4, bool4, 2, 2, 2, 1> zzzy;
		boolswizzle<4, bool4, 2, 2, 2, 2> zzzz;
		boolswizzle<4, bool4, 2, 2, 2, 3> zzzw;
		boolswizzle<4, bool4, 2, 2, 3, 0> zzwx;
		boolswizzle<4, bool4, 2, 2, 3, 1> zzwy;
		boolswizzle<4, bool4, 2, 2, 3, 2> zzwz;
		boolswizzle<4, bool4, 2, 2, 3, 3> zzww;
		boolswizzle<4, bool4, 2, 3, 0, 0> zwxx;
		boolswizzle<4, bool4, 2, 3, 0, 1> zwxy;
		boolswizzle<4, bool4, 2, 3, 0, 2> zwxz;
		boolswizzle<4, bool4, 2, 3, 0, 3> zwxw;
		boolswizzle<4, bool4, 2, 3, 1, 0> zwyx;
		boolswizzle<4, bool4, 2, 3, 1, 1> zwyy;
		boolswizzle<4, bool4, 2, 3, 1, 2> zwyz;
		boolswizzle<4, bool4, 2, 3, 1, 3> zwyw;
		boolswizzle<4, bool4, 2, 3, 2, 0> zwzx;
		boolswizzle<4, bool4, 2, 3, 2, 1> zwzy;
		boolswizzle<4, bool4, 2, 3, 2, 2> zwzz;
		boolswizzle<4, bool4, 2, 3, 2, 3> zwzw;
		boolswizzle<4, bool4, 2, 3, 3, 0> zwwx;
		boolswizzle<4, bool4, 2, 3, 3, 1> zwwy;
		boolswizzle<4, bool4, 2, 3, 3, 2> zwwz;
		boolswizzle<4, bool4, 2, 3, 3, 3> zwww;

		boolswizzle<4, bool4, 3, 0, 0, 0> wxxx;
		boolswizzle<4, bool4, 3, 0, 0, 1> wxxy;
		boolswizzle<4, bool4, 3, 0, 0, 2> wxxz;
		boolswizzle<4, bool4, 3, 0, 0, 3> wxxw;
		boolswizzle<4, bool4, 3, 0, 1, 0> wxyx;
		boolswizzle<4, bool4, 3, 0, 1, 1> wxyy;
		boolswizzle<4, bool4, 3, 0, 1, 2> wxyz;
		boolswizzle<4, bool4, 3, 0, 1, 3> wxyw;
		boolswizzle<4, bool4, 3, 0, 2, 0> wxzx;
		boolswizzle<4, bool4, 3, 0, 2, 1> wxzy;
		boolswizzle<4, bool4, 3, 0, 2, 2> wxzz;
		boolswizzle<4, bool4, 3, 0, 2, 3> wxzw;
		boolswizzle<4, bool4, 3, 0, 3, 0> wxwx;
		boolswizzle<4, bool4, 3, 0, 3, 1> wxwy;
		boolswizzle<4, bool4, 3, 0, 3, 2> wxwz;
		boolswizzle<4, bool4, 3, 0, 3, 3> wxww;
		boolswizzle<4, bool4, 3, 1, 0, 0> wyxx;
		boolswizzle<4, bool4, 3, 1, 0, 1> wyxy;
		boolswizzle<4, bool4, 3, 1, 0, 2> wyxz;
		boolswizzle<4, bool4, 3, 1, 0, 3> wyxw;
		boolswizzle<4, bool4, 3, 1, 1, 0> wyyx;
		boolswizzle<4, bool4, 3, 1, 1, 1> wyyy;
		boolswizzle<4, bool4, 3, 1, 1, 2> wyyz;
		boolswizzle<4, bool4, 3, 1, 1, 3> wyyw;
		boolswizzle<4, bool4, 3, 1, 2, 0> wyzx;
		boolswizzle<4, bool4, 3, 1, 2, 1> wyzy;
		boolswizzle<4, bool4, 3, 1, 2, 2> wyzz;
		boolswizzle<4, bool4, 3, 1, 2, 3> wyzw;
		boolswizzle<4, bool4, 3, 1, 3, 0> wywx;
		boolswizzle<4, bool4, 3, 1, 3, 1> wywy;
		boolswizzle<4, bool4, 3, 1, 3, 2> wywz;
		boolswizzle<4, bool4, 3, 1, 3, 3> wyww;
		boolswizzle<4, bool4, 3, 2, 0, 0> wzxx;
		boolswizzle<4, bool4, 3, 2, 0, 1> wzxy;
		boolswizzle<4, bool4, 3, 2, 0, 2> wzxz;
		boolswizzle<4, bool4, 3, 2, 0, 3> wzxw;
		boolswizzle<4, bool4, 3, 2, 1, 0> wzyx;
		boolswizzle<4, bool4, 3, 2, 1, 1> wzyy;
		boolswizzle<4, bool4, 3, 2, 1, 2> wzyz;
		boolswizzle<4, bool4, 3, 2, 1, 3> wzyw;
		boolswizzle<4, bool4, 3, 2, 2, 0> wzzx;
		boolswizzle<4, bool4, 3, 2, 2, 1> wzzy;
		boolswizzle<4, bool4, 3, 2, 2, 2> wzzz;
		boolswizzle<4, bool4, 3, 2, 2, 3> wzzw;
		boolswizzle<4, bool4, 3, 2, 3, 0> wzwx;
		boolswizzle<4, bool4, 3, 2, 3, 1> wzwy;
		boolswizzle<4, bool4, 3, 2, 3, 2> wzwz;
		boolswizzle<4, bool4, 3, 2, 3, 3> wzww;
		boolswizzle<4, bool4, 3, 3, 0, 0> wwxx;
		boolswizzle<4, bool4, 3, 3, 0, 1> wwxy;
		boolswizzle<4, bool4, 3, 3, 0, 2> wwxz;
		boolswizzle<4, bool4, 3, 3, 0, 3> wwxw;
		boolswizzle<4, bool4, 3, 3, 1, 0> wwyx;
		boolswizzle<4, bool4, 3, 3, 1, 1> wwyy;
		boolswizzle<4, bool4, 3, 3, 1, 2> wwyz;
		boolswizzle<4, bool4, 3, 3, 1, 3> wwyw;
		boolswizzle<4, bool4, 3, 3, 2, 0> wwzx;
		boolswizzle<4, bool4, 3, 3, 2, 1> wwzy;
		boolswizzle<4, bool4, 3, 3, 2, 2> wwzz;
		boolswizzle<4, bool4, 3, 3, 2, 3> wwzw;
		boolswizzle<4, bool4, 3, 3, 3, 0> wwwx;
		boolswizzle<4, bool4, 3, 3, 3, 1> wwwy;
		boolswizzle<4, bool4, 3, 3, 3, 2> wwwz;
		boolswizzle<4, bool4, 3, 3, 3, 3> wwww;


	};

	bool4():x(false),y(false),z(false),w(false){}

	bool4(bool b):x(b),y(b),z(b),w(b){}

	bool4(bool x, bool y, bool z, bool w):x(x),y(y),z(z),w(w){}

	bool4(bool2 xy, bool z, bool w):x(xy.y),y(xy.y),z(z),w(w){}

	bool4(bool x, bool2 yz, bool w):x(x),y(yz.x),z(yz.y),w(w){}

	bool4(bool x, bool y, bool2 zw):x(x),y(y),z(zw.x),w(zw.y){}

	bool4(bool2 xy, bool2 zw):x(xy.y),y(xy.y),z(zw.x),w(zw.y){}

	bool4(bool3 xyz, bool w):x(xyz.x),y(xyz.y),z(xyz.z),w(w){}

	bool4(bool x, bool3 yzw):x(x),y(yzw.x),z(yzw.y),w(yzw.z){}

	bool4& operator&=(const bool4& o)
	{
		x = x && o.x;
		y = y && o.y;
		z = z && o.z;
		w = w && o.w;
		return *this;
	}

	bool4& operator|=(const bool4& o)
	{
		x = x || o.x;
		y = y || o.y;
		z = z || o.z;
		w = w || o.w;
		return *this;
	}

	bool4 operator&	(const bool4& o) const
	{
		return bool4(x && o.x, y && o.y, z && o.z, w && o.w);
	}

	bool4 operator&&(const bool4& o) const
	{
		return bool4(x && o.x, y && o.y, z && o.z, w && o.w);
	}

	bool4 operator|	(const bool4& o) const
	{
		return bool4(x || o.x, y || o.y, z || o.z, w || o.w);
	}

	bool4 operator||(const bool4& o) const
	{
		return bool4(x || o.x, y || o.y, z || o.z, w || o.w);
	}

	bool4 operator==(const bool4& o) const
	{
		return bool4(x == o.x, y == o.y, z == o.z, w == o.w);
	}

	bool4 operator!=(const bool4& o) const
	{
		return bool4(x != o.x, y != o.y, z != o.z, w == o.w);
	}

	bool4 operator<=(const bool4& o) const
	{
		return bool4(x <= o.x, y <= o.y, z <= o.z, w <= o.w);
	}

	bool4 operator>=(const bool4& o) const
	{
		return bool4(x >= o.x, y >= o.y, z >= o.z, w >= o.w);
	}

	bool4 operator!() const
	{
		return bool4(!x, !y, !z, !w);
	}

	bool operator[](unsigned int i) const
	{
		if(i < 0 || i > 3)
			throw std::range_error("bool4 index out of range.");
		return v[i];
	}

	bool& operator[](unsigned int i)
	{
		if(i < 0 || i > 3)
			throw std::range_error("bool4 index out of range.");
		return v[i];
	}

	bool any() const
	{
		return x || y || z || w;
	}

	bool all() const
	{
		return x && y && z && w;
	}

	static bool4 random()
	{
		return bool4(rand()%2==0, rand()%2==0, rand()%2==0, rand()%2==0);
	}

	static const bool4 zero;
	static const bool4 xUnit;
	static const bool4 yUnit;
	static const bool4 zUnit;
	static const bool4 wUnit;
	static const bool4 one;

};

}}
