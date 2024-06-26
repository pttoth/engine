#include "engine/actor/Camera.h"

#include "pt/utility.hpp"
#include <assert.h>

using namespace engine;
using namespace math;

engine::Camera::
Camera( const std::string& name ):
    Actor( name )
{}


math::float3 engine::Camera::
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
    return math::float3::xUnit;
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


//TODO: fix these!
//  this is camera coordinate system, use worldcomponent direction here (world coordindates)
const math::float3 engine::Camera::
GetForward() const
{
    return math::float3{0, 0, -1};
}


const math::float3 engine::Camera::
GetBackward() const
{
    return math::float3{0, 0, 1};
}


const math::float3 engine::Camera::
GetRight() const
{
    return math::float3{1, 0, 0};
}


const math::float3 engine::Camera::
GetLeft() const
{
    return math::float3{-1, 0, 0};
}


const math::float3 engine::Camera::
GetUp() const
{
    return math::float3{0, 1, 0};
}


const math::float3 engine::Camera::
GetDown() const
{
    return math::float3{0, -1, 0};
}
