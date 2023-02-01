#include "engine/experimental/RealComponent.h"

using namespace engine;
using namespace engine::experimental;

RealComponent::
RealComponent( const std::string& name ):
    WorldComponent( name )
{}


RealComponent::
RealComponent( const RealComponent& other ):
    WorldComponent( other )
{}


RealComponent::
RealComponent( RealComponent&& source ):
    WorldComponent( source )
{}


RealComponent::
~RealComponent()
{}
