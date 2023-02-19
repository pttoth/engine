#include "engine/test/ActorTester/PlayerPawn.h"

using namespace engine;
using namespace engine::experimental;

using namespace pt::math;

PlayerPawn::
PlayerPawn( const std::string& name ):
    Actor( name ),
    mColorRectComp( Component::GenerateComponentName( name, "mColorRectComp" ) )
{
    init();
}


PlayerPawn::
PlayerPawn( const PlayerPawn& other ):
    Actor( other ),
    mColorRectComp( Component::GenerateComponentName( GetName(), "mColorRectComp" ) )
{
    init();
}


PlayerPawn::
PlayerPawn( PlayerPawn&& source ):
    Actor( source ),
    mColorRectComp( Component::GenerateComponentName( GetName(), "mColorRectComp" ) )
{
    init();
}


PlayerPawn::
~PlayerPawn()
{}


PlayerPawn &PlayerPawn::
operator=( const PlayerPawn& other )
{}


PlayerPawn &PlayerPawn::
operator=( PlayerPawn&& source )
{}


bool PlayerPawn::
operator==( const PlayerPawn& other ) const
{}


void PlayerPawn::
OnTick(float t, float dt)
{
    //Actor::OnTick( t, dt );
}


void PlayerPawn::
init()
{
    this->AddComponent( &mColorRectComp );
    mColorRectComp.SetHeight(1.0f);
    mColorRectComp.SetWidth(1.0f);
    mColorRectComp.SetPosition( float3(0.0f, 0.0f, 0.0f) );
    mColorRectComp.SetFrameColor( float3::white );
    mColorRectComp.SetFrameEnabled(true);
    mColorRectComp.SetParent( this->GetRootComponent() );

    Actor::FlushMessages( *this );
}


void PlayerPawn::
OnSpawned()
{

}


void PlayerPawn::
OnDespawned()
{

}


void PlayerPawn::
SetParent()
{

}


void PlayerPawn::
RemoveParent()
{

}


