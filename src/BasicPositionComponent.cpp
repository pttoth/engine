#include "BasicPositionComponent.h"

#include <assert.h>

using namespace engine;


BasicPositionComponent::
BasicPositionComponent(const std::string& name):
    WorldComponent(name)
{}


BasicPositionComponent::
BasicPositionComponent(const BasicPositionComponent& other):
    WorldComponent(other)
{}


BasicPositionComponent::
~BasicPositionComponent()
{}


bool BasicPositionComponent::
operator==(const BasicPositionComponent &other)
{
    assert(false); //TODO: implement
    return true;
}


void BasicPositionComponent::
tick(float t, float dt)
{}


void BasicPositionComponent::
OnSpawn()
{}


void BasicPositionComponent::
OnDespawn()
{}


