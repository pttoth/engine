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

float3
CalculateFloatingRecPosition(float t)
{
    float3 retval;
    float r = 0.3f;
    float speed = 3.0f;

    retval.x = cosf(t*speed) * r;
    retval.y = sinf(t*speed) * r;
    retval.z = 0;

    return retval;
}


PlayerPawn::
PlayerPawn(const std::string& name):
    Entity(name),
    mMainRect( GenerateComponentName( this->GetName() , "mMainRect") ),
    mSubRect( GenerateComponentName( this->GetName(), "mSubRect") ),
    mFloatingRect( GenerateComponentName( this->GetName(), "mFloatingRect" ) ),
    mLastFloatingTime(0.0f)

{
    this->addComponent( &mMainRect );
    mMainRect.SetHeight(1.0f);
    mMainRect.SetWidth(1.0f);
    //mBillboardComponent.SetMode(engine::SDLBillboardComponent::Mode::FramedRGBA);
    mMainRect.setPosition( float3(0.0f, 0.0f, 0.0f) );
    mMainRect.SetBaseColor( float3::yellow, 1.0f );
    //mBillboardComponent.SetFrameColor( float3::blue );
    mMainRect.SetFrameEnabled(true);
    //mBillboardComponent.SetBaseColor( pt::math::float3(0.0f, 0.0f, 0.0f) );
    mMainRect.setParent( this->getRootComponent() );

    this->addComponent( &mSubRect );
    mSubRect.SetHeight(0.5f);
    mSubRect.SetWidth(0.5f);
    mSubRect.setPosition( float3(0.2f, 0.2f, 0.0f) );
    mSubRect.SetBaseColor( float3::green, 1.0f);
    //mBillboardComponent.SetFrameColor( float3::cyan );
    //mBillboardComponent.SetFrameEnabled(true);

    //mBbc.SetBaseColor( pt::math::float3(0.0f, 0.0f, 0.0f) );
    mSubRect.setParent( &mMainRect );

    this->addComponent( &mFloatingRect );
    mFloatingRect.SetHeight(0.15f);
    mFloatingRect.SetWidth(0.15f);
    mFloatingRect.SetBaseColor(float3::red, 0.75f);

    mFloatingRect.setParent( &mSubRect );

}


PlayerPawn::
PlayerPawn(const PlayerPawn &other):
    Entity(other),
    mMainRect( GenerateComponentName( this->GetName() , "mMainRect") ),
    mSubRect( GenerateComponentName( this->GetName(), "mSubRect") ),
    mFloatingRect( GenerateComponentName( this->GetName(), "mFloatingRect" ) ),
    mLastFloatingTime(0.0f)
{
    assert(false);
    //TODO: implement
}


PlayerPawn::
~PlayerPawn()
{
    mMainRect.setParent(nullptr);
    this->removeComponent( &mMainRect );
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


engine::SDLBillboardComponent *PlayerPawn::
getMainRect()
{
    return &mMainRect;
}


engine::SDLBillboardComponent* PlayerPawn::
getSubRect()
{
    return &mSubRect;
}


engine::SDLBillboardComponent *PlayerPawn::
getFloatingRect()
{
    return &mFloatingRect;
}


void PlayerPawn::
tick(float t, float dt)
{
    mLastFloatingTime = t;

    float3 newpos = CalculateFloatingRecPosition(t);

    mFloatingRect.setPosition(newpos);
}
