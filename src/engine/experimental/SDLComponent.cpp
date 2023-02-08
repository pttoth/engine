#include "engine/experimental/SDLComponent.h"

using namespace engine;
using namespace engine::experimental;

SDLComponent::
SDLComponent( const std::string& name ):
    RealComponent( name )
{}


SDLComponent::
SDLComponent( const SDLComponent& other ):
    RealComponent( other )
{}


SDLComponent::
SDLComponent( SDLComponent&& source ):
    RealComponent( source )
{}


SDLComponent::
~SDLComponent()
{}


SDLComponent &SDLComponent::
operator=( const SDLComponent& other )
{}


SDLComponent &SDLComponent::
operator=( SDLComponent&& source )
{}


bool SDLComponent::
operator==( const SDLComponent& other ) const
{}
