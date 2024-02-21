#include "engine/Camera.h"

#include "pt/utility.hpp"

#include <assert.h>

using namespace engine;

engine::Camera::
Camera( const std::string& name ):
    Actor( name )
{
    InitMembers();
}


const math::float3 engine::Camera::
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
    auto root = this->GetRootComponent_NoLock();
    auto pos = root->GetPosition();

    pos += dir;
    root->SetPosition( pos );
}


float engine::Camera::
GetAspectRatio() const
{
    return mAspectRatio;
}


void engine::Camera::
SetAspectRatio( float ratio )
{
    mAspectRatio = ratio;
}


float Camera::
GetZoom() const
{
    return mZoom;
}


void Camera::
SetZoom( float zoom )
{
    mZoom = zoom;
}


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


void Camera::
InitMembers()
{
    mAspectRatio = 1.0f,
    mZoom = 1.0f;
}
