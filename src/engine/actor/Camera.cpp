#include "engine/actor/Camera.h"

#include "engine/Utility.h"
#include "pt/logging.h"
#include "pt/utility.hpp"

#include <assert.h>

using namespace engine;
using namespace math;

engine::Camera::
Camera( const std::string& name ):
    Actor( name )
{}


void Camera::
RotateCamera( float pitch, float yaw )
{
    auto lambda = [this, pitch, yaw]() -> void
    {
        pt::MutexLockGuard lock( mMutActorData );

        vec4 preferredUp    = GetPreferredUp_NoLock();
        vec4 right          = GetRight_NoLock();
        auto root           = this->GetRootComponent_NoLock();

        mat4 rotX = math::float4x4::rotation( right.XYZ(), pitch );
        mat4 rotZ = math::float4x4::rotation( preferredUp.XYZ(), yaw );
        mat4 Mrot = root->GetRotationMtx();
        Mrot = rotZ * rotX * Mrot;
        root->SetRotation( Mrot );
    };
    this->PostMessage( lambda );
}


void Camera::
LookAt( const float3& target )
{
    auto lambda = [this, target]() -> void
    {
        const float margin = PT_MATH_ERROR_MARGIN;

        //TODO: lock here, when cached data logic is available for Actor getters
        vec3 cam_pos = this->GetWorldPosition();

        // prevent looking at self
        if( vec3(target-cam_pos).length() < margin ){
            return;
        }

        // lock 'this'
        pt::MutexLockGuard lock( mMutActorData );
        auto root = this->GetRootComponent_NoLock();

        vec3 t_rel = (target - cam_pos).normalize();
        vec3 t_rel_XY = vec3( t_rel.x, t_rel.y, 0.0f ).normalize();

        const vec3 X = vec3::xUnit;
        float yawDeg   = RadToDeg( CalcAngle( X, t_rel_XY ) );
        float pitchDeg = RadToDeg( CalcAngle( t_rel_XY, t_rel ) );

        FRotator rotator = FRotator( pitchDeg, yawDeg, 0.0f );
        //PT_LOG_DEBUG( ToString( rotator ) );
        //PT_LOG_DEBUG( ToString( rotator.GetTransform() ) );

        root->SetRotation( rotator.GetTransform() );
    };
    this->PostMessage( lambda );
}


math::float4x4 Camera::
GetLookAtMtx() const
{
    pt::MutexLockGuard lock( mMutActorData );
    const vec4 dir     = GetForward_NoLock(); // TODO: this has to be based on world transform
    return CalcLookAtMtx( dir, GetPreferredUp_NoLock() );
}


math::float4x4 Camera::
GetViewMtx() const
{
    const float3 pos = this->GetRootComponent_NoLock()->GetPosition();
    float4x4  translation = float4x4::identity;

    translation.m[0][3] = -pos.x;
    translation.m[1][3] = -pos.y;
    translation.m[2][3] = -pos.z;

    return GetLookAtMtx() * translation;
}


math::float4 engine::Camera::
GetDir( Camera::Dir direction ) const
{
    switch ( direction ){
        case Dir::FORWARD:      return GetForward_NoLock();
        case Dir::BACKWARD:     return GetBackward_NoLock();
        case Dir::LEFT:         return GetLeft_NoLock();
        case Dir::RIGHT:        return GetRight_NoLock();
        case Dir::UP:           return GetUp_NoLock();
        case Dir::DOWN:         return GetDown_NoLock();
    }
    PT_LOG_ERR( "Invalid direction enum(" << direction << ") supplied to Camera::GetDir()" );
    return vec4::xUnit;
}


void Camera::
Move( const math::float3& dir )
{
    auto lambda = [this, dir]() -> void
    {
        pt::MutexLockGuard lock( mMutActorData );
        Move_NoLock( dir );
    };

    this->PostMessage( lambda );
}


