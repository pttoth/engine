#include "test/ticktester/PlayerPawn.h"

#include <assert.h>
#include <sstream>


using namespace test;
using namespace test::ticktester;

using namespace pt::math;


std::string
GenerateComponentName(const std::string& entityname,
                      const std::string& componentname)
{
    std::stringstream ss;
    ss << entityname << "." << componentname;
    return ss.str();
}


PlayerPawn::
PlayerPawn(const std::string& name):
    Entity(name),
    mBillboardComponent( GenerateComponentName( this->GetName() , "mBillboardComponent") )
    ,mBbc(GenerateComponentName(this->GetName(), "mBbc"))
{
    this->addComponent( &mBillboardComponent );
    mBillboardComponent.SetHeight(1.0f);
    mBillboardComponent.SetWidth(1.0f);
    //mBillboardComponent.SetMode(engine::SDLBillboardComponent::Mode::FramedRGBA);
    mBillboardComponent.setPosition( float3(0.0f, 0.0f, 0.0f) );
    mBillboardComponent.SetBaseColor( float3::red, 1.0f );
    //mBillboardComponent.SetFrameColor( float3::blue );
    mBillboardComponent.SetFrameEnabled(true);
    //mBillboardComponent.SetBaseColor( pt::math::float3(0.0f, 0.0f, 0.0f) );
    mBillboardComponent.setParent( this->getRootComponent() );

    this->addComponent( &mBbc );
    mBbc.SetHeight(0.5f);
    mBbc.SetWidth(0.5f);
    mBbc.setPosition( float3(0.35f, 0.35f, 0.0f) );
    mBbc.SetBaseColor( float3::green, 0.5f);
    //mBillboardComponent.SetFrameColor( float3::cyan );
    //mBillboardComponent.SetFrameEnabled(true);

    //mBbc.SetBaseColor( pt::math::float3(0.0f, 0.0f, 0.0f) );
    mBbc.setParent( &mBillboardComponent );

}


PlayerPawn::
PlayerPawn(const PlayerPawn &other):
    Entity(other),
    mBillboardComponent( GenerateComponentName( this->GetName(), "mBillboardComponent") )
  ,mBbc(GenerateComponentName(this->GetName(), "mBbc"))
{
    assert(false);
    //TODO: implement
}


PlayerPawn::
~PlayerPawn()
{
    mBillboardComponent.setParent(nullptr);
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

engine::SDLBillboardComponent* PlayerPawn::
getBBC()
{
    return &mBbc;
}


void PlayerPawn::
tick(float t, float dt)
{

}
