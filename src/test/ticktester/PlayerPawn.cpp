#include "test/ticktester/PlayerPawn.h"

#include <assert.h>

using namespace pt;
using namespace pt::ticktester;


PlayerPawn::
PlayerPawn():
    Entity()
{
    this->addComponent( &mBillboardComponent );
    mBillboardComponent.setPosition( pt::math::float3(0.0f, 0.0f, 0.0f) );
    mBillboardComponent.SetBaseColor( pt::math::float3::blue );
    //mBillboardComponent.SetBaseColor( pt::math::float3(0.0f, 0.0f, 0.0f) );
    mBillboardComponent.setParent( this->getRootComponent() );


}


PlayerPawn::
~PlayerPawn()
{

    this->removeComponent( &mBillboardComponent );
}


bool PlayerPawn::
operator==(const PlayerPawn &other) const
{
    assert(false); //TODO: implement

    return false;
}


void PlayerPawn::
OnRegister()
{


}


void PlayerPawn::
OnUnregister()
{


}

void PlayerPawn::OnCreateContext()
{

}

void PlayerPawn::OnDeleteContext()
{

}


void PlayerPawn::
tick(float t, float dt)
{

}
