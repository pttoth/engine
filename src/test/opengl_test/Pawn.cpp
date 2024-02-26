#include "test/opengl_test/Pawn.h"


Pawn::
Pawn( const std::string& name ):
    engine::Actor( name )
{}


Pawn::
~Pawn()
{}


bool Pawn::
OnCreateRenderContext()
{


    return true;
}


void Pawn::
OnDestroyRenderContext()
{

}


void Pawn::
OnTick( float t, float dt )
{}


void Pawn::
OnSpawned()
{}


void Pawn::
OnDespawned()
{}
