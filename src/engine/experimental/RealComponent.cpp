#include "engine/experimental/RealComponent.h"

#include "engine/experimental/ComponentVisitor.h"

#include <assert.h>

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


void RealComponent::
OnAddedToEntity(ComponentVisitor &visitor)
{
    assert( false );
}


void RealComponent::
OnRemovedFromEntity(ComponentVisitor &visitor)
{
    assert( false );
}


void RealComponent::
Tick(float t, float dt)
{}


void RealComponent::
Decouple()
{}


void RealComponent::
OnSpawned()
{}


void RealComponent::
OnDespawned()
{}
