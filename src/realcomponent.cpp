#include "realcomponent.h"

#include <assert.h>

using namespace engine;


void RealComponent::
RegisterRealComponentParts(RealComponent *component)
{}

void RealComponent::
UnregisterRealComponentParts(RealComponent *component)
{}

RealComponent::
RealComponent()
{}

RealComponent::
~RealComponent()
{}

bool RealComponent::
operator==(const RealComponent &other)
{
    assert(false); //TODO: implement
    return true;
}

void RealComponent::
tick(float t, float dt)
{}

void RealComponent::
OnRegistered()
{}

void RealComponent::
OnUnregistered()
{}

void RealComponent::
onSpawn()
{}


