#include "engine/test/ActorTester/PlayerPawn.h"

using namespace engine;

using namespace math;

PlayerPawn::
PlayerPawn( const std::string& name ):
    Actor( name ),
    mColorRectComp( Component::GenerateComponentName( name, "mColorRectComp" ) )
{
    init();
}


PlayerPawn::
~PlayerPawn()
{}


void PlayerPawn::
init()
{
    this->AddComponent( &mColorRectComp );
    mColorRectComp.SetHeight(1.0f);
    mColorRectComp.SetWidth(1.0f);
    mColorRectComp.SetPosition( float3(0.0f, 0.0f, 0.0f) );
    mColorRectComp.SetFrameColor( float3::white );
    mColorRectComp.SetFrameEnabled(true);
    mColorRectComp.SetParent( this->GetRootComponent_NoLock() );

    Actor::FlushMessages_NoDelay( *this );
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
OnTick(float t, float dt)
{
    //Actor::OnTick( t, dt );
}
