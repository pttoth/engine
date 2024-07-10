#include "engine/actor/Camera.h"

#include "pt/utility.hpp"
#include "pt/logging.h"

#include <assert.h>

using namespace engine;
using namespace math;

engine::Camera::
Camera( const std::string& name ):
    Actor( name )
{}


math::float4 engine::Camera::
GetDir( Camera::Dir direction ) const
{
    assert( direction < 6 ); //TODO: log error instead
    switch ( direction ){
        case Dir::FORWARD:      return GetForward();
        case Dir::BACKWARD:     return GetBackward();
        case Dir::LEFT:         return GetLeft();
        case Dir::RIGHT:        return GetRight();
        case Dir::UP:           return GetUp();
        case Dir::DOWN:         return GetDown();
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
GetForward() const
{
    return GetForward_NoLock();
}


float4 Camera::
GetBackward() const
{
    return GetBackward_NoLock();
}


float4 Camera::
GetRight() const
{
    return GetRight_NoLock();
}


float4 Camera::
GetLeft() const
{
    return GetLeft_NoLock();
}


float4 Camera::
GetUp() const
{
    return GetUp_NoLock();
}


float4 Camera::
GetDown() const
{
    return GetDown_NoLock();
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


float3 Camera::
GetPreferredUp() const
{
    return mPreferredUp;
}


void Camera::
SetPreferredUp( const math::float3& vector )
{
    mPreferredUp = vector.normalize();
}


float4 Camera::
GetPreferredUp_NoLock() const
{
    return vec4::zUnit;
}
