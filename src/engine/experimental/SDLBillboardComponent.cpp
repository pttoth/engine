#include "engine/experimental/SDLBillboardComponent.h"

using namespace engine;
using namespace engine::experimental;

SDLBillboardComponent::
SDLBillboardComponent( const std::string& name ):
    SDLComponent( name )
{}


SDLBillboardComponent::
SDLBillboardComponent( const SDLBillboardComponent& other ):
    SDLComponent( other )
{}


SDLBillboardComponent::
SDLBillboardComponent( SDLBillboardComponent&& source ):
    SDLComponent( source )
{}


SDLBillboardComponent::
~SDLBillboardComponent()
{}


SDLBillboardComponent &SDLBillboardComponent::
operator=( const SDLBillboardComponent& other )
{}


SDLBillboardComponent &SDLBillboardComponent::
operator=( SDLBillboardComponent&& source )
{}


bool SDLBillboardComponent::
operator==( const SDLBillboardComponent& other ) const
{}
