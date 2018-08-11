/*
 * -------------------------------------------------------------------------
 * FILE:    float4x4.h
 * AUTHOR:  Dr. Szécsi László - szecsi@iit.bme.hu
 * -------------------------------------------------------------------------
 */
 #pragma once

#include <math.h>

namespace pttoth { namespace math {

class float4x4
{
public:
	union
	{
		struct
		{
			float        _00, _01, _02, _03;
			float        _10, _11, _12, _13;
			float        _20, _21, _22, _23;
			float        _30, _31, _32, _33;
		};
        float m[4][4];
		float l[16];
    };

	float4x4():
		_00(1.0f), _01(0.0f), _02(0.0f), _03(0.0f),
		_10(0.0f), _11(1.0f), _12(0.0f), _13(0.0f),
		_20(0.0f), _21(0.0f), _22(1.0f), _23(0.0f),
		_30(0.0f), _31(0.0f), _32(0.0f), _33(1.0f)
	{
	}

	float4x4(
		float _00, float _01, float _02, float _03,
		float _10, float _11, float _12, float _13,
		float _20, float _21, float _22, float _23,
		float _30, float _31, float _32, float _33):
		_00(_00), _01(_01), _02(_02), _03(_03),
		_10(_10), _11(_11), _12(_12), _13(_13),
		_20(_20), _21(_21), _22(_22), _23(_23),
		_30(_30), _31(_31), _32(_32), _33(_33)
	{
	}

	static const float4x4 identity;

	float4x4 elementwiseProduct(const float4x4& o) const
	{
		float4x4 r;
		for(int i=0;i<16;i++)
			r.l[i] = l[i] * o.l[i];
		return r;
	}

	float4x4 operator+(const float4x4& o) const
	{
		float4x4 r;
		for(int i=0;i<16;i++)
			r.l[i] = l[i] + o.l[i];
		return r;
	}

	float4x4 operator-(const float4x4& o) const
	{
		float4x4 r;
		for(int i=0;i<16;i++)
			r.l[i] = l[i] - o.l[i];
		return r;
	}

	float4x4& assignElementwiseProduct(const float4x4& o)
	{
		for(int i=0;i<16;i++)
			l[i] *= o.l[i];
		return *this;
	}

	float4x4& operator*=(float s )
	{
		for(int i=0;i<16;i++)
			l[i] *= s;
		return *this;
	}

	float4x4& operator/=(float s )
	{
		float is = 1 / s;
		for(int i=0;i<16;i++)
			l[i] *= is;
		return *this;
	}

	float4x4& operator+=(const float4x4& o)
	{
		for(int i=0;i<16;i++)
			l[i] += o.l[i];
		return *this;
	}

	float4x4& operator-=(const float4x4& o)
	{
		for(int i=0;i<16;i++)
			l[i] -= o.l[i];
		return *this;
	}

	float4x4 mul(const float4x4& o) const
	{
		float4x4 product;

		for (int r=0;r<4;r++)
			for (int c=0;c<4;c++)
		        product.m[r][c] = 
		            m[r][0] * o.m[0][c] +
		            m[r][1] * o.m[1][c] +
		            m[r][2] * o.m[2][c] +
		            m[r][3] * o.m[3][c];

		return product;
	}

	float4x4 operator<<(const float4x4& o) const
	{
		return mul(o);
	}

	float4x4& operator <<=(const float4x4& o)
	{
		*this = *this << o;
		return *this;
	}

	float4x4 operator*(const float4x4& o) const
	{
		return mul(o);
	}

	float4x4& operator*=(const float4x4& o)
	{
		*this = *this * o;
		return *this;
	}

	float4 mul(const float4& v) const
	{
		return float4( v.dot( *(float4*)m[0] ), v.dot( *(float4*)m[1] ), v.dot( *(float4*)m[2] ), v.dot( *(float4*)m[3] ) );
	}

	float4 transform(const float4& v) const
	{
		return float4( 
			_00 * v.x + _10 * v.y + _20 * v.z + _30 * v.w,
			_01 * v.x + _11 * v.y + _21 * v.z + _31 * v.w,
			_02 * v.x + _12 * v.y + _22 * v.z + _32 * v.w,
			_03 * v.x + _13 * v.y + _23 * v.z + _33 * v.w
			);
	}

	float4 operator*(const float4& v) const
	{
		return mul(v);
	}

	float4x4 operator*(float s) const
	{
		float4x4 r;
		for(int i=0;i<16;i++)
			r.l[i] = l[i]*s;
		return r;
	}

	static float4x4 scaling(const float3& factors)
	{
	    float4x4 s = identity;
		s._00 = factors.x;
		s._11 = factors.y;
		s._22 = factors.z;

		return s;
	}

