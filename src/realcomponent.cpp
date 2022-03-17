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
RealComponent(const std::string& name):
    WorldComponent(name)
{}


RealComponent::
RealComponent(const RealComponent &other):
    WorldComponent(other)
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




