#include "DrawableComponent.h"

#include "services.h"
#include "enginecontrol.h"

using namespace engine;



void DrawableComponent::
OnCreateContext()
{}


void DrawableComponent::
OnDestroyContext()
{}


DrawableComponent::
DrawableComponent(const std::string &name):
    RealComponent(name),
    mContextInitialized(false)
{}


DrawableComponent::
DrawableComponent(const DrawableComponent &other):
    RealComponent(other),
    mContextInitialized(false)
{

}


DrawableComponent::
~DrawableComponent()
{
    DestroyContext();
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
{
    CreateContext();
}


void DrawableComponent::
OnUnregistered()
{

}


void DrawableComponent::
CreateContext()
{
    if( !mContextInitialized ){
        OnCreateContext();
        mContextInitialized = true;
    }
}

void DrawableComponent::
DestroyContext()
{
    if( mContextInitialized ){
        OnDestroyContext();
        mContextInitialized = false;
    }
}