	static float4x4 translation(const float3& offset)
	{
	    float4x4 t = identity;
		t._30 = offset.x;
		t._31 = offset.y;
		t._32 = offset.z;
		return t;
	}

	static float4x4 rotation(const float3& axis, float angle)
	{
	    float4x4 r = identity;
		
	    float s = sin(angle);
	    float c = cos(angle);
	    float t = 1 - c;
		
		float axisLength = axis.length();
		if(axisLength == 0.0f)
			return identity;
		float3 ax = axis / axisLength;
	
	    float& x = ax[0];
	    float& y = ax[1];
	    float& z = ax[2];
	
	    r._00 = t*x*x+c;
	    r._01 = t*y*x+s*z;
	    r._02 = t*z*x-s*y;
	
	    r._10 = t*x*y-s*z;
	    r._11 = t*y*y+c;
	    r._12 = t*z*y+s*x;
	
	    r._20 = t*x*z+s*y;
	    r._21 = t*y*z-s*x;
	    r._22 = t*z*z+c;
	
	    return r;
	}

	static float4x4 reflection(const float4& plane)
	{
		float4 normalPlane = plane / plane.xyz.length();

		return float4x4(
			-2 * normalPlane.x * normalPlane.x + 1 , -2 * normalPlane.y * normalPlane.x     , -2 * normalPlane.z * normalPlane.x       , 0 ,
			-2 * normalPlane.x * normalPlane.y     , -2 * normalPlane.y * normalPlane.y + 1 , -2 * normalPlane.z * normalPlane.y       , 0 ,
			-2 * normalPlane.x * normalPlane.z     , -2 * normalPlane.y * normalPlane.z     , -2 * normalPlane.z * normalPlane.z + 1   , 0 ,
			-2 * normalPlane.x * normalPlane.w     , -2 * normalPlane.y * normalPlane.w     , -2 * normalPlane.z * normalPlane.w       , 1 );
	}


	static float4x4 view(const float3& eye, const float3& ahead, const float3& up)
	{
		float3 zaxis = ahead.normalize();
		float3 xaxis = up.cross(zaxis).normalize();
		float3 yaxis = zaxis.cross(xaxis);
    
		return float4x4(
		 xaxis.x         ,  yaxis.x         ,  zaxis.x          , 0,
		 xaxis.y         ,  yaxis.y         ,  zaxis.y          , 0,
		 xaxis.z         ,  yaxis.z         ,  zaxis.z          , 0,
		-xaxis.dot(eye)  , -yaxis.dot(eye)  , -zaxis.dot(eye)   , 1);
	}

	static float4x4 proj(float fovy, float aspect, float zn, float zf)
	{
		float yScale = 1.0f / ::tanf(fovy * 0.5f);
		float xScale = yScale / aspect;
		return float4x4(
			xScale ,    0.0f    ,      0.0f            ,   0.0f,
			0.0f      ,  yScale ,      0.0f            ,   0.0f,
			0.0f      ,    0.0f    ,   zf/(zf-zn)      ,   1,
			0.0f      ,    0.0f    ,   -zn*zf/(zf-zn)  ,   0);
	}

	float4x4 transpose() const
	{
		return float4x4(
			_00, _10, _20, _30,
			_01, _11, _21, _31,
			_02, _12, _22, _32,
			_03, _13, _23, _33);
	}

