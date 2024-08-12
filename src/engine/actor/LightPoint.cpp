#include "engine/actor/LightPoint.h"

using namespace engine;

LightPoint::
LightPoint( const std::string& name ):
    Actor( name )
{}


LightPoint::
~LightPoint()
{}


void LightPoint::
OnTick( float t, float dt )
{}


void LightPoint::
OnSpawned()
{}


void LightPoint::
OnDespawned()
{}


bool LightPoint::
OnCreateRenderContext()
{
    return true;
}


void LightPoint::
OnDestroyRenderContext()
{}
