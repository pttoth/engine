#include "engine/actor/CameraOrtographic.h"

#include "pt/utility.hpp"

using namespace engine;
using namespace math;


engine::CameraOrtographic::
CameraOrtographic( const std::string& name ):
    Camera( name )
{
    Construct();
}


bool CameraOrtographic::
operator==( const CameraOrtographic& other ) const
{
    PT_UNIMPLEMENTED_FUNCTION
}


void CameraOrtographic::
RotateCamera( float x_angle, float z_angle )
{
    PT_UNIMPLEMENTED_FUNCTION
}


void CameraOrtographic::
LookAt( const float3& lookat_pos )
{
    PT_UNIMPLEMENTED_FUNCTION
}


float4x4 CameraOrtographic::
GetRotationMtx() const
{
    PT_UNIMPLEMENTED_FUNCTION
}


math::float4x4 engine::CameraOrtographic::
GetViewMtx() const
{
    auto root = this->GetRootComponent_NoLock();

    const math::float3 pos = root->GetPosition();
    //const math::float4 ori = root->GetOrientation();

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


math::float4x4 engine::CameraOrtographic::
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

    //float NearZ = 0.1f;
    //float FarZ  = 15.0f;

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


float CameraOrtographic::
GetPitch() const
{
    PT_UNIMPLEMENTED_FUNCTION
}


float CameraOrtographic::
GetRoll() const
{
    PT_UNIMPLEMENTED_FUNCTION
}


float CameraOrtographic::
GetYaw() const
{
    PT_UNIMPLEMENTED_FUNCTION
}


void engine::CameraOrtographic::
Move( const math::float3& dir )
{
    auto root = this->GetRootComponent_NoLock();
    auto pos = root->GetPosition();

    pos.z = 0;
    pos += dir;
    pos.z = 0;
    root->SetPosition( pos );

    UpdateData();
}


void CameraOrtographic::
SetNearClippingDistance( float val )
{
    PT_UNIMPLEMENTED_FUNCTION
}


void CameraOrtographic::
SetFarClippingDistance( float val )
{
    PT_UNIMPLEMENTED_FUNCTION
}


void CameraOrtographic::
OnTick( float t, float dt )
{}


void CameraOrtographic::
OnSpawned()
{}


void CameraOrtographic::
OnDespawned()
{}


void CameraOrtographic::
Construct()
{
    SetAspectRatio( 16.0f/9.0f ); //TODO: put this in config
    SetZoom( 1.0f );

    UpdateData();
}


void engine::CameraOrtographic::
UpdateData()
{}
