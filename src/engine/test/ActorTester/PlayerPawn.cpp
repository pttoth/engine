#include "engine/test/ActorTester/PlayerPawn.h"

using namespace engine;
using namespace engine::experimental;


PlayerPawn::
PlayerPawn( const std::string& name ):
    Actor( name ),
    mColorRectComp( Component::GenerateComponentName( name, "mColorRectComp" ) )
{}


PlayerPawn::
PlayerPawn( const PlayerPawn& other ):
    Actor( other ),
    mColorRectComp( Component::GenerateComponentName( GetName(), "mColorRectComp" ) )
{}


PlayerPawn::
PlayerPawn( PlayerPawn&& source ):
    Actor( source ),
    mColorRectComp( Component::GenerateComponentName( GetName(), "mColorRectComp" ) )
{}


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
Tick()
{

}


void PlayerPawn::
TickComponents()
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


