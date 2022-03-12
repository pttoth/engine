#include "DrawableComponent.h"

#include "services.h"
#include "enginecontrol.h"

using namespace engine;



void DrawableComponent::
OnCreateContext()
{

}


void DrawableComponent::
OnDestroyContext()
{

}


void DrawableComponent::
Spawn()
{
    RealComponent::Spawn();
    EngineControl* gc = Services::getGameControl();
    gc->AddDrawable(this);
}

void DrawableComponent::
Despawn()
{
    RealComponent::Despawn();
    EngineControl* gc = Services::getGameControl();
    gc->RemoveDrawable(this);
}

void DrawableComponent::
OnRegistered()
{}


void DrawableComponent::
OnUnregistered()
{}

