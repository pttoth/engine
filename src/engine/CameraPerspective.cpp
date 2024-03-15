#include "engine/CameraPerspective.h"

#include "engine/Utility.h"
#include "pt/alias.h"
#include "pt/utility.hpp"

#include <assert.h>

using namespace engine;
using namespace math;


CameraPerspective::
CameraPerspective( const std::string& name ):
    Camera( name )
{
    Construct_NoLock();
}


CameraPerspective::
~CameraPerspective()
{}


void CameraPerspective::
RotateXZ( float x_angle, float z_angle )
{
    using mat4 = math::float4x4;
    using vec3 = math::float3;
    const vec3 up = vec3(0.0f, 0.0f, 1.0f);

    mat4  rotX = mat4::identity;
    mat4  rotZ = mat4::identity;
    math::float4 target;

    target  = math::float4( mLookatRelative, 1 );
    vec3 right = mLookatRelative.normalize().cross( up );

    rotX = math::float4x4::rotation( right, x_angle );
    rotZ = math::float4x4::rotation( up, z_angle );

    target = rotX * rotZ * target;
    mLookatRelative = Vecf3FromVecf4( target );
    UpdateData_NoLock();
}


math::float4x4 CameraPerspective::
GetRotationMtx() const
{
    return CalcRotMtx( mLookat - GetPosition(), mPreferredUp );
}


math::float4x4 CameraPerspective::
GetViewMtx() const
{
    //return this->GetRootComponent_NoLock()->GetTransform().invert();

    const float3 pos = this->GetRootComponent_NoLock()->GetPosition();
    float4x4  translation = float4x4::identity;

    translation.m[0][3] -= pos.x;
    translation.m[1][3] -= pos.y;
    translation.m[2][3] -= pos.z;

    return GetRotationMtx() * translation;
}


math::float4x4 CameraPerspective::
GetProjMtx() const
{
    math::float4x4 proj = math::float4x4::identity;

    float NearZ = mClippingNearDist;
    float FarZ  = mClippingFarDist;

    proj.m[0][0] = -1/  (tanf(mFOV / 2) * mAspectRatio);
    proj.m[1][1] = -1/   tanf(mFOV / 2);

    proj.m[2][2] = (-1*NearZ-FarZ) / (NearZ - FarZ);
    proj.m[3][2] = (2*FarZ*NearZ)  / (NearZ - FarZ);
    proj.m[2][3] = -1.0f;   //OpenGL
    //proj.m[2][3] = 1.0f;  //DirectX

    proj.m[3][3] = 0;

    return proj;
}


void CameraPerspective::
Move( const math::float3& dir )
{
    auto lambda = [this, dir]() -> void
    {
        pt::MutexLockGuard lock( mMutActorData );
        Move_NoLock( dir );
        UpdateData_NoLock();
    };
    this->PostMessage( lambda );
}


void CameraPerspective::
SetLookat( const math::float3& pos )
{
    auto lambda = [this, pos]() -> void
    {
        pt::MutexLockGuard lock( mMutActorData );
        mLookat = pos;
        mLookatRelative = mLookatRelative - this->GetPosition();
    };

    this->PostMessage( lambda );
}


void CameraPerspective::
SetNearClippingDistance( float val )
{
    auto lambda = [this, val]() -> void
    {
        pt::MutexLockGuard lock( mMutActorData );
        mClippingNearDist = val;
    };

    this->PostMessage( lambda );
}


void CameraPerspective::
SetFarClippingDistance( float val )
{
    auto lambda = [this, val]() -> void
    {
        pt::MutexLockGuard lock( mMutActorData );
        mClippingFarDist = val;
    };

    this->PostMessage( lambda );
}


const float3 CameraPerspective::
GetForward() const
{
    return mLookatRelative.normalize();
}


const float3 CameraPerspective::
GetBackward() const
{
    return GetForward() * -1;
}


const float3 CameraPerspective::
GetRight() const
{
    return mCamRight.normalize();
}


const float3 CameraPerspective::
GetLeft() const
{
    return GetRight() * -1;
}


const float3 CameraPerspective::
GetUp() const
{
    return mCamUp.normalize();
}


const float3 CameraPerspective::
GetDown() const
{
    return GetUp() * -1;
}


bool CameraPerspective::
OnCreateRenderContext()
{
    return true;
}


void CameraPerspective::
OnDestroyRenderContext()
{}


void CameraPerspective::
OnTick( float t, float dt )
{}


void CameraPerspective::
OnSpawned()
{}


void CameraPerspective::
OnDespawned()
{}


void CameraPerspective::
Construct_NoLock()
{}


void CameraPerspective::
UpdateData_NoLock()
{
    vec3 pos = GetRootComponent_NoLock()->GetPosition();
    mLookat = pos + mLookatRelative;

    mCamZ        = (pos - mLookat); //note: invert this for DirectX
    assert( 0.0f < mCamZ.length() );
    mCamZ        = mCamZ.normalize();

    mCamRight    = mPreferredUp.cross( mCamZ );
    assert( 0.0f < mCamRight.length() );
    mCamRight    = mCamRight.normalize();
    mCamUp       = mCamZ.cross( mCamRight );  //can skip normalization here

    return;
}
