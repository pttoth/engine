#include "engine/experimental/SDLComponent.h"

using namespace engine;
using namespace engine::experimental;

SDLComponent::
SDLComponent( const std::string& name ):
    RealComponent( name )
{}


SDLComponent::
~SDLComponent()
{}


void SDLComponent::
Draw( float t, float dt )
{}


void SDLComponent::
Tick( float t, float dt )
{
    RealComponent::Tick( t, dt );
}

