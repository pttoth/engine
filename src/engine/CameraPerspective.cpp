#include "engine/CameraPerspective.h"

#include "pt/utility.hpp"

using namespace engine;
using namespace math;


CameraPerspective::
CameraPerspective( const std::string& name ):
    Camera( name )
{
    Construct();
}


bool CameraPerspective::
operator==( const CameraPerspective& other ) const
{
    PT_UNIMPLEMENTED_FUNCTION
}


const math::float4x4 CameraPerspective::
GetRotationMtx() const
{
    const float4 ori = this->GetRootComponent_NoLock()->GetOrientation();
    float4x4  orient = float4x4::identity;
    orient._00 = ori.x;
    orient._11 = ori.y;
    orient._22 = ori.z * (-1); //OpenGL
    //orient._22 = ori.z; //DirectX

    return orient;
}


const math::float4x4 CameraPerspective::
GetViewMtx() const
{
    const float3 pos = this->GetRootComponent_NoLock()->GetPosition();

    float4x4  translation = float4x4::identity;
    translation.m[0][3] -= pos.x;
    translation.m[1][3] -= pos.y;
    translation.m[2][3] -= pos.z;

    return translation * GetRotationMtx();
}


const math::float4x4 CameraPerspective::
GetProjMtx() const
{
    PT_UNIMPLEMENTED_FUNCTION
}


void CameraPerspective::
Move( const math::float3& dir )
{
    Camera::Move( dir );
    UpdateData();
}


bool CameraPerspective::
OnCreateRenderContext()
{
    PT_UNIMPLEMENTED_FUNCTION
}


void CameraPerspective::
OnDestroyRenderContext()
{

}


void CameraPerspective::
OnTick( float t, float dt )
{

}


void CameraPerspective::
OnSpawned()
{

}


void CameraPerspective::
OnDespawned()
{

}


void CameraPerspective::
Construct()
{

}


void CameraPerspective::
UpdateData()
{

}
