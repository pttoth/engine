#include "Camera.h"

using namespace engine;

engine::Camera::
Camera(const std::string &name):
    Entity(name)
{
    InitMembers();
}


Camera::
Camera(const Camera &other):
    Entity(other)
{
    InitMembers();
}


const pt::math::float3 engine::Camera::
GetDir(Camera::Dir direction) const
{
    assert(direction < 6); //TODO: log error instead
    switch (direction){
        case Dir::FORWARD:      return GetForward();
        case Dir::BACKWARD:     return GetBackward();
        case Dir::LEFT:         return GetLeft();
        case Dir::RIGHT:        return GetRight();
        case Dir::UP:           return GetUp();
        case Dir::DOWN:         return GetDown();
    }
    return pt::math::float3::xUnit;
}


float engine::Camera::
GetAspectRatio() const
{
    return mAspectRatio;
}


void engine::Camera::
SetAspectRatio(float ratio)
{
    mAspectRatio = ratio;
}


float Camera::
GetZoom() const
{
    return mZoom;
}


void Camera::
SetZoom(float zoom)
{
    mZoom = zoom;
}


void Camera::
InitMembers()
{
    mAspectRatio = 1.0f,
    mZoom = 1.0f;
}

