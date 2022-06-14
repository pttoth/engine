#include "engine/PositionComponent.h"

#include <assert.h>

using namespace engine;


PositionComponent::
PositionComponent(const std::string& name):
    WorldComponent(name)
{}


PositionComponent::
PositionComponent(const PositionComponent& other):
    WorldComponent(other)
{}


PositionComponent::
~PositionComponent()
{}


bool PositionComponent::
operator==(const PositionComponent &other)
{
    assert(false); //TODO: implement
    return true;
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