float engine::Camera::
GetAspectRatio() const
{
    pt::MutexLockGuard lock( mMutActorData );
    return GetAspectRatio_NoLock();
}


void engine::Camera::
SetAspectRatio( float ratio )
{
    auto lambda = [this, ratio]() -> void
    {
        pt::MutexLockGuard lock( mMutActorData );
        SetAspectRatio_NoLock( ratio );
    };

    this->PostMessage( lambda );
}


float Camera::
GetFOVDeg() const
{
    pt::MutexLockGuard lock( mMutActorData );
    return math::RadToDeg( mFOV );
}


void Camera::
SetFOVDeg( float fov )
{
    SetFOVRad( math::DegToRad( fov ) );
}


float Camera::
GetFOVRad() const
{
    pt::MutexLockGuard lock( mMutActorData );
    return GetFOVRad_NoLock();
}


void Camera::
SetFOVRad( float fov )
{
    auto lambda = [this, fov]() -> void
    {
        pt::MutexLockGuard lock( mMutActorData );
        SetFOVRad_NoLock( fov );
    };

    this->PostMessage( lambda );
}


void Camera::
SetNearClippingDistance( float val )
{
    pt::MutexLockGuard lock( mMutActorData );
    SetNearClippingDistance_NoLock( val );
}


float Camera::
GetNearClippingDistance() const
{
    pt::MutexLockGuard lock( mMutActorData );
    return GetNearClippingDistance_NoLock();
}


void Camera::
SetFarClippingDistance( float val )
{
    pt::MutexLockGuard lock( mMutActorData );
    SetFarClippingDistance_NoLock( val );
}


float Camera::
GetFarClippingDistance() const
{
    pt::MutexLockGuard lock( mMutActorData );
    return GetFarClippingDistance_NoLock();
}


void Camera::
Move_NoLock( const math::float3& dir )
{
    auto rootComp = this->GetRootComponent_NoLock();
    rootComp->SetPosition( rootComp->GetPosition() + dir );
}


float Camera::
GetAspectRatio_NoLock() const
{
    return mAspectRatio;
}


void Camera::
SetAspectRatio_NoLock( float ratio )
{
    mAspectRatio = ratio;
}


float Camera::
GetFOVRad_NoLock() const
{
    return mFOV;
}


void Camera::
SetFOVRad_NoLock( float fov )
{
    mFOV = fov;
}


float Camera::
GetNearClippingDistance_NoLock() const
{
    return mClippingNearDist;
}


void Camera::
SetNearClippingDistance_NoLock( float distance )
{
    mClippingNearDist = distance;
}


float Camera::
GetFarClippingDistance_NoLock() const
{
    return mClippingFarDist;
}


void Camera::
SetFarClippingDistance_NoLock( float distance )
{
    mClippingFarDist = distance;
}


float4 Camera::
GetForward_NoLock() const
{
    auto mtx_rot = this->GetRootComponent_NoLock()->GetRotationMtx();
    return mtx_rot * vec4::xUnit;
}


float4 Camera::
GetBackward_NoLock() const
{
    return GetForward_NoLock() * -1;
}


float4 Camera::
GetRight_NoLock() const
{
    auto mtx_rot = this->GetRootComponent_NoLock()->GetRotationMtx();
    return mtx_rot * vec4::yUnit * -1;
}


float4 Camera::
GetLeft_NoLock() const
{
    return GetRight_NoLock() * -1;
}


float4 Camera::
GetUp_NoLock() const
{
    auto mtx_rot = this->GetRootComponent_NoLock()->GetRotationMtx();
    return mtx_rot * vec4::zUnit;
}


float4 Camera::
GetDown_NoLock() const
{
    return GetUp_NoLock() * -1;
}


float4 Camera::
GetPreferredUp_NoLock() const
{
    return mPreferredUp;
}


void Camera::
SetPreferredUp_NoLock( const math::float4& vector )
{
    mPreferredUp = vector.normalize();
}
