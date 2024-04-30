#include "engine/actor/CameraPerspective.h"

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
RotateCamera( float pitch_angle, float yaw_angle )
{
    auto lambda = [this, pitch_angle, yaw_angle]() -> void
    {
        using mat4 = math::float4x4;
        using vec3 = math::float3;

        mat4  rotX = mat4::identity;
        mat4  rotZ = mat4::identity;
        math::float4 target;

        target  = math::float4( mLookatRelative, 1 );
        vec3 right = mLookatRelative.normalize().cross( mPreferredUp );

        rotX = math::float4x4::rotation( right, pitch_angle );
        rotZ = math::float4x4::rotation( mPreferredUp, yaw_angle );

        target = rotX * rotZ * target;
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
        mLookatRelative = lookat_pos - GetRootComponent_NoLock()->GetPosition();
        if( mLookatRelative.length() < 0.0001f ){
            mLookatRelative = vec3::xUnit;
            PT_LOG_LIMITED_ERR( 100, "Triggered failsafe reset for camera '" << GetName() << "' as Lookat-Position distance was too short" );
        }

        UpdateData_NoLock();
    };
    this->PostMessage( lambda );
}


math::float4x4 CameraPerspective::
GetRotationMtx() const
{
    const math::float3& direction = mLookatRelative;
    const math::float3& preferredUp = mPreferredUp;

    if( 0 == direction.length() ){
        PT_LOG_ERR( "Camera '" << GetName() << "': null-vector supplied as 'direction!'" );
        assert( 0 != direction.length() );
        return math::float4x4::identity;
    }
    math::float3 dir = direction.normalize();
    math::float3 right = dir.cross( preferredUp.normalize() );

    bool gimbal_lock = !(0.0f < right.length());
    if( gimbal_lock ){
        PT_LOG_ERR( "CalcRotMtx: Gimbal lock occured!" );
        PT_LOG_ERR( "dir: \n" << ToString(dir) );
        PT_LOG_ERR( "preferredUp: \n" << ToString(preferredUp) );
        assert( !gimbal_lock );
        return math::float4x4::identity;
    }
    math::float3    up      = right.cross( dir );
    math::float4x4  orient  = math::float4x4::identity;

    //(World)X stays    X(Screen)
    //(World)Y becomes -Z(Screen)
    //(World)Z becomes  Y(Screen)
    orient.m[0][0] = right.v[0];    orient.m[0][1] = right.v[1];    orient.m[0][2] = right.v[2];
    orient.m[1][0] = up.v[0];       orient.m[1][1] = up.v[1];       orient.m[1][2] = up.v[2];
    orient.m[2][0] = -dir.v[0];     orient.m[2][1] = -dir.v[1];     orient.m[2][2] = -dir.v[2];

    return orient;
}


math::float4x4 CameraPerspective::
GetViewMtx() const
{
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

    proj.m[2][3] = -1.0f;
    proj.m[3][3] = 0;

    return proj;
}


float CameraPerspective::
GetPitch() const
{
    if( (0.0f == mLookatRelative.x) && (0.0f == mLookatRelative.y) ){
        if( 0.0f < mLookatRelative.z ){
            return M_PI/2;
        }else if( 0.0f > mLookatRelative.z ){
            return M_PI/2 * -1;
        }
        return 0.0f;
    }
    float3 proj = mLookatRelative;
    proj.z = 0.0f;
    return acosf( proj.normalize().dot( mLookatRelative.normalize() ) );
}


float CameraPerspective::
GetRoll() const
{
    PT_WARN_UNIMPLEMENTED_FUNCTION
    return 0.0f;
}


float CameraPerspective::
GetYaw() const
{
    if( (0.0f == mLookatRelative.x) && (0.0f == mLookatRelative.y) ){
        return 0.0f;
    }
    float3 proj = mLookatRelative;
    proj.z = 0.0f;
    return acosf( proj.normalize().dot( mLookatRelative.normalize() ) );
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


void CameraPerspective::
SetFOV( const float fov )
{
    auto lambda = [this, fov]() -> void
    {
        pt::MutexLockGuard lock( mMutActorData );
        mFOV = fov;
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
    mCamZ        = (mLookatRelative * -1).normalize();
    mCamRight    = mPreferredUp.cross( mCamZ );
    assert( 0.0001f <= mCamRight.length() );
    mCamRight    = mCamRight.normalize();
    mCamUp       = mCamZ.cross( mCamRight );

    return;
}
