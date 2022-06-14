#include "engine/DrawableComponent.h"

#include "engine/Services.h"
#include "engine/EngineControl.h"

#include "engine/DrawingControl.h"

using namespace engine;



void DrawableComponent::
OnCreateContext()
{}


void DrawableComponent::
OnDestroyContext()
{}


DrawableComponent::
DrawableComponent(const std::string &name):
    RealComponent(name)
{}


DrawableComponent::
DrawableComponent(const DrawableComponent &other):
    RealComponent(other)
{}


DrawableComponent::
~DrawableComponent()
{
    DestroyContext();
}


void DrawableComponent::
Spawn()
{
    RealComponent::Spawn();

    DrawingControl* dc = Services::GetDrawingControl();
    dc->AddDrawable(this);
}

void DrawableComponent::
Despawn()
{
    RealComponent::Despawn();
    DrawingControl* dc = Services::GetDrawingControl();
    dc->RemoveDrawable(this);
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

