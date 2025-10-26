#include "hunjam/Food.h"

using namespace engine;

Food::
Food( const std::string& name ):
    Actor( name )
{}


Food::
~Food()
{}


void Food::
OnTick( float t, float dt )
{}


void Food::
OnSpawned()
{}


void Food::
OnDespawned()
{}


bool Food::
OnCreateRenderContext()
{
    return true;
}


void Food::
OnDestroyRenderContext()
{}
