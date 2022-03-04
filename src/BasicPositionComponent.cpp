#include "BasicPositionComponent.h"

#include <assert.h>

using namespace engine;


BasicPositionComponent::
BasicPositionComponent()
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
OnRegistered()
{}


void BasicPositionComponent::
OnUnregistered()
{}


void BasicPositionComponent::
onSpawn()
{}


