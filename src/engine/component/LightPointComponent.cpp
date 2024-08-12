#include "engine/component/LightPointComponent.h"

using namespace engine;

LightPointComponent::
LightPointComponent( const std::string& name ):
    RealComponent( name )
{}


void LightPointComponent::
OnTick( float t, float dt )
{

}


void LightPointComponent::
OnDraw( float t, float dt )
{

}


bool LightPointComponent::
OnCreateContext()
{
    return true;
}


bool LightPointComponent::
OnDestroyContext()
{
    return true;
}


