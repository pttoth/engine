#include "RealComponent.h"

#include <assert.h>

using namespace engine;


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




