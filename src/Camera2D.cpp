#include "Camera2D.h"

using namespace engine;

engine::Camera2D::
Camera2D(): Camera()
{
    SetAspectRatio(16.0f/9.0f); //TODO: put this in config
    SetZoom(1.0f);

    UpdateData();
}


void Camera2D::
OnRegister()
{}


void Camera2D::
OnUnregister()
{}


void engine::Camera2D::
UpdateData()
{

}


const pt::math::float4x4 engine::Camera2D::
GetViewMtx() const
{
    const pt::math::float3 pos = this->getRootComponent()->getPosition();

    //(x,y), z is always 0
    pt::math::float4x4  translation = pt::math::float4x4::identity;
    translation.m[3][0] -= pos.v[0];
    translation.m[3][1] -= pos.v[1];
    translation.m[3][2] -= 0;

    //always looks top-down
    pt::math::float4x4  orient = pt::math::float4x4::identity;
    orient.m[0][0] = 1;
    orient.m[1][1] = 1;
    orient.m[2][2] = -1; //OpenGL
    //orient.m[2][2] = 1; //DirectX

    return translation * orient;
}


const pt::math::float4x4 engine::Camera2D::
GetProjMtx() const
{
    pt::math::float4x4 proj = pt::math::float4x4::identity;

    //TODO: implement zoom functionality ('I' divided by zoom?)

    return proj;
}


void engine::Camera2D::
Move(const pt::math::float3& dir)
{
    auto root = this->getRootComponent();
    auto pos = root->getPosition();

    pos.z = 0;
    pos += dir;
    pos.z = 0;
    root->setPosition( pos );

    UpdateData();
}


void Camera2D::
tick(float t, float dt)
{}


const pt::math::float3 engine::Camera2D::
GetForward() const
{
    return pt::math::float3{0, 0, -1};
}


const pt::math::float3 engine::Camera2D::
GetBackward() const
{
    return pt::math::float3{0, 0, 1};
}


const pt::math::float3 engine::Camera2D::
GetRight() const
{
    return pt::math::float3{1, 0, 0};
}


const pt::math::float3 engine::Camera2D::
GetLeft() const
{
    return pt::math::float3{-1, 0, 0};
}


const pt::math::float3 engine::Camera2D::
GetUp() const
{
    return pt::math::float3{0, 1, 0};
}


const pt::math::float3 engine::Camera2D::
GetDown() const
{
    return pt::math::float3{0, -1, 0};
}

