#include "engine/test/ActorTester/ColorRectComponent.h"

#include <assert.h>

using namespace engine;
using namespace engine::experimental;

ColorRectComponent::
ColorRectComponent(const std::string &name):
    SDLBillboardComponent( name )
{}


ColorRectComponent::
ColorRectComponent( const ColorRectComponent& other ):
    SDLBillboardComponent( other )
{}


ColorRectComponent::
ColorRectComponent( ColorRectComponent&& source ):
    SDLBillboardComponent( source )
{}


ColorRectComponent::
~ColorRectComponent()
{}


ColorRectComponent &ColorRectComponent::
operator=( const ColorRectComponent& other )
{}


ColorRectComponent &ColorRectComponent::
operator=( ColorRectComponent&& source )
{}


bool ColorRectComponent::
operator==( const ColorRectComponent& other ) const
{}


void ColorRectComponent::
Draw(float t, float dt)
{
    SDLBillboardComponent::Draw(t, dt );

}


void ColorRectComponent::
OnSpawned()
{
    SDLBillboardComponent::OnSpawned();

}


void ColorRectComponent::
OnDespawned()
{
    SDLBillboardComponent::OnDespawned();

}


bool ColorRectComponent::
OnCreateContext()
{
    bool success = SDLBillboardComponent::OnCreateContext();
    assert( success );
    if( !success ){
        return false;
    }


    return true;
}


bool ColorRectComponent::
OnDestroyContext()
{
    bool success = SDLBillboardComponent::OnDestroyContext();
    assert( success );
    if( !success ){
        return false;
    }


    return true;
}

