#include "test/opengl_test/BillboardActor.h"

#include "engine/gl/Buffer.hpp"

BillboardActor::
BillboardActor( const std::string& name ):
    engine::Actor( name )
{}


BillboardActor::
~BillboardActor()
{}


bool BillboardActor::
OnCreateRenderContext()
{
    return true;
}


void BillboardActor::
OnDestroyRenderContext()
{}


void BillboardActor::
OnTick( float t, float dt )
{}


void BillboardActor::
OnSpawned()
{}


void BillboardActor::
OnDespawned()
{}
