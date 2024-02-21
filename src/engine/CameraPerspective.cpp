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
    //TODO: implement
    PT_UNIMPLEMENTED_FUNCTION
}


const math::float4x4 CameraPerspective::
GetRotationMtx() const
{
    //TODO: implement
    PT_UNIMPLEMENTED_FUNCTION
}


const math::float4x4 CameraPerspective::
GetViewMtx() const
{

}


const math::float4x4 CameraPerspective::
GetProjMtx() const
{

}


void CameraPerspective::
Move( const math::float3& dir )
{
    Camera::Move( dir );
    UpdateData();
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
