#include "engine/test/ticktester/PlayerPawn.h"

#include "engine/EngineControl.h"
#include "engine/Services.h"

#include "engine/Scheduler.h"

#include <assert.h>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <random>

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
    mFloatingRect( GenerateComponentName( this->GetName(), "mFloatingRect" ) )
{
    this->AddComponent( &mMainRect );
    mMainRect.SetHeight(1.0f);
    mMainRect.SetWidth(1.0f);
    mMainRect.SetPosition( float3(0.0f, 0.0f, 0.0f) );
    mMainRect.SetFrameColor( float3::white );
    mMainRect.SetFrameEnabled(true);
    mMainRect.SetParent( this->GetRootComponent() );


    this->AddComponent( &mSubRect );
    mSubRect.SetHeight(0.5f);
    mSubRect.SetWidth(0.5f);
    mSubRect.SetPosition( float3(-1.0f, 0.0f, 0.0f) );
    mSubRect.SetFrameColor( float3::white );
    mSubRect.SetFrameEnabled(true);
    mSubRect.SetParent( &mMainRect );


    this->AddComponent( &mFloatingRect );
    mFloatingRect.SetHeight(0.15f);
    mFloatingRect.SetWidth(0.15f);
    mSubRect.SetFrameColor( float3::white );
    mSubRect.SetFrameEnabled(true);
    mFloatingRect.SetParent( &mSubRect );
}


PlayerPawn::
PlayerPawn(const PlayerPawn &other):
    Entity(other),
    mMainRect( GenerateComponentName( this->GetName() , "mMainRect") ),
    mSubRect( GenerateComponentName( this->GetName(), "mSubRect") ),
    mFloatingRect( GenerateComponentName( this->GetName(), "mFloatingRect" ) )
{
    assert(false);
    //TODO: implement
}


PlayerPawn::
~PlayerPawn()
{
    mMainRect.SetParent(nullptr);
    this->RemoveComponent( &mMainRect );
}


bool PlayerPawn::
operator==(const PlayerPawn &other) const
{
    assert(false); //TODO: implement

    return false;
}


void PlayerPawn::
OnSpawned()
{
    engine::Scheduler* s = engine::Services::GetScheduler();

    mRectangles.reserve(8);
    mRectangles.push_back(&mMainRect);
    mRectangles.push_back(&mSubRect);
    mRectangles.push_back(&mFloatingRect);

    mMainRect.RegisterTickFunction();
    mSubRect.RegisterTickFunction();
    mFloatingRect.RegisterTickFunction();

    mMainRect.SetID(0);
    mSubRect.SetID(1);
    mFloatingRect.SetID(2);

    ShuffleTickOrder();
}


void PlayerPawn::
OnDespawned()
{

}


void PlayerPawn::
SetFloatRadius(float r)
{
    mFloatRadius = std::max(r, 0.0f);
}


float PlayerPawn::
GetFloatRadius() const
{
    return mFloatRadius;
}


void PlayerPawn::
ShuffleTickOrder()
{
    engine::Scheduler* s = engine::Services::GetScheduler();

    for(auto r : mRectangles){
        s->RemoveTicker(*r);
    }

    std::random_device rd;
    std::mt19937 g(rd());

    std::shuffle(mRectangles.begin(), mRectangles.end(), g);

    for(auto r : mRectangles){
        s->AddTicker(*r);
    }


    s->AddTickDependency( mSubRect, mMainRect );
    s->AddTickDependency( mFloatingRect, mMainRect );
    s->AddTickDependency( mFloatingRect, mSubRect );

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

    mFloatingRect.SetPosition(newpos);

    int asd = (int) t;
    if( 1 <= asd%2  ){
        ShuffleTickOrder();
    }
}


void PlayerPawn::
OnMouseButtonDown(int32_t x, int32_t y, uint8_t button, uint8_t clicks, uint32_t timestamp, uint32_t mouseid)
{}


void PlayerPawn::
OnMouseButtonUp(int32_t x, int32_t y, uint8_t button, uint8_t clicks, uint32_t timestamp, uint32_t mouseid)
{}


void PlayerPawn::
OnMouseMotion(int32_t x, int32_t y, int32_t x_rel, int32_t y_rel, uint32_t timestamp, uint32_t mouseid)
{}


void PlayerPawn::
OnMouseWheel(int32_t x, int32_t y, uint32_t timestamp, uint32_t mouseid, uint32_t direction)
{}


void PlayerPawn::
OnKeyDown(SDL_Keycode keycode, uint16_t keymod, uint32_t timestamp, uint8_t repeat)
{}


void PlayerPawn::
OnKeyUp(SDL_Keycode keycode, uint16_t keymod, uint32_t timestamp, uint8_t repeat)
{}
