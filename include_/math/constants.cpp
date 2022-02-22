/*
 * -------------------------------------------------------------------------
 * FILE:    constants.cpp
 * AUTHOR:  Dr. Szécsi László - szecsi@iit.bme.hu
 * -------------------------------------------------------------------------
 */
 

//#include "DXUT.h"
#include "math.h"

using namespace pttoth::math;

const bool1 bool1::zero(false);
const bool1 bool1::xUnit(true);
const bool1 bool1::one(true);

const bool2 bool2::zero(false, false);
const bool2 bool2::xUnit(true, false);
const bool2 bool2::yUnit(false, true);
const bool2 bool2::one(true, true);

const bool3 bool3::zero(false, false, false);
const bool3 bool3::xUnit(true, false, false);
const bool3 bool3::yUnit(false, true, false);
const bool3 bool3::zUnit(false, false, true);
const bool3 bool3::one(true, true, true);

const bool4 bool4::zero(false, false, false, false);
const bool4 bool4::xUnit(true, false, false, false);
const bool4 bool4::yUnit(false, true, false, false);
const bool4 bool4::zUnit(false, false, true, false);
const bool4 bool4::wUnit(false, false, false, true);
const bool4 bool4::one(true, true, true, true);


const int1 int1::zero(0);
const int1 int1::xUnit(1);
const int1 int1::one(1);

const int2 int2::zero(0, 0);
const int2 int2::xUnit(1, 0);
const int2 int2::yUnit(0, 1);
const int2 int2::one(1, 1);

const int3 int3::zero(0, 0, 0);
const int3 int3::xUnit(1, 0, 0);
const int3 int3::yUnit(0, 1, 0);
const int3 int3::zUnit(0, 0, 1);
const int3 int3::one(1, 1, 1);

const int4 int4::zero(0, 0, 0, 0);
const int4 int4::xUnit(1, 0, 0, 0);
const int4 int4::yUnit(0, 1, 0, 0);
const int4 int4::zUnit(0, 0, 1, 0);
const int4 int4::wUnit(0, 0, 0, 1);
const int4 int4::one(1, 1, 1, 1);


const float1 float1::zero(0.0f);
const float1 float1::xUnit(1.0f);
const float1 float1::one(1.0f);

const float2 float2::zero(0.0f, 0.0f);
const float2 float2::xUnit(1.0f, 0.0f);
const float2 float2::yUnit(0.0f, 1.0f);
const float2 float2::one(1.0f, 1.0f);

const float3 float3::zero(0.0f, 0.0f, 0.0f);
const float3 float3::xUnit(1.0f, 0.0f, 0.0f);
const float3 float3::yUnit(0.0f, 1.0f, 0.0f);
const float3 float3::zUnit(0.0f, 0.0f, 1.0f);
const float3 float3::one(1.0f, 1.0f, 1.0f);

const float3 float3::black				(0.0f, 0.0f, 0.0f);
const float3 float3::navy				(0.0f, 0.0f, 0.5f);
const float3 float3::blue				(0.0f, 0.0f, 1.0f);
const float3 float3::darkGreen			(0.0f, 0.5f, 0.0f);
const float3 float3::teal				(0.0f, 0.5f, 0.5f);
const float3 float3::azure				(0.0f, 0.5f, 1.0f);
const float3 float3::green				(0.0f, 1.0f, 0.0f);
const float3 float3::cyan				(0.0f, 1.0f, 1.0f);
const float3 float3::maroon				(0.5f, 0.0f, 0.0f);
const float3 float3::purple				(0.5f, 0.0f, 0.5f);
const float3 float3::slateBlue			(0.5f, 0.0f, 1.0f);
const float3 float3::olive				(0.5f, 0.5f, 0.0f);
const float3 float3::gray				(0.5f, 0.5f, 0.5f);
const float3 float3::cornflower			(0.5f, 0.5f, 1.0f);
const float3 float3::aquamarine			(0.5f, 1.0f, 0.75f);
const float3 float3::red				(1.0f, 0.0f, 0.0f);
const float3 float3::deepPink			(1.0f, 0.0f, 0.5f);
const float3 float3::magenta			(1.0f, 0.0f, 1.0f);
const float3 float3::orange				(1.0f, 0.5f, 0.0f);
const float3 float3::coral				(1.0f, 0.5f, 0.31f);
const float3 float3::mallow				(1.0f, 0.5f, 1.0f);
const float3 float3::yellow				(1.0f, 1.0f, 0.0f);
const float3 float3::gold				(1.0f, 1.0f, 0.5f);
const float3 float3::white				(1.0f, 1.0f, 1.0f);
const float3 float3::silver				(0.75f, 0.75f, 0.75f);


const float4 float4::zero(0.0f, 0.0f, 0.0f, 0.0f);
const float4 float4::xUnit(1.0f, 0.0f, 0.0f, 0.0f);
const float4 float4::yUnit(0.0f, 1.0f, 0.0f, 0.0f);
const float4 float4::zUnit(0.0f, 0.0f, 1.0f, 0.0f);
const float4 float4::wUnit(0.0f, 0.0f, 0.0f, 1.0f);
const float4 float4::one(1.0f, 1.0f, 1.0f, 1.0f);

const float4 float4::identity(0.0f, 0.0f, 0.0f, 1.0f);

const float4x4 float4x4::identity(
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f);
