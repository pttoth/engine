#include "engine/experimental/PositionComponent.h"

#include <assert.h>

using namespace engine;
using namespace engine::experimental;

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
