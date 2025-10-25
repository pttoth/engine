#include "hunjam//StakeActor.h"

using namespace engine;

StakeActor::
StakeActor( const std::string& name ):
    engine::Actor( name ), mMesh( "mMesh" )
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
