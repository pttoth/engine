#include "test/ticktester/PlayerPawn.h"

#include "EngineControl.h"
#include "Services.h"

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
CalculateFloatingRecPosition(float t, float r)
{
    float3 retval;
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
    mMainRect.setPosition( float3(0.0f, 0.0f, 0.0f) );
    mMainRect.SetFrameColor( float3::white );
    mMainRect.SetFrameEnabled(true);
    mMainRect.setParent( this->getRootComponent() );


    this->addComponent( &mSubRect );
    mSubRect.SetHeight(0.5f);
    mSubRect.SetWidth(0.5f);
    mSubRect.setPosition( float3(-1.0f, 0.0f, 0.0f) );
    mSubRect.SetFrameColor( float3::white );
    mSubRect.SetFrameEnabled(true);
    mSubRect.setParent( &mMainRect );
    //ec->AddTickDependency( &mSubRect, &mMainRect );


    this->addComponent( &mFloatingRect );
    mFloatingRect.SetHeight(0.15f);
    mFloatingRect.SetWidth(0.15f);
    mSubRect.SetFrameColor( float3::white );
    mSubRect.SetFrameEnabled(true);
    mFloatingRect.setParent( &mSubRect );
    //ec->AddTickDependency( &mMainRect, &mFloatingRect );

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


void PlayerPawn::
SetFloatRadius(float r)
{
    mFloatRadius = r;
}


float PlayerPawn::
GetFloatRadius() const
{
    return mFloatRadius;
}


ColorRectComponent *PlayerPawn::
getMainRect()
{
    return &mMainRect;
}


ColorRectComponent* PlayerPawn::
getSubRect()
{
    return &mSubRect;
}


ColorRectComponent *PlayerPawn::
getFloatingRect()
{
    return &mFloatingRect;
}


void PlayerPawn::
Tick(float t, float dt)
{
    mLastFloatingTime = t;

    float3 newpos = CalculateFloatingRecPosition(t, mFloatRadius);

    mFloatingRect.setPosition(newpos);
}
