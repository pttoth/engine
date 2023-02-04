#include "engine/experimental/PositionComponent.h"

#include "engine/experimental/ComponentVisitor.h"

#include <assert.h>

using namespace engine;
using namespace engine::experimental;

PositionComponent::
PositionComponent( const std::string& name ):
    WorldComponent( name )
{}


PositionComponent::
PositionComponent( const PositionComponent& other ):
    WorldComponent( other )
{}


PositionComponent::
PositionComponent( PositionComponent&& source ):
    WorldComponent( source )
{}


PositionComponent::
~PositionComponent()
{}


bool PositionComponent::
operator==( const PositionComponent& other ) const
{
    assert( false );
}


void PositionComponent::
Tick(float t, float dt)
{}


void PositionComponent::
OnSpawned()
{}


void PositionComponent::
OnDespawned()
{}
