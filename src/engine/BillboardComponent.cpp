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
{}


void BillboardComponent::
OnDespawned()
{}


void BillboardComponent::
OnTick(float t, float dt)
{}


bool BillboardComponent::
OnCreateContext()
{}


bool BillboardComponent::
OnDestroyContext()
{}
