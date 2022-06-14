#include "DrawableComponent.h"

#include "Services.h"
#include "EngineControl.h"

#include "DrawingControl.h"

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

