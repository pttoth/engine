#include "engine/actor/CameraPerspective.h"

#include "engine/Utility.h"
#include "pt/alias.h"
#include "pt/utility.hpp"

#include <assert.h>

using namespace engine;
using namespace math;

const float gErrorMargin = 0.0001f;

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
RotateCamera( float pitch_angle, float yaw_angle )
{
    auto lambda = [this, pitch_angle, yaw_angle]() -> void
    {
        mat4  rotX = mat4::identity;
        mat4  rotZ = mat4::identity;
        math::float4 target;

        target  = math::float4( mLookatRelative, 1 );
        vec3 right = mLookatRelative.cross( mPreferredUp );

        rotX = math::float4x4::rotation( right, pitch_angle );
        rotZ = math::float4x4::rotation( mPreferredUp, yaw_angle );

        target = rotZ * rotX * target;
        mLookatRelative = Vecf3FromVecf4( target );
        UpdateData_NoLock();
    };
    this->PostMessage( lambda );
}


void CameraPerspective::
LookAt( const float3& lookat_pos )
{
    auto lambda = [this, lookat_pos]() -> void
    {
        pt::MutexLockGuard lock( mMutActorData );
        vec3 lookAt = lookat_pos - GetRootComponent_NoLock()->GetPosition();
        if( lookAt.length() < gErrorMargin ){
            return;
        }else{
            mLookatRelative = lookAt;
        }

        UpdateData_NoLock();
    };
    this->PostMessage( lambda );
}


math::float4x4 CameraPerspective::
GetLookAtMtx() const
{
    mat4 lookAt = mat4::identity;
    const vec3& right   = mCamRight;
    const vec3& up      = mCamUp;
    const vec3& dir     = mLookatRelative.normalize();

    // right-handed system
    //X: right  (screen horizontal)     (thumb)
    //Y: up     (screen vertical)       (pointing finger)
    //Z: -dir   (points towards viewer) (middle finger)
    lookAt.m[0][0] = right.v[0];    lookAt.m[0][1] = right.v[1];    lookAt.m[0][2] = right.v[2];
    lookAt.m[1][0] = up.v[0];       lookAt.m[1][1] = up.v[1];       lookAt.m[1][2] = up.v[2];
    lookAt.m[2][0] = -dir.v[0];     lookAt.m[2][1] = -dir.v[1];     lookAt.m[2][2] = -dir.v[2];
    lookAt.m[3][3] = 1.0f;

    return lookAt;
}


math::float4x4 CameraPerspective::
GetViewMtx() const
{
    const float3 pos = this->GetRootComponent_NoLock()->GetPosition();
    float4x4  translation = float4x4::identity;

    translation.m[0][3] = -pos.x;
    translation.m[1][3] = -pos.y;
    translation.m[2][3] = -pos.z;

    return GetLookAtMtx() * translation;
}


math::float4x4 CameraPerspective::
GetProjMtx() const
{
    math::float4x4 proj;

    float FoV   = GetFOVRad_NoLock();
    float aspect = GetAspectRatio_NoLock();
    float nearZ = mClippingNearDist;
    float farZ  = mClippingFarDist;

    proj.m[0][0] = 1/ (tanf(FoV / 2) * aspect);
    proj.m[1][1] = 1/  tanf(FoV / 2);

    proj.m[2][2] = -1*(nearZ+farZ) / (nearZ - farZ);
    proj.m[2][3] = -2* farZ*nearZ  / (nearZ - farZ);

    proj.m[3][2] = -1.0f;
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
    return mCamForward;
}


const float3 CameraPerspective::
GetBackward() const
{
    return GetForward() * -1;
}


const float3 CameraPerspective::
GetRight() const
{
    return mCamRight;
}


const float3 CameraPerspective::
GetLeft() const
{
    return GetRight() * -1;
}


const float3 CameraPerspective::
GetUp() const
{
    return mCamUp;
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
    vec3    forward = mLookatRelative.normalize();
    vec3    right   = forward.cross( mPreferredUp );
    float   len     = right.length();

    if( len < gErrorMargin ){
        PT_LOG_ERR( "Gimbal lock occured, resetting camera!" );
        // set camera in the X axis direction
        mCamRight    = vec3::yUnit * -1;
        mCamForward  = vec3::xUnit;
        mCamUp       = vec3::zUnit;
        mPreferredUp = vec3::zUnit;

        mLookatRelative = vec3::xUnit;
    }else{
        mCamForward  = forward;
        mCamRight    = right.normalize();
        mCamUp       = right.cross( forward ).normalize();
    }

    return;
}
