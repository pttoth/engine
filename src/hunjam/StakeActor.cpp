#include "hunjam//StakeActor.h"

using namespace engine;

StakeActor::
StakeActor( const std::string& name ):
    Actor( name )
{}


StakeActor::
~StakeActor()
{}


void StakeActor::
OnTick( float t, float dt )
{}


void StakeActor::
OnSpawned()
{}


void StakeActor::
OnDespawned()
{}


bool StakeActor::
OnCreateRenderContext()
{
    return true;
}


void StakeActor::
OnDestroyRenderContext()
{}
