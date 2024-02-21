#include "engine/BillboardComponent.h"

#include "pt/utility.hpp"

using namespace engine;


BillboardComponent::
BillboardComponent( const std::string& name ):
    RealComponent( name )
{}


void BillboardComponent::
Draw(float t, float dt)
{}


void BillboardComponent::
OnSpawned()
{
    //add this component to the drawing manager
}


void BillboardComponent::
OnDespawned()
{
    //remove this component from the drawing manager
}


void BillboardComponent::
OnTick(float t, float dt)
{}


bool BillboardComponent::
OnCreateContext()
{
    //set up billboard mesh 6 vertex 2 triangles
}


bool BillboardComponent::
OnDestroyContext()
{
    //destroy billboard mesh data
}
