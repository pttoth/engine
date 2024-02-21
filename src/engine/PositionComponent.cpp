#include "engine/PositionComponent.h"

#include <assert.h>

using namespace engine;

PositionComponent::
PositionComponent( const std::string& name ):
    WorldComponent( name )
{}


PositionComponent::
~PositionComponent()
{}


void PositionComponent::
OnSpawned()
{}


void PositionComponent::
OnDespawned()
{}


void PositionComponent::
OnTick( float t, float dt )
{}
