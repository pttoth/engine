#include "realcomponent.h"

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
RealComponent(const RealComponent &other)
{}

RealComponent::
RealComponent(RealComponent &&other)
{}

RealComponent::
~RealComponent()
{}

RealComponent &RealComponent::
operator=(const RealComponent &other)
{}

RealComponent &RealComponent::
operator=(RealComponent &&other)
{}

bool RealComponent::
operator==(const RealComponent &other) const
{}

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


