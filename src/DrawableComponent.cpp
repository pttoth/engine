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
OnRegistered()
{}


void DrawableComponent::
OnUnregistered()
{}


void DrawableComponent::
OnSpawn()
{
    EngineControl* gc = Services::getGameControl();
    gc->AddDrawable(this);
}


void DrawableComponent::
OnDespawn()
{
    EngineControl* gc = Services::getGameControl();
    gc->RemoveDrawable(this);
}
