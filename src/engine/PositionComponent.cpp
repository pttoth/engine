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
Tick(float t, float dt)
{}


void PositionComponent::
OnSpawned()
{}


void PositionComponent::
OnDespawned()
{}