	float4x4 _invert() const
	{
		float det;
		float d10, d20, d21, d31, d32, d03;
		float4x4 inv;
		
		/* Inverse = adjoint / det. (See linear algebra texts.)*/
		
		/* pre-compute 2x2 dets for last two rows when computing */
		/* cofactors of first two rows. */
		d10 = (_02*_13-_03*_12);
		d20 = (_02*_23-_03*_22);
		d21 = (_12*_23-_13*_22);
		d31 = (_12*_33-_13*_32);
		d32 = (_22*_33-_23*_32);
		d03 = (_32*_03-_33*_02);
		
		inv.l[0] =  (_11 * d32 - _21 * d31 + _31 * d21);
		inv.l[1] = -(_01 * d32 + _21 * d03 + _31 * d20);
		inv.l[2] =  (_01 * d31 + _11 * d03 + _31 * d10);
		inv.l[3] = -(_01 * d21 - _11 * d20 + _21 * d10);
		
		/* Compute determinant as early as possible using these cofactors. */
		det = _00 * inv.l[0] + _10 * inv.l[1] + _20 * inv.l[2] + _30 * inv.l[3];
		
		/* Run singularity test. */
		if (det == 0.0) {
			return identity;
		}
		else
		{
		   float invDet = 1.0 / det;
		   /* Compute rest of inverse. */
		   inv.l[0] *= invDet;
		   inv.l[1] *= invDet;
		   inv.l[2] *= invDet;
		   inv.l[3] *= invDet;
		
		   inv.l[4] = -(_10 * d32 - _20 * d31 + _30 * d21) * invDet;
		   inv.l[5] =  (_00 * d32 + _20 * d03 + _30 * d20) * invDet;
		   inv.l[6] = -(_00 * d31 + _10 * d03 + _30 * d10) * invDet;
		   inv.l[7] =  (_00 * d21 - _10 * d20 + _20 * d10) * invDet;
		
		   /* Pre-compute 2x2 dets for first two rows when computing */
		   /* cofactors of last two rows. */
		   d10 = _00*_11-_01*_10;
		   d20 = _00*_21-_01*_20;
		   d21 = _10*_21-_11*_20;
		   d31 = _10*_31-_11*_30;
		   d32 = _20*_31-_21*_30;
		   d03 = _30*_01-_31*_00;
		
		   inv.l[8] =  (_13 * d32 - _23 * d31 + _33 * d21) * invDet;
		   inv.l[9] = -(_03 * d32 + _23 * d03 + _33 * d20) * invDet;
		   inv.l[10] =  (_03 * d31 + _13 * d03 + _33 * d10) * invDet;
		   inv.l[11] = -(_03 * d21 - _13 * d20 + _23 * d10) * invDet;
		   inv.l[12] = -(_12 * d32 - _22 * d31 + _32 * d21) * invDet;
		   inv.l[13] =  (_02 * d32 + _22 * d03 + _32 * d20) * invDet;
		   inv.l[14] = -(_02 * d31 + _12 * d03 + _32 * d10) * invDet;
		   inv.l[15] =  (_02 * d21 - _12 * d20 + _22 * d10) * invDet;
		
			return inv;
		}
	}

	float4x4 invert() const
	{
		register float det;

		if( _03 != 0.0f || _13 != 0.0f || _23 != 0.0f || _33 != 1.0f )
		{
		   return _invert();
		}

		float4x4 inv;

		/* Inverse = adjoint / det. */
		inv.l[0] = _11 * _22 - _21 * _12;
		inv.l[1] = _21 * _02 - _01 * _22;
		inv.l[2] = _01 * _12 - _11 * _02;

		/* Compute determinant as early as possible using these cofactors. */
		det = _00 * inv.l[0] + _10 * inv.l[1] + _20 * inv.l[2];

		/* Run singularity test. */
		if (det == 0.0f)
		{
		   /* printf("invert_float4x4: Warning: Singular float4x4.\n"); */
		   return identity;
		}
		else
		{
		   float d10, d20, d21, d31, d32, d03;
		   register float im00, im10, im20, im30;

		   det = 1.0f / det;

		   /* Compute rest of inverse. */
		   inv.l[0] *= det;
		   inv.l[1] *= det;
		   inv.l[2] *= det;
		   inv.l[3]  = 0.0f;

		   im00 = _00 * det;
		   im10 = _10 * det;
		   im20 = _20 * det;
		   im30 = _30 * det;
		   inv.l[4] = im20 * _12 - im10 * _22;
		   inv.l[5] = im00 * _22 - im20 * _02;
		   inv.l[6] = im10 * _02 - im00 * _12;
		   inv.l[7] = 0.0f;

		   /* Pre-compute 2x2 dets for first two rows when computing */
		   /* cofactors of last two rows. */
		   d10 = im00 * _11 - _01 * im10;
		   d20 = im00 * _21 - _01 * im20;
		   d21 = im10 * _21 - _11 * im20;
		   d31 = im10 * _31 - _11 * im30;
		   d32 = im20 * _31 - _21 * im30;
		   d03 = im30 * _01 - _31 * im00;

		   inv.l[8] =  d21;
		   inv.l[9] = -d20;
		   inv.l[10] = d10;
		   inv.l[11] = 0.0f;

		   inv.l[12] = -(_12 * d32 - _22 * d31 + _32 * d21);
		   inv.l[13] =  (_02 * d32 + _22 * d03 + _32 * d20);
		   inv.l[14] = -(_02 * d31 + _12 * d03 + _32 * d10);
		   inv.l[15] =  1.0f;

		   return inv;
		}
	}

};

inline float4 operator*(const float4& v, const float4x4& m)
{
	return m.transform(v);
}

inline const float4& operator*=(float4& v, const float4x4& m)
{
	v = m.transform(v);
	return v;
}

}}
