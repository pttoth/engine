#include "engine/Camera2D.h"

using namespace engine;
using namespace math;


engine::Camera2D::
Camera2D(const std::string& name):
    Camera(name)
{
    Construct();
}


void engine::Camera2D::
UpdateData()
{}


const math::float4x4 engine::Camera2D::
GetViewMtx() const
{
    auto root = this->GetRootComponent();

    const math::float3 pos = root->GetPosition();
    const math::float4 ori = root->GetOrientation();

    //always looks top-down
    math::float4x4  orient = math::float4x4::identity;
    //orient._00 = ori.x;
    //orient._11 = ori.y;
    //orient._22 = ori.z * (-1); //OpenGL
    //orient._22 = ori.z; //DirectX

    orient._22 = 1;

    //(x,y), z is always 0
    math::float4x4  translation = math::float4x4::identity;
    translation.m[0][3] -= pos.x;
    translation.m[1][3] -= pos.y;
    translation.m[2][3] -= 0;


    return translation * orient;
}


const math::float4x4 engine::Camera2D::
GetProjMtx() const
{
    //-----------------------------------
    //OpenGLTutorials' 3d camera proj mtx ('pre-multiplication' transposition)
    //-----------------------------------
    /*
    float4x4 proj = float4x4::identity;
    float NearZ = m_clipping_near_dist;
    float FarZ  = m_clipping_far_dist;

    proj.m[0][0] = -1/  (tanf(m_FOV / 2) * m_aspect_ratio);
    proj.m[1][1] = -1/   tanf(m_FOV / 2);

    proj.m[2][2] = (-1*NearZ-FarZ) / (NearZ - FarZ);
    proj.m[3][2] = (2*FarZ*NearZ)  / (NearZ - FarZ);
    proj.m[2][3] = -1.0f;   //OpenGL
    //proj.m[2][3] = 1.0f;  //DirectX

    proj.m[3][3] = 0;
    */
    //-----------------------------------
    float4x4 proj = float4x4::identity;

    float NearZ = 0.1f;
    float FarZ  = 15.0f;

    float m_FOV = 90.0f;
    float m_aspect_ratio = 16.0f/9.0f;

    proj.m[0][0] = 1/  (tanf(m_FOV / 2) * m_aspect_ratio);
    proj.m[1][1] = 1/   tanf(m_FOV / 2);

    //proj.m[2][2] = (1*NearZ-FarZ) / (NearZ - FarZ);
    //proj.m[3][2] = (2*FarZ*NearZ)  / (NearZ - FarZ);    //note: post-multiplication transposition
    proj.m[2][3] = -1.0f;   //OpenGL
    //proj.m[2][3] = 1.0f;  //DirectX


    //proj[][]

    //TODO: implement zoom functionality ('I' divided by zoom?)

    return proj;
}


void engine::Camera2D::
Move(const math::float3& dir)
{
    auto root = this->GetRootComponent();
    auto pos = root->GetPosition();

    pos.z = 0;
    pos += dir;
    pos.z = 0;
    root->SetPosition( pos );

    UpdateData();
}


void Camera2D::
OnTick(float t, float dt)
{}



const math::float3 engine::Camera2D::
GetForward() const
{
    return math::float3{0, 0, -1};
}


const math::float3 engine::Camera2D::
GetBackward() const
{
    return math::float3{0, 0, 1};
}


const math::float3 engine::Camera2D::
GetRight() const
{
    return math::float3{1, 0, 0};
}


const math::float3 engine::Camera2D::
GetLeft() const
{
    return math::float3{-1, 0, 0};
}


const math::float3 engine::Camera2D::
GetUp() const
{
    return math::float3{0, 1, 0};
}


const math::float3 engine::Camera2D::
GetDown() const
{
    return math::float3{0, -1, 0};
}


void Camera2D::
Construct()
{
    SetAspectRatio( 16.0f/9.0f ); //TODO: put this in config
    SetZoom( 1.0f );

    UpdateData();
}


void Camera2D::
OnSpawned()
{}


void Camera2D::
OnDespawned()
{}
