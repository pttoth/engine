#include "test/ticktester/PlayerPawn.h"

#include <assert.h>
#include <sstream>


using namespace test;
using namespace test::ticktester;


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
{
    this->addComponent( &mBillboardComponent );
    mBillboardComponent.SetHeight(1.0f);
    mBillboardComponent.SetWidth(1.0f);
    mBillboardComponent.setPosition( pt::math::float3(0.0f, 0.0f, 0.0f) );
    mBillboardComponent.SetBaseColor( pt::math::float3::blue );
    //mBillboardComponent.SetBaseColor( pt::math::float3(0.0f, 0.0f, 0.0f) );
    mBillboardComponent.setParent( this->getRootComponent() );
}


PlayerPawn::
PlayerPawn(const PlayerPawn &other):
    Entity(other),
    mBillboardComponent( GenerateComponentName( this->GetName(), "mBillboardComponent") )
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


void PlayerPawn::
tick(float t, float dt)
{

}
